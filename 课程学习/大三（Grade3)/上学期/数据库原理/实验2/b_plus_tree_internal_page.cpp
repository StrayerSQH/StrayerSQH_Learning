//===----------------------------------------------------------------------===//
//
//                         CMU-DB 项目 (15-445/645)
//                         ***请勿公开分享***
//
// 文件标识: src/page/b_plus_tree_internal_page.cpp
//
// 版权所有 (c) 2018, 卡内基梅隆大学数据库组
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <sstream>

#include "common/exception.h"
#include "storage/page/b_plus_tree_internal_page.h"

namespace bustub {
/*****************************************************************************
 * 辅助方法和工具函数
 *****************************************************************************/

/*
 * 创建新的内部页面后的初始化方法
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Init(page_id_t page_id, page_id_t parent_id, int max_size) {
  SetPageType(IndexPageType::INTERNAL_PAGE);  // 设置为内部页面类型
  SetPageId(page_id);                         // 设置页面ID
  SetParentPageId(parent_id);                 // 设置父页面ID
  SetMaxSize(max_size);                       // 设置最大容量
  SetSize(0);                                 // 初始大小为0
}

/*
 * 辅助方法：获取/设置给定"索引"(即数组偏移量)关联的键
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::KeyAt(int index) const -> KeyType { 
  return array_[index].first; 
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::SetKeyAt(int index, const KeyType &key) { 
  array_[index].first = key; 
}

/*
 * 辅助方法：获取给定"索引"(即数组偏移量)关联的 Value (PageId)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueAt(int index) const -> ValueType { 
  return array_[index].second; 
}

/**
 * 辅助方法：设置给定索引处的 Value (PageId)
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::SetValueAt(int index, const ValueType &value) { 
  array_[index].second = value; 
}

/**
 * 查找指向给定 Child PageId 的索引
 * 用于在合并或重分配时找到子节点在父节点中的位置
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueIndex(const ValueType &value) const -> int {
  for (int i = 0; i < GetSize(); ++i) {
    if (array_[i].second == value) {  // 线性搜索匹配的值
      return i;
    }
  }
  return -1;  // 未找到返回-1
}

/**
 * 使用二分查找找到 Key 所在的子节点 PageId。
 * 查找满足 key >= K(i) 的最右边的键，然后返回其对应的值。
 * * 内部节点结构: [Ptr0, Key1, Ptr1, Key2, Ptr2 ...]
 * Ptr0 指向所有 key < Key1 的子树。
 * Ptr1 指向 Key1 <= key < Key2 的子树。
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::Lookup(const KeyType &key, const KeyComparator &comparator) const -> ValueType {
  // 从索引 1 开始搜索，因为 array_[0].first (Key0) 是无效的
  int left = 1;
  int right = GetSize();

  while (left < right) {
    int mid = left + (right - left) / 2;
    if (comparator(key, array_[mid].first) < 0) {  // key < array_[mid].first
      right = mid;  // key 在左边
    } else {  // key >= array_[mid].first
      left = mid + 1;  // key 在右边，继续找更大的 Key
    }
  }

  // left 指向第一个 > key 的位置，或者是数组末尾。
  // 我们需要的是最后一个 <= key 的位置，所以是 left - 1。
  return array_[left - 1].second;
}

/**
 * 填充新根节点 (当根分裂时调用)
 * OldRoot (old_value) 被放到左边
 * NewChild (new_value) 被放到右边，分隔键是 new_key
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::PopulateNewRoot(const ValueType &old_value, const KeyType &new_key,
                                                     const ValueType &new_value) {
  array_[0].second = old_value;  // 第一个子指针指向旧的根 (左子树)
  array_[1].first = new_key;     // 分隔键
  array_[1].second = new_value;  // 第二个子指针指向新的节点 (右子树)
  SetSize(2);                    // 新根有2个子节点
}

/**
 * 在 old_value (Child PageId) 之后插入新的节点指针 new_value 和分隔键 new_key
 * 用于分裂子节点后，在父节点插入新产生的节点指针。
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::InsertNodeAfter(const ValueType &old_value, const KeyType &new_key,
                                                     const ValueType &new_value) -> int {
  int idx = ValueIndex(old_value) + 1;  // 找到插入位置 (旧值的后面)

  // 向右移动元素腾出空间
  for (int i = GetSize(); i > idx; --i) {
    array_[i] = array_[i - 1];
  }

  // 插入新的键值对
  array_[idx].first = new_key;
  array_[idx].second = new_value;
  IncreaseSize(1);  // 大小增加1

  return GetSize();
}

/**
 * 分裂逻辑: 将一半的条目移动到 recipient 页面
 * 注意：移动子节点指针时，必须更新这些子节点的 ParentPageId 指向 recipient
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveHalfTo(BPlusTreeInternalPage *recipient,
                                                BufferPoolManager *buffer_pool_manager) {
  int start_idx = GetSize() / 2;   // 分裂点
  int move_count = GetSize() - start_idx;  // 要移动的元素数量

  // 将后半部分复制到接收者
  recipient->CopyNFrom(array_ + start_idx, move_count, buffer_pool_manager);
  IncreaseSize(-move_count);  // 减少当前页面的大小
}

/**
 * 辅助函数: 从源数组复制 N 个条目
 * 关键点: 每次复制一个子节点指针，都需要 Fetch 该子节点并更新其 SetParentPageId
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyNFrom(MappingType *items, int size, BufferPoolManager *buffer_pool_manager) {
  int start = GetSize();  // 从当前大小的位置开始
  for (int i = 0; i < size; ++i) {
    array_[start + i] = items[i];  // 复制键值对
    
    // 必须更新被移动的子页面的 ParentPageId
    auto *page = buffer_pool_manager->FetchPage(items[i].second);
    if (page != nullptr) {
        auto *child = reinterpret_cast<BPlusTreePage *>(page->GetData());
        child->SetParentPageId(GetPageId());  // 设置父页面ID为当前页面 (Recipient)
        buffer_pool_manager->UnpinPage(items[i].second, true);  // 取消固定并标记为脏页
    }
  }
  IncreaseSize(size);  // 增加大小
}

/**
 * 移除指定索引处的键值对
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Remove(int index) {
  // 向左移动元素填充空隙
  for (int i = index; i < GetSize() - 1; ++i) {
    array_[i] = array_[i + 1];
  }
  IncreaseSize(-1);  // 大小减少1
}

/**
 * 合并逻辑: 将当前页面的所有条目移动到 recipient (通常是左兄弟)
 * @param middle_key: 父节点中用于分隔这两个兄弟的键 (合并时此键会下沉)
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveAllTo(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
                                               BufferPoolManager *buffer_pool_manager) {
  // 内部节点的第一个键通常是无效的。在合并时，从父节点下沉下来的 middle_key 会填补这个位置。
  SetKeyAt(0, middle_key);

  // 将所有条目复制到接收者
  recipient->CopyNFrom(array_, GetSize(), buffer_pool_manager);
  SetSize(0);  // 当前页面清空
}

/**
 * 重分配 (Borrow from Right): 将当前页的第一个条目移动到 recipient (左兄弟) 的末尾
 * @param middle_key: 父节点中的分隔键。借位时，父节点的键会旋转下来，而被借的键会旋转上去。
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveFirstToEndOf(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
                                                      BufferPoolManager *buffer_pool_manager) {
  // 构造要移动的项：键是父节点下来的 middle_key，值是当前节点的第一个指针
  MappingType pair{middle_key, array_[0].second};
  
  // 移动到接收者末尾
  recipient->CopyLastFrom(pair, buffer_pool_manager);

  // 移除第一个元素，后面的补上来
  for (int i = 0; i < GetSize() - 1; ++i) {
    array_[i] = array_[i + 1];
  }
  IncreaseSize(-1);
}

/**
 * 辅助函数: 复制单个元素到末尾，并更新子节点父指针
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyLastFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager) {
  array_[GetSize()] = pair;  // 在末尾添加

  // 更新被移动子节点的父指针
  auto *page = buffer_pool_manager->FetchPage(pair.second);
  auto *child = reinterpret_cast<BPlusTreePage *>(page->GetData());
  child->SetParentPageId(GetPageId());  // 设置父页面ID
  buffer_pool_manager->UnpinPage(pair.second, true);

  IncreaseSize(1);
}

/**
 * 重分配 (Borrow from Left): 将当前页的最后一个条目移动到 recipient (右兄弟) 的前面
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveLastToFrontOf(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
                                                       BufferPoolManager *buffer_pool_manager) {
  // 构造要移动的项：键是父节点下来的 middle_key，值是当前节点的最后一个指针
  MappingType pair{middle_key, array_[GetSize() - 1].second};
  
  // 移动到接收者前面
  recipient->CopyFirstFrom(pair, buffer_pool_manager);
  IncreaseSize(-1);
}

/**
 * 辅助函数: 复制单个元素到前面，并更新子节点父指针
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyFirstFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager) {
  // 向右移动所有元素腾出空间
  for (int i = GetSize(); i > 0; --i) {
    array_[i] = array_[i - 1];
  }
  array_[0] = pair;  // 在前面插入

  // 更新被移动子节点的父指针
  auto *page = buffer_pool_manager->FetchPage(pair.second);
  auto *child = reinterpret_cast<BPlusTreePage *>(page->GetData());
  child->SetParentPageId(GetPageId());
  buffer_pool_manager->UnpinPage(pair.second, true);

  IncreaseSize(1);
}

// 模板实例化
template class BPlusTreeInternalPage<GenericKey<4>, page_id_t, GenericComparator<4>>;
template class BPlusTreeInternalPage<GenericKey<8>, page_id_t, GenericComparator<8>>;
template class BPlusTreeInternalPage<GenericKey<16>, page_id_t, GenericComparator<16>>;
template class BPlusTreeInternalPage<GenericKey<32>, page_id_t, GenericComparator<32>>;
template class BPlusTreeInternalPage<GenericKey<64>, page_id_t, GenericComparator<64>>;
}  // namespace bustub