//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/include/page/b_plus_tree_page.h
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#pragma once

#include <cassert>
#include <climits>
#include <cstdlib>
#include <string>

#include "buffer/buffer_pool_manager.h"
#include "storage/index/generic_key.h"

namespace bustub {

// 定义映射类型，通常是 std::pair<Key, Value>
#define MappingType std::pair<KeyType, ValueType>

// 定义模板参数宏，方便后续使用
#define INDEX_TEMPLATE_ARGUMENTS template <typename KeyType, typename ValueType, typename KeyComparator>

// 定义页面类型枚举：无效页面、叶子页面、内部页面
enum class IndexPageType { INVALID_INDEX_PAGE = 0, LEAF_PAGE, INTERNAL_PAGE };

/**
 * BPlusTreePage 是内部页面 (Internal Page) 和叶子页面 (Leaf Page) 的基类。
 *
 * 它实际上充当了每个 B+ 树页面的头部 (Header)，包含两类页面共享的元数据。
 *
 * Header 格式 (单位: 字节, 总共 24 字节):
 * ----------------------------------------------------------------------------
 * | PageType (4) | LSN (4) | CurrentSize (4) | MaxSize (4) |
 * ----------------------------------------------------------------------------
 * | ParentPageId (4) | PageId(4) |
 * ----------------------------------------------------------------------------
 */
class BPlusTreePage {
 public:
  // 判断当前页面是否为叶子节点
  auto IsLeafPage() const -> bool;
  // 判断当前页面是否为根节点 (父节点 ID 是否无效)
  auto IsRootPage() const -> bool;
  // 设置页面类型 (Leaf 或 Internal)
  void SetPageType(IndexPageType page_type);

  // 获取当前页面中存储的键值对数量
  auto GetSize() const -> int;
  // 设置当前页面大小
  void SetSize(int size);
  // 增加或减少当前页面大小 (amount 可以为负数)
  void IncreaseSize(int amount);

  // 获取该页面允许的最大容量
  auto GetMaxSize() const -> int;
  // 设置最大容量
  void SetMaxSize(int max_size);
  // 获取该页面允许的最小容量 (通常是 MaxSize / 2)
  auto GetMinSize() const -> int;

  // 获取父页面的 PageId
  auto GetParentPageId() const -> page_id_t;
  // 设置父页面的 PageId
  void SetParentPageId(page_id_t parent_page_id);

  // 获取当前页面的 PageId
  auto GetPageId() const -> page_id_t;
  // 设置当前页面的 PageId
  void SetPageId(page_id_t page_id);

  // 设置日志序列号 (用于恢复)
  void SetLSN(lsn_t lsn = INVALID_LSN);

 private:
  // 成员变量：内部节点和叶子节点共享的属性
  // __attribute__((__unused__)) 用于抑制编译器关于未使用变量的警告
  IndexPageType page_type_ __attribute__((__unused__)); // 页面类型
  lsn_t lsn_ __attribute__((__unused__));               // Log Sequence Number
  int size_ __attribute__((__unused__));                // 当前元素数量
  int max_size_ __attribute__((__unused__));            // 最大容量
  page_id_t parent_page_id_ __attribute__((__unused__)); // 父节点页面 ID
  page_id_t page_id_ __attribute__((__unused__));       // 自身页面 ID
};

}  // namespace bustub