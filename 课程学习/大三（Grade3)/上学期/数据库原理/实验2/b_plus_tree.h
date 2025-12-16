//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/include/index/b_plus_tree.h
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#pragma once

#include <queue>
#include <string>
#include <vector>

#include "concurrency/transaction.h"
#include "storage/index/index_iterator.h"
#include "storage/page/b_plus_tree_internal_page.h"
#include "storage/page/b_plus_tree_leaf_page.h"

namespace bustub {

#define BPLUSTREE_TYPE BPlusTree<KeyType, ValueType, KeyComparator>

// Latch Crabbing (锁蟹行) 的操作类型
enum class Operation { SEARCH = 0, INSERT, DELETE };

/**
 * B+ 树的主类，提供对外的 API。
 *
 * 实现了简单的 B+ 树数据结构：
 * - 内部页面指导搜索方向。
 * - 叶子页面包含实际数据。
 * (1) 仅支持唯一键 (Unique Key)。
 * (2) 支持 Insert (插入) 和 Remove (删除)。
 * (3) 结构应能动态收缩和增长 (Split/Merge)。
 * (4) 实现用于范围扫描的 Index Iterator。
 */
INDEX_TEMPLATE_ARGUMENTS
class BPlusTree {
  using InternalPage = BPlusTreeInternalPage<KeyType, page_id_t, KeyComparator>;
  using LeafPage = BPlusTreeLeafPage<KeyType, ValueType, KeyComparator>;

 public:
  explicit BPlusTree(std::string name, BufferPoolManager *buffer_pool_manager, const KeyComparator &comparator,
                     int leaf_max_size = LEAF_PAGE_SIZE, int internal_max_size = INTERNAL_PAGE_SIZE);

  // 如果树中没有键值对，返回 true
  auto IsEmpty() const -> bool;

  // 插入键值对
  auto Insert(const KeyType &key, const ValueType &value, Transaction *transaction = nullptr) -> bool;

  // 删除键值对
  void Remove(const KeyType &key, Transaction *transaction = nullptr);

  // 获取给定键对应的值 (点查询)
  auto GetValue(const KeyType &key, std::vector<ValueType> *result, Transaction *transaction = nullptr) -> bool;

  // 获取根节点的 Page ID
  auto GetRootPageId() -> page_id_t;

  // 索引迭代器 (用于范围扫描)
  auto Begin() -> INDEXITERATOR_TYPE;
  auto Begin(const KeyType &key) -> INDEXITERATOR_TYPE;
  auto End() -> INDEXITERATOR_TYPE;

  // 打印树结构 (调试用)
  void Print(BufferPoolManager *bpm);

  // 绘制树结构图 (Graphviz dot 格式)
  void Draw(BufferPoolManager *bpm, const std::string &outf);

  // 从文件读取数据并逐个插入 (测试用)
  void InsertFromFile(const std::string &file_name, Transaction *transaction = nullptr);

  // 从文件读取数据并逐个删除 (测试用)
  void RemoveFromFile(const std::string &file_name, Transaction *transaction = nullptr);

 private:
  // 更新 Header Page 中的 Root Page ID (持久化根节点位置)
  void UpdateRootPageId(int insert_record = 0);

  // B+ 树操作的辅助函数
  
  // 查找包含 key 的叶子页面 (涉及锁蟹行逻辑)
  auto FindLeafPage(const KeyType &key, bool leftMost, Operation op, Transaction *transaction) -> Page *;
  
  // 树为空时，开始新的树 (创建根节点)
  void StartNewTree(const KeyType &key, const ValueType &value);
  
  // 插入到叶子节点
  auto InsertIntoLeaf(const KeyType &key, const ValueType &value, Transaction *transaction) -> bool;
  
  // 分裂叶子节点
  auto Split(LeafPage *leaf_page) -> LeafPage *;
  
  // 分裂内部节点
  auto Split(InternalPage *internal_page) -> InternalPage *;
  
  // 将新分裂出来的节点插入到父节点中 (递归向上)
  void InsertIntoParent(BPlusTreePage *old_node, const KeyType &key, BPlusTreePage *new_node, Transaction *transaction);

  // 删除后的调整：决定是合并 (Coalesce) 还是重分配 (Redistribute)
  template <typename N>
  auto CoalesceOrRedistribute(N *node, Transaction *transaction) -> bool;
  
  // 调整根节点 (如根节点被删空或内部根节点只有一个子节点)
  auto AdjustRoot(BPlusTreePage *old_root_node) -> bool;
  
  // 重分配 (借位)
  template <typename N>
  void Redistribute(N *neighbor_node, N *node, InternalPage *parent, int index, bool from_left);
  
  // 合并 (两个节点合二为一)
  template <typename N>
  auto Coalesce(N *neighbor_node, N *node, InternalPage *parent, int index, Transaction *transaction) -> bool;

  // 并发控制辅助函数
  void UnlockUnpinPages(Transaction *transaction); // 解锁并 Unpin 事务中的所有页面
  void UnlockPages(Transaction *transaction);      // 仅解锁 (用于 Search)

  // 检查节点是否"安全" (插入时未满，删除时多于半满)
  template <typename N>
  auto IsSafe(N *node, Operation op) -> bool;

  /* Debug Routines for FREE!! */
  void ToGraph(BPlusTreePage *page, BufferPoolManager *bpm, std::ofstream &out) const;
  void ToString(BPlusTreePage *page, BufferPoolManager *bpm) const;

  // 成员变量
  std::string index_name_;
  page_id_t root_page_id_;
  BufferPoolManager *buffer_pool_manager_;
  KeyComparator comparator_;
  int leaf_max_size_;
  int internal_max_size_;
  ReaderWriterLatch root_latch_; // 保护 root_page_id_ 的读写锁
};

}  // namespace bustub