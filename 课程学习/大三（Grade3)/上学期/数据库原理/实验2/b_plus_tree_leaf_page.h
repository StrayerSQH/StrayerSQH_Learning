//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/include/page/b_plus_tree_leaf_page.h
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#pragma once

#include <utility>
#include <vector>

#include "storage/page/b_plus_tree_page.h"

namespace bustub {

#define B_PLUS_TREE_LEAF_PAGE_TYPE BPlusTreeLeafPage<KeyType, ValueType, KeyComparator>
#define LEAF_PAGE_HEADER_SIZE 28
#define LEAF_PAGE_SIZE ((BUSTUB_PAGE_SIZE - LEAF_PAGE_HEADER_SIZE) / sizeof(MappingType))

/**
 * 存储索引键和记录 ID (Record ID)。
 * Record ID = Page ID + Slot ID (详见 include/common/rid.h)。
 * 仅支持唯一键。
 *
 * 叶子页面格式 (键是按顺序存储的):
 * ----------------------------------------------------------------------
 * | HEADER | KEY(1) + RID(1) | KEY(2) + RID(2) | ... | KEY(n) + RID(n)
 * ----------------------------------------------------------------------
 *
 * Header 格式 (单位: 字节, 总共 28 字节):
 * ---------------------------------------------------------------------
 * | PageType (4) | LSN (4) | CurrentSize (4) | MaxSize (4) |
 * ---------------------------------------------------------------------
 * -----------------------------------------------
 * | ParentPageId (4) | PageId (4) | NextPageId (4)
 * -----------------------------------------------
 */
INDEX_TEMPLATE_ARGUMENTS
class BPlusTreeLeafPage : public BPlusTreePage {
 public:
  // 从缓冲池创建新叶子页面后，必须调用 Init 方法设置默认值
  void Init(page_id_t page_id, page_id_t parent_id = INVALID_PAGE_ID, int max_size = LEAF_PAGE_SIZE);
  
  // 辅助方法：获取/设置下一个叶子页面的 ID (用于范围扫描)
  auto GetNextPageId() const -> page_id_t;
  void SetNextPageId(page_id_t next_page_id);
  
  // 获取指定索引处的 Key
  auto KeyAt(int index) const -> KeyType;

  // B+ 树操作的额外辅助方法
  auto ValueAt(int index) const -> ValueType;
  void SetKeyAt(int index, const KeyType &key);
  void SetValueAt(int index, const ValueType &value);

  // 查找: 使用二分查找找到第一个 >= 给定 key 的索引
  auto KeyIndex(const KeyType &key, const KeyComparator &comparator) const -> int;

  // 查找: 找到给定 key 对应的 value，如果找到返回 true
  auto Lookup(const KeyType &key, ValueType *value, const KeyComparator &comparator) const -> bool;

  // 插入: 将键值对插入叶子节点，返回插入后的大小
  auto Insert(const KeyType &key, const ValueType &value, const KeyComparator &comparator) -> int;

  // 分裂: 将一半的元素移动到 recipient 页面 (新分裂出的节点)
  void MoveHalfTo(BPlusTreeLeafPage *recipient);

  // 合并: 将所有元素移动到 recipient 页面 (通常是左兄弟)
  void MoveAllTo(BPlusTreeLeafPage *recipient);

  // 重分配 (从右兄弟借): 将第一个元素移动到 recipient 页面的末尾
  void MoveFirstToEndOf(BPlusTreeLeafPage *recipient);

  // 重分配 (从左兄弟借): 将最后一个元素移动到 recipient 页面的前面
  void MoveLastToFrontOf(BPlusTreeLeafPage *recipient);

  // 删除: 从叶子节点中移除 key
  auto RemoveAndDeleteRecord(const KeyType &key, const KeyComparator &comparator) -> int;

  // 获取指定索引处的完整项 (Key + Value)
  auto GetItem(int index) -> const MappingType &;

 private:
  // 辅助函数：从数组复制 N 个元素
  void CopyNFrom(MappingType *items, int size);
  void CopyLastFrom(const MappingType &item);
  void CopyFirstFrom(const MappingType &item);

  // 下一个叶子页面的 ID (链表结构)
  page_id_t next_page_id_;
  // 页面数据的弹性数组成员 (实际存储 Key-Value 对的地方)
  MappingType array_[1];
};
}  // namespace bustub