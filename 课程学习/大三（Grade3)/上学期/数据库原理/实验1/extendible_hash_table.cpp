//===----------------------------------------------------------------------===//
//
//                          BusTub
//
// extendible_hash_table.cpp
//
// Identification: src/container/hash/extendible_hash_table.cpp
//
// Copyright (c) 2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <cstdlib>
#include <functional>
#include <list>
#include <utility>

#include "container/hash/extendible_hash_table.h"
#include "storage/page/page.h"

namespace bustub {

/**
 * 构造函数
 * @param bucket_size 每个桶能容纳的最大元素数量
 */
template <typename K, typename V>
ExtendibleHashTable<K, V>::ExtendibleHashTable(size_t bucket_size) : bucket_size_(bucket_size) {
  // 初始化：创建一个全局深度为 0 的目录
  // 此时目录中只有一个指针，指向一个初始的空桶（局部深度也为 0）
  dir_.push_back(std::make_shared<Bucket>(bucket_size_, 0));
}

/**
 * 计算 Key 在目录（Directory）中的索引
 * 逻辑：取 Hash 值的低 global_depth_ 位
 */
template <typename K, typename V>
auto ExtendibleHashTable<K, V>::IndexOf(const K &key) -> size_t {
  int mask = (1 << global_depth_) - 1;
  return std::hash<K>()(key) & mask;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepth() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetGlobalDepthInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepthInternal() const -> int {
  return global_depth_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepth(int dir_index) const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetLocalDepthInternal(dir_index);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepthInternal(int dir_index) const -> int {
  return dir_[dir_index]->GetDepth();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBuckets() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetNumBucketsInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBucketsInternal() const -> int {
  return num_buckets_;
}

/**
 * 查找操作
 * 1. 加锁
 * 2. 计算目录索引
 * 3. 委托给对应的 Bucket 进行查找
 */
template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Find(const K &key, V &value) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);
  size_t index = IndexOf(key);
  return dir_[index]->Find(key, value);
}

/**
 * 删除操作
 * 1. 加锁
 * 2. 计算目录索引
 * 3. 委托给对应的 Bucket 进行删除
 */
template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Remove(const K &key) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);
  size_t index = IndexOf(key);
  return dir_[index]->Remove(key);
}

/**
 * 插入操作 (核心逻辑)
 * 处理流程：直接插入 -> 失败则分裂桶 -> 再次尝试
 */
template <typename K, typename V>
void ExtendibleHashTable<K, V>::Insert(const K &key, const V &value) {
  std::scoped_lock<std::mutex> lock(latch_); // 全局加锁，保证线程安全
  
  while (true) {
    // 1. 定位桶
    size_t index = IndexOf(key);
    auto target_bucket = dir_[index];
    
    // 2. 尝试直接插入
    // 如果桶未满，或者Key已存在(更新值)，Insert会返回 true
    if (target_bucket->Insert(key, value)) {
      return;  // 插入成功，结束
    }
    
    // 3. 桶已满，需要执行分裂逻辑 (Split)
    // -----------------------------------------------------------
    int local_depth = target_bucket->GetDepth();
    int global_depth = GetGlobalDepthInternal();
    
    // 3a. 检查目录是否需要扩容 (Directory Expansion)
    // 如果当前桶的局部深度等于全局深度，说明目录的位数不足以区分新的分裂
    // 必须增加全局深度，目录大小翻倍
    if (local_depth == global_depth) {
      global_depth_++; // 全局深度 +1
      size_t dir_size = dir_.size();
      // 扩容目录：将现有的所有指针复制一份追加到后面
      // 例如：原本 [PtrA, PtrB]，扩容后变成 [PtrA, PtrB, PtrA, PtrB]
      for (size_t i = 0; i < dir_size; i++) {
        dir_.push_back(dir_[i]);
      }
    }
    
    // 3b. 准备分裂：增加局部深度并创建新桶
    target_bucket->IncrementDepth(); // 原桶深度 +1
    local_depth++; // 更新局部变量以便后续计算
    
    // 创建一个新桶，深度与原桶更新后的深度一致
    auto new_bucket = std::make_shared<Bucket>(bucket_size_, local_depth);
    num_buckets_++;
    
    // 3c. 数据重分布 (Redistribute)
    // 我们需要将原桶(target_bucket)中的所有数据拿出来，
    // 根据新的局部深度（多出来的那一位 bit）重新分配到 "原桶" 或 "新桶"
    auto &items = target_bucket->GetItems();
    // 拷贝一份数据用于遍历，因为我们会清空原链表
    std::list<std::pair<K, V>> temp_items(items.begin(), items.end());
    items.clear(); // 清空原桶，准备重新填充
    
    for (const auto &item : temp_items) {
      size_t hash_value = std::hash<K>()(item.first);
      // mask: 用来取哈希值的低 local_depth 位
      // 例如 local_depth 变成了 3 (二进制 111)，mask 就是 7
      size_t bucket_index = hash_value & ((1 << local_depth) - 1);
      
      // 检查新增的那一位 bit 是 0 还是 1
      // 1 << (local_depth - 1) 对应最高的那一位
      if ((bucket_index & (1 << (local_depth - 1))) == 0) {
        // bit 为 0 -> 留在原桶
        target_bucket->GetItems().push_back(item);
      } else {
        // bit 为 1 -> 移到新桶
        new_bucket->GetItems().push_back(item);
      }
    }
    
    // 3d. 更新目录指针 (Update Directory Pointers)
    // 遍历整个目录，找到所有指向旧桶 (target_bucket) 的指针。
    // 根据索引的第 (local_depth - 1) 位来决定：
    //   - 如果是 0，继续指向旧桶（无需修改，因为默认是复制的旧指针）
    //   - 如果是 1，修改指向新桶
    for (size_t i = 0; i < dir_.size(); i++) {
      if (dir_[i] == target_bucket) {
        // 检查索引 i 的特定位
        if ((i >> (local_depth - 1)) & 1) {
          dir_[i] = new_bucket;
        }
        // else: 保持指向 target_bucket
      }
    }
    
    // 循环继续 (while true)...
    // 分裂完成后，我们回到循环顶部，重新计算 Key 的 Index，
    // 并再次尝试插入。因为有可能分裂一次后，该 Key 所在的桶依然是满的（极端哈希冲突），
    // 所以需要循环直到插入成功。
  }
}

//===--------------------------------------------------------------------===//
// Bucket 实现
//===--------------------------------------------------------------------===//
template <typename K, typename V>
ExtendibleHashTable<K, V>::Bucket::Bucket(size_t array_size, int depth) : size_(array_size), depth_(depth) {}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Find(const K &key, V &value) -> bool {
  // 线性扫描链表查找 Key
  for (const auto &pair : list_) {
    if (pair.first == key) {
      value = pair.second;
      return true;
    }
  }
  return false;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Remove(const K &key) -> bool {
  // 遍历链表，找到 Key 则移除
  for (auto it = list_.begin(); it != list_.end(); ++it) {
    if (it->first == key) {
      list_.erase(it);
      return true;
    }
  }
  return false;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Insert(const K &key, const V &value) -> bool {
  // 1. 检查 Key 是否已存在 (更新语义)
  for (auto &pair : list_) {
    if (pair.first == key) {
      pair.second = value; // 更新值
      return true;
    }
  }
  
  // 2. 检查桶是否已满
  if (IsFull()) {
    return false; // 桶满，通知上层需要分裂
  }
  
  // 3. 插入新元素
  list_.emplace_back(key, value);
  return true;
}

template class ExtendibleHashTable<page_id_t, Page *>;
template class ExtendibleHashTable<Page *, std::list<Page *>::iterator>;
template class ExtendibleHashTable<int, int>;
// test purpose
template class ExtendibleHashTable<int, std::string>;
template class ExtendibleHashTable<int, std::list<int>::iterator>;

}  // namespace bustub