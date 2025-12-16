//===----------------------------------------------------------------------===//
//
//                         CMU-DB 项目 (15-445/645)
//                         ***请勿公开分享***
//
// 文件标识: src/page/b_plus_tree_leaf_page.cpp
//
// 版权所有 (c) 2018, 卡内基梅隆大学数据库组
//
//===----------------------------------------------------------------------===//

#include <sstream>

#include "common/exception.h"
#include "common/rid.h"
#include "storage/page/b_plus_tree_leaf_page.h"

namespace bustub {

/*****************************************************************************
 * 辅助方法和工具函数
 *****************************************************************************/

/**
 * 创建新的叶子页面后的初始化方法
 * 包括设置页面类型、当前大小为零、设置页面ID/父页面ID、
 * 设置下一个页面ID和最大大小
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::Init(page_id_t page_id, page_id_t parent_id, int max_size) {
  SetPageType(IndexPageType::LEAF_PAGE);  // 设置为叶子页面类型
  SetPageId(page_id);                     // 设置页面ID
  SetParentPageId(parent_id);             // 设置父页面ID
  SetMaxSize(max_size);                   // 设置最大容量
  SetSize(0);                             // 初始大小为0
  next_page_id_ = INVALID_PAGE_ID;        // 下一个页面ID初始化为无效
}

/**
 * 辅助方法：设置/获取下一个页面ID (维护叶子节点的链表结构)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::GetNextPageId() const -> page_id_t { 
  return next_page_id_; 
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::SetNextPageId(page_id_t next_page_id) { 
  next_page_id_ = next_page_id; 
}

/*
 * 辅助方法：查找并返回与输入"索引"(即数组偏移量)关联的键
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::KeyAt(int index) const -> KeyType { 
  return array_[index].first; 
}

/**
 * 辅助方法：获取给定索引处的值 (RID)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::ValueAt(int index) const -> ValueType { 
  return array_[index].second; 
}

/**
 * 辅助方法：设置给定索引处的键
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::SetKeyAt(int index, const KeyType &key) { 
  array_[index].first = key; 
}

/**
 * 辅助方法：设置给定索引处的值
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::SetValueAt(int index, const ValueType &value) { 
  array_[index].second = value; 
}

/**
 * 获取索引处的完整项 (Key + Value)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::GetItem(int index) -> const MappingType & { 
  return array_[index]; 
}

/**
 * 二分查找: 找到第一个 Key >= 给定 Key 的索引
 * 用于确定插入位置或查找键的位置
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::KeyIndex(const KeyType &key, const KeyComparator &comparator) const -> int {
  int left = 0;
  int right = GetSize();
  while (left < right) {
    int mid = left + (right - left) / 2;  // 防止溢出的中间点计算
    if (comparator(array_[mid].first, key) < 0) {  // 中间键 < 查找键
      left = mid + 1;  // 向右搜索
    } else {
      right = mid;     // 向左搜索
    }
  }
  return left;  // 返回应该插入或找到键的位置
}

/**
 * 查找：找到给定键对应的值
 * @return 如果找到返回true，否则返回false
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::Lookup(const KeyType &key, ValueType *value, const KeyComparator &comparator) const
    -> bool {
  int idx = KeyIndex(key, comparator);  // 查找键的位置
  if (idx < GetSize() && comparator(array_[idx].first, key) == 0) {  // 找到完全匹配的键
    *value = array_[idx].second;  // 设置返回值
    return true;
  }
  return false;  // 未找到
}

/**
 * 按排序顺序将键值对插入叶子节点
 * @return 插入后的大小
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::Insert(const KeyType &key, const ValueType &value, const KeyComparator &comparator)
    -> int {
  int idx = KeyIndex(key, comparator);  // 查找插入位置

  // 检查重复键 (B+树通常不支持重复键，除非特殊处理)
  if (idx < GetSize() && comparator(array_[idx].first, key) == 0) {
    return GetSize();  // 重复键，不插入
  }

  // 向右移动元素腾出空间 (memmove 逻辑)
  for (int i = GetSize(); i > idx; --i) {
    array_[i] = array_[i - 1];
  }

  // 插入新的键值对
  array_[idx].first = key;
  array_[idx].second = value;
  IncreaseSize(1);  // 大小增加1

  return GetSize();
}

/**
 * 从叶子节点中删除键
 * @return 删除后的大小
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_LEAF_PAGE_TYPE::RemoveAndDeleteRecord(const KeyType &key, const KeyComparator &comparator) -> int {
  int idx = KeyIndex(key, comparator);  // 查找键的位置

  // 键未找到
  if (idx >= GetSize() || comparator(array_[idx].first, key) != 0) {
    return GetSize();  // 返回当前大小（未改变）
  }

  // 向左移动元素填充空隙 (覆盖删除)
  for (int i = idx; i < GetSize() - 1; ++i) {
    array_[i] = array_[i + 1];
  }
  IncreaseSize(-1);  // 大小减少1

  return GetSize();
}

/**
 * 分裂逻辑: 将一半的项移动到 recipient 页面
 * 从索引 GetSize()/2 开始复制到接收者
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveHalfTo(BPlusTreeLeafPage *recipient) {
  int start_idx = GetSize() / 2;   // 分裂点
  int move_count = GetSize() - start_idx;  // 要移动的元素数量

  // 复制后半部分到接收者 (新页)
  recipient->CopyNFrom(array_ + start_idx, move_count);

  // 更新链表指针：当前页 -> 新页 -> 原下一页
  recipient->SetNextPageId(GetNextPageId());  // 接收者的下一个页面指向原下一个页面
  SetNextPageId(recipient->GetPageId());      // 当前页面的下一个页面指向接收者

  IncreaseSize(-move_count);  // 当前页面大小减少
}

/**
 * 从数组复制 N 个元素到当前叶子页面的末尾
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::CopyNFrom(MappingType *items, int size) {
  int start = GetSize();  // 从当前大小的位置开始
  for (int i = 0; i < size; ++i) {
    array_[start + i] = items[i];  // 复制键值对
  }
  IncreaseSize(size);  // 增加大小
}

/**
 * 合并逻辑: 将所有项移动到 recipient 页面 (通常是当前页的左兄弟)
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveAllTo(BPlusTreeLeafPage *recipient) {
  recipient->CopyNFrom(array_, GetSize());    // 将自己的所有数据复制给 recipient
  recipient->SetNextPageId(GetNextPageId());  // 维护链表
  SetSize(0);  // 清空当前页面 (之后会被删除)
}

/**
 * 重分配逻辑 (Borrow from Right): 将当前页面的第一个元素移动到 recipient (左兄弟) 的末尾
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveFirstToEndOf(BPlusTreeLeafPage *recipient) {
  recipient->CopyLastFrom(array_[0]);  // 复制第一个项到接收者末尾

  // 向左移动剩余元素
  for (int i = 0; i < GetSize() - 1; ++i) {
    array_[i] = array_[i + 1];
  }
  IncreaseSize(-1);  // 大小减少1
}

/**
 * 将单个项复制到这个叶子页面的末尾
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::CopyLastFrom(const MappingType &item) {
  array_[GetSize()] = item;  // 在末尾添加
  IncreaseSize(1);           // 大小增加1
}

/**
 * 重分配逻辑 (Borrow from Left): 将当前页面的最后一个元素移动到 recipient (右兄弟) 的前面
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::MoveLastToFrontOf(BPlusTreeLeafPage *recipient) {
  recipient->CopyFirstFrom(array_[GetSize() - 1]);  // 复制最后一个项到接收者前面
  IncreaseSize(-1);  // 大小减少1
}

/**
 * 将单个项复制到这个叶子页面的前面（移动现有元素）
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_LEAF_PAGE_TYPE::CopyFirstFrom(const MappingType &item) {
  // 向右移动所有元素腾出空间
  for (int i = GetSize(); i > 0; --i) {
    array_[i] = array_[i - 1];
  }
  array_[0] = item;  // 在前面插入
  IncreaseSize(1);   // 大小增加1
}

// 模板实例化 - 支持不同键大小的B+树
template class BPlusTreeLeafPage<GenericKey<4>, RID, GenericComparator<4>>;
template class BPlusTreeLeafPage<GenericKey<8>, RID, GenericComparator<8>>;
template class BPlusTreeLeafPage<GenericKey<16>, RID, GenericComparator<16>>;
template class BPlusTreeLeafPage<GenericKey<32>, RID, GenericComparator<32>>;
template class BPlusTreeLeafPage<GenericKey<64>, RID, GenericComparator<64>>;
}  // namespace bustub