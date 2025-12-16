/**
 * index_iterator.cpp
 * 索引迭代器实现文件
 */
#include <cassert>

#include "storage/index/index_iterator.h"

namespace bustub {

// 默认构造函数 - 创建一个空的(无效/结束)迭代器
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE::IndexIterator()
    : page_id_(INVALID_PAGE_ID),      // 无效的页面ID
      leaf_(nullptr),                  // 空指针表示没有叶子节点
      index_(0),                       // 索引位置为0
      buffer_pool_manager_(nullptr) {} // 没有缓冲区管理器

// 带参数构造函数 - 创建指向特定位置的迭代器
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE::IndexIterator(LeafPage *leaf, int index, BufferPoolManager *buffer_pool_manager)
    : page_id_(leaf != nullptr ? leaf->GetPageId() : INVALID_PAGE_ID),  // 获取叶子页面的ID
      leaf_(leaf),                                                      // 指向叶子页面
      index_(index),                                                    // 在叶子页面中的索引位置
      buffer_pool_manager_(buffer_pool_manager) {}                      // 缓冲区管理器引用

// 移动构造函数 - 转移资源所有权
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE::IndexIterator(IndexIterator &&other) noexcept
    : page_id_(other.page_id_),                         
      leaf_(other.leaf_),                               
      index_(other.index_),                             
      buffer_pool_manager_(other.buffer_pool_manager_)  
{
  // 取得所有权后，将源对象置空，防止析构时 Unpin
  other.page_id_ = INVALID_PAGE_ID;
  other.leaf_ = nullptr;
  other.index_ = 0;
  other.buffer_pool_manager_ = nullptr;
}

// 移动赋值运算符
INDEX_TEMPLATE_ARGUMENTS
auto INDEXITERATOR_TYPE::operator=(IndexIterator &&other) noexcept -> IndexIterator & {
  if (this != &other) {
    // 如果当前持有页面，先释放
    if (leaf_ != nullptr && buffer_pool_manager_ != nullptr) {
      buffer_pool_manager_->UnpinPage(page_id_, false);
    }
    
    // 转移所有权
    page_id_ = other.page_id_;
    leaf_ = other.leaf_;
    index_ = other.index_;
    buffer_pool_manager_ = other.buffer_pool_manager_;
    
    // 置空源对象
    other.page_id_ = INVALID_PAGE_ID;
    other.leaf_ = nullptr;
    other.index_ = 0;
    other.buffer_pool_manager_ = nullptr;
  }
  return *this;
}

// 析构函数 - 确保 Unpin 页面
INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE::~IndexIterator() {
  if (leaf_ != nullptr && buffer_pool_manager_ != nullptr) {
    buffer_pool_manager_->UnpinPage(page_id_, false);  // 取消固定 (只读，非脏)
  }
}

// 检查是否结束
INDEX_TEMPLATE_ARGUMENTS
auto INDEXITERATOR_TYPE::IsEnd() -> bool { 
  return leaf_ == nullptr; 
}

// 解引用
INDEX_TEMPLATE_ARGUMENTS
auto INDEXITERATOR_TYPE::operator*() -> const MappingType & {
  assert(leaf_ != nullptr);
  return leaf_->GetItem(index_);
}

// 前缀自增 (++iter)
INDEX_TEMPLATE_ARGUMENTS
auto INDEXITERATOR_TYPE::operator++() -> INDEXITERATOR_TYPE & {
  index_++;  // 移动到下一个 slot
  
  // 检查是否越过了当前页面的边界
  if (index_ >= leaf_->GetSize()) {
    page_id_t next_page_id = leaf_->GetNextPageId();

    // 释放当前页面
    buffer_pool_manager_->UnpinPage(page_id_, false);

    if (next_page_id == INVALID_PAGE_ID) {
      // 后面没有页面了 -> 迭代结束
      leaf_ = nullptr;
      page_id_ = INVALID_PAGE_ID;
      index_ = 0;
    } else {
      // 跨越到下一个叶子页面
      auto *next_page = buffer_pool_manager_->FetchPage(next_page_id);
      leaf_ = reinterpret_cast<LeafPage *>(next_page->GetData());
      page_id_ = next_page_id;
      index_ = 0; // 新页面的第一个元素
    }
  }
  return *this;
}

// 相等比较
INDEX_TEMPLATE_ARGUMENTS
auto INDEXITERATOR_TYPE::operator==(const IndexIterator &itr) const -> bool {
  return page_id_ == itr.page_id_ && index_ == itr.index_;
}

// 不等比较
INDEX_TEMPLATE_ARGUMENTS
auto INDEXITERATOR_TYPE::operator!=(const IndexIterator &itr) const -> bool { 
  return !(*this == itr);
}

// 模板实例化
template class IndexIterator<GenericKey<4>, RID, GenericComparator<4>>;
template class IndexIterator<GenericKey<8>, RID, GenericComparator<8>>;
template class IndexIterator<GenericKey<16>, RID, GenericComparator<16>>;
template class IndexIterator<GenericKey<32>, RID, GenericComparator<32>>;
template class IndexIterator<GenericKey<64>, RID, GenericComparator<64>>;

}  // namespace bustub