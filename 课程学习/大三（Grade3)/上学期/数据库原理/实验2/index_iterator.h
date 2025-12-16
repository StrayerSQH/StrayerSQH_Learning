//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/include/index/index_iterator.h
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
/**
 * index_iterator.h
 * 用于 B+ 树的范围扫描 (Range Scan)
 */
#pragma once
#include "buffer/buffer_pool_manager.h"
#include "storage/page/b_plus_tree_leaf_page.h"

namespace bustub {

#define INDEXITERATOR_TYPE IndexIterator<KeyType, ValueType, KeyComparator>

INDEX_TEMPLATE_ARGUMENTS
class IndexIterator {
  using LeafPage = BPlusTreeLeafPage<KeyType, ValueType, KeyComparator>;

 public:
  // 构造函数
  IndexIterator();
  IndexIterator(LeafPage *leaf, int index, BufferPoolManager *buffer_pool_manager);
  ~IndexIterator();  // NOLINT

  // 禁用拷贝，防止 Double Unpin (同一页面被多次取消固定)
  IndexIterator(const IndexIterator &) = delete;
  auto operator=(const IndexIterator &) -> IndexIterator & = delete;

  // 启用移动语义 (Move Constructor/Assignment)
  IndexIterator(IndexIterator &&other) noexcept;
  auto operator=(IndexIterator &&other) noexcept -> IndexIterator &;

  // 检查是否到达末尾
  auto IsEnd() -> bool;

  // 解引用操作符，返回当前指向的键值对
  auto operator*() -> const MappingType &;

  // 前缀自增操作符 (++iter)，移动到下一个元素
  auto operator++() -> IndexIterator &;

  // 相等/不等比较
  auto operator==(const IndexIterator &itr) const -> bool;
  auto operator!=(const IndexIterator &itr) const -> bool;

 private:
  page_id_t page_id_;   // 当前叶子页面的 ID
  LeafPage *leaf_;      // 当前叶子页面指针
  int index_;           // 在当前页面中的索引
  BufferPoolManager *buffer_pool_manager_; // BPM，用于 Fetch/Unpin 页面
};

}  // namespace bustub