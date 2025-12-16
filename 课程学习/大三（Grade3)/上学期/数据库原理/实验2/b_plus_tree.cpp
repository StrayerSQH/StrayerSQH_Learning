#include <string>

#include "common/exception.h"
#include "common/logger.h"
#include "common/rid.h"
#include "storage/index/b_plus_tree.h"
#include "storage/page/header_page.h"

namespace bustub {
INDEX_TEMPLATE_ARGUMENTS
BPLUSTREE_TYPE::BPlusTree(std::string name, BufferPoolManager *buffer_pool_manager, const KeyComparator &comparator,
                          int leaf_max_size, int internal_max_size)
    : index_name_(std::move(name)),
      root_page_id_(INVALID_PAGE_ID),
      buffer_pool_manager_(buffer_pool_manager),
      comparator_(comparator),
      leaf_max_size_(leaf_max_size),
      internal_max_size_(internal_max_size) {}

/*
 * 辅助函数：判断 B+ 树是否为空
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::IsEmpty() const -> bool { return root_page_id_ == INVALID_PAGE_ID; }

/*****************************************************************************
 * 并发控制辅助函数 (CONCURRENT HELPER FUNCTIONS)
 *****************************************************************************/

/*
 * 检查节点对于给定操作是否"安全" (Safe)
 * - INSERT: 节点未满 (size < max_size)。插入不会导致分裂。
 * - DELETE: 节点拥有的键数量 > 最小限制。删除不会导致下溢 (Underflow)。
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
auto BPLUSTREE_TYPE::IsSafe(N *node, Operation op) -> bool {
  if (op == Operation::SEARCH) {
    return true; // 读取总是安全的
  }
  if (op == Operation::INSERT) {
    // 插入安全：插入后不会超过 max_size
    // 注意：代码中使用 max_size - 1 作为阈值，可能需要根据具体 split 实现调整
    if (node->IsLeafPage()) {
      return node->GetSize() < leaf_max_size_ - 1;
    }
    return node->GetSize() < internal_max_size_ - 1;
  }
  // 删除安全：删除后不会小于 MinSize
  if (node->IsRootPage()) {
    // 根节点的特殊情况
    if (node->IsLeafPage()) {
      return node->GetSize() > 1; // 根节点作为叶子，至少得有1个（删完变成0就是空树）
    }
    return node->GetSize() > 2; // 根节点作为内部节点，至少得有2个孩子（删完变成1个孩子需要收缩）
  }
  return node->GetSize() > node->GetMinSize();
}

/*
 * 解锁并取消固定 (Unpin) 事务中记录的所有页面 (用于写操作完成后的清理)
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::UnlockUnpinPages(Transaction *transaction) {
  if (transaction == nullptr) {
    return;
  }
  auto page_set = transaction->GetPageSet();
  for (auto *page : *page_set) {
    if (page == nullptr) {
      // nullptr 标记表示我们持有 root_latch_
      root_latch_.WUnlock();
    } else {
      page->WUnlatch();
      buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
    }
  }
  page_set->clear();

  // 删除被标记为删除的页面 (物理删除)
  auto deleted_page_set = transaction->GetDeletedPageSet();
  for (auto page_id : *deleted_page_set) {
    buffer_pool_manager_->DeletePage(page_id);
  }
  deleted_page_set->clear();
}

/*
 * 仅解锁页面 (用于读操作/Search)
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::UnlockPages(Transaction *transaction) {
  if (transaction == nullptr) {
    return;
  }
  auto page_set = transaction->GetPageSet();
  for (auto *page : *page_set) {
    page->RUnlatch();
    buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
  }
  page_set->clear();
}

/*****************************************************************************
 * SEARCH (查找)
 *****************************************************************************/
/*
 * 查找可能包含 Key 的叶子页面
 * 使用 Latch Crabbing (锁蟹行/锁耦合) 技术进行并发访问
 * @return 指向叶子页面的 Page 指针 (已加锁)
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::FindLeafPage(const KeyType &key, bool leftMost, Operation op, Transaction *transaction) -> Page * {
  // 1. 获取根节点锁
  if (op == Operation::SEARCH) {
    root_latch_.RLock();
  } else {
    root_latch_.WLock();
    if (transaction != nullptr) {
      transaction->AddIntoPageSet(nullptr);  // 标记持有 root 锁
    }
  }

  // 空树检查
  if (IsEmpty()) {
    if (op == Operation::SEARCH) {
      root_latch_.RUnlock();
    } else {
      // 写入操作如果发现空树，通常在外面会调用 StartNewTree，这里解锁返回
      if (transaction != nullptr) {
        transaction->GetPageSet()->pop_back(); 
      }
      root_latch_.WUnlock();
    }
    return nullptr;
  }

  // 2. Fetch 根页面
  auto *page = buffer_pool_manager_->FetchPage(root_page_id_);
  if (page == nullptr) {
    // 异常处理：无法获取页面
    if (op == Operation::SEARCH) {
      root_latch_.RUnlock();
    } else {
      if (transaction != nullptr) transaction->GetPageSet()->pop_back();
      root_latch_.WUnlock();
    }
    return nullptr;
  }
  
  auto *node = reinterpret_cast<BPlusTreePage *>(page->GetData());

  // 3. 对根页面加锁
  if (op == Operation::SEARCH) {
    page->RLatch();
    root_latch_.RUnlock(); // 读操作：拿到子节点锁后，立即释放父节点锁
  } else {
    page->WLatch();
    if (transaction != nullptr) {
      transaction->AddIntoPageSet(page); // 写操作：记录锁住的页面
    }
  }

  // 4. 向下遍历直到叶子节点
  while (!node->IsLeafPage()) {
    auto *internal = reinterpret_cast<InternalPage *>(node);
    page_id_t child_page_id;
    
    // 确定子节点 ID
    if (leftMost) {
      child_page_id = internal->ValueAt(0);
    } else {
      child_page_id = internal->Lookup(key, comparator_);
    }

    auto *child_page = buffer_pool_manager_->FetchPage(child_page_id);
    if (child_page == nullptr) {
      // 异常处理
      if (op == Operation::SEARCH) {
        page->RUnlatch();
        buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
      } else {
        if (transaction != nullptr) UnlockUnpinPages(transaction);
      }
      return nullptr;
    }
    
    auto *child_node = reinterpret_cast<BPlusTreePage *>(child_page->GetData());

    // 加子节点锁
    if (op == Operation::SEARCH) {
      child_page->RLatch();
      // Crabbing: 拿到子锁，释放父锁
      page->RUnlatch();
      buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
    } else {
      child_page->WLatch();
      // 写操作优化：如果子节点是"安全"的(不会发生Split/Coalesce)，则可以释放所有祖先锁
      // 当前实现为了简单（或者是根据作业要求），可能采用了悲观策略，或者在 IsSafe 检查后释放
      if (transaction != nullptr) {
        transaction->AddIntoPageSet(child_page);
      }
      // TODO: 可以在这里添加 IsSafe 判断来提前释放祖先锁
    }

    node = child_node;
    page = child_page;
  }

  // 对于写操作，不要从 page_set 中移除叶子页面，因为调用者需要它
  // 如果这里移除了，UnlockUnpinPages 可能会错误地释放它
  // 注意：某些实现习惯在这里移除并单独返回，取决于 Insert/Remove 怎么处理

  return page;
}

/*
 * 获取 Key 对应的 Value (点查询)
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::GetValue(const KeyType &key, std::vector<ValueType> *result, Transaction *transaction) -> bool {
  auto *page = FindLeafPage(key, false, Operation::SEARCH, transaction);
  if (page == nullptr) {
    return false;
  }

  auto *leaf_page = reinterpret_cast<LeafPage *>(page->GetData());

  ValueType value;
  bool found = leaf_page->Lookup(key, &value, comparator_);

  // 释放叶子页面的读锁 (FindLeafPage 中加的锁)
  page->RUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetPageId(), false);

  if (found) {
    result->push_back(value);
  }
  return found;
}

/*****************************************************************************
 * INSERTION (插入)
 *****************************************************************************/
/*
 * 插入 Key-Value 对。
 * 如果树为空，创建新树。否则插入叶子节点。
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::Insert(const KeyType &key, const ValueType &value, Transaction *transaction) -> bool {
  // 保护 Root ID 的检查
  root_latch_.WLock();
  if (IsEmpty()) {
    StartNewTree(key, value);
    root_latch_.WUnlock();
    return true;
  }
  root_latch_.WUnlock();
  
  // 正常的插入流程
  return InsertIntoLeaf(key, value, transaction);
}

/*
 * 树为空时，初始化第一个节点 (既是根又是叶子)
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::StartNewTree(const KeyType &key, const ValueType &value) {
  page_id_t new_page_id;
  auto *page = buffer_pool_manager_->NewPage(&new_page_id);
  if (page == nullptr) {
    throw Exception(ExceptionType::OUT_OF_MEMORY, "Cannot allocate new page for B+ tree root");
  }

  auto *root = reinterpret_cast<LeafPage *>(page->GetData());
  root->Init(new_page_id, INVALID_PAGE_ID, leaf_max_size_);
  root->Insert(key, value, comparator_);

  root_page_id_ = new_page_id;
  UpdateRootPageId(1); // 1 表示插入记录到 HeaderPage

  buffer_pool_manager_->UnpinPage(new_page_id, true);
}

/*
 * 插入到叶子节点
 * 处理重复键检查和分裂逻辑
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::InsertIntoLeaf(const KeyType &key, const ValueType &value, Transaction *transaction) -> bool {
  auto *page = FindLeafPage(key, false, Operation::INSERT, transaction);
  if (page == nullptr) {
    return false;
  }

  auto *leaf_page = reinterpret_cast<LeafPage *>(page->GetData());

  // 检查重复键
  ValueType existing_value;
  if (leaf_page->Lookup(key, &existing_value, comparator_)) {
    // 发现重复，回滚/清理
    if (transaction != nullptr) {
      UnlockUnpinPages(transaction);
    }
    page->WUnlatch();
    buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
    return false;
  }

  // 插入数据
  int new_size = leaf_page->Insert(key, value, comparator_);

  // 如果插入后满了，需要分裂
  if (new_size >= leaf_max_size_) {
    auto *new_leaf = Split(leaf_page);
    // 向上插入新节点的分隔键 (新节点的第一个 Key)
    KeyType new_key = new_leaf->KeyAt(0);
    InsertIntoParent(leaf_page, new_key, new_leaf, transaction);
    buffer_pool_manager_->UnpinPage(new_leaf->GetPageId(), true);
  }

  // 释放所有锁 (Latch Crabbing 保持的祖先锁)
  if (transaction != nullptr) {
    UnlockUnpinPages(transaction);
  }
  
  // 释放叶子节点的锁 (如果 FindLeafPage 没有把它放入 transaction page_set)
  page->WUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetPageId(), true);
  return true;
}

/*
 * 分裂叶子节点
 * 创建新节点，移动一半数据，更新链表
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::Split(LeafPage *leaf_page) -> LeafPage * {
  page_id_t new_page_id;
  auto *page = buffer_pool_manager_->NewPage(&new_page_id);
  if (page == nullptr) {
    throw Exception(ExceptionType::OUT_OF_MEMORY, "Cannot allocate new leaf page for split");
  }

  auto *new_leaf = reinterpret_cast<LeafPage *>(page->GetData());
  // 初始化新节点，父节点 ID 暂时设为原节点的父节点
  new_leaf->Init(new_page_id, leaf_page->GetParentPageId(), leaf_max_size_);

  // 移动数据
  leaf_page->MoveHalfTo(new_leaf);

  return new_leaf;
}

/*
 * 分裂内部节点
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::Split(InternalPage *internal_page) -> InternalPage * {
  page_id_t new_page_id;
  auto *page = buffer_pool_manager_->NewPage(&new_page_id);
  if (page == nullptr) {
    throw Exception(ExceptionType::OUT_OF_MEMORY, "Cannot allocate new internal page for split");
  }

  auto *new_internal = reinterpret_cast<InternalPage *>(page->GetData());
  new_internal->Init(new_page_id, internal_page->GetParentPageId(), internal_max_size_);

  // 移动数据 (注意：移动内部节点时，buffer_pool_manager 用于更新子节点的父指针)
  internal_page->MoveHalfTo(new_internal, buffer_pool_manager_);

  return new_internal;
}

/*
 * 递归向上插入：将新节点 (new_node) 和分隔键 (key) 插入到父节点中
 * 注意：所有祖先页面应该已经被锁定在 transaction 的 page_set 中
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::InsertIntoParent(BPlusTreePage *old_node, const KeyType &key, BPlusTreePage *new_node,
                                      Transaction *transaction) {
  // 如果 old_node 是根节点，则创建新的根节点
  if (old_node->IsRootPage()) {
    page_id_t new_root_id;
    auto *page = buffer_pool_manager_->NewPage(&new_root_id);
    if (page == nullptr) {
      throw Exception(ExceptionType::OUT_OF_MEMORY, "Cannot allocate new root page");
    }

    auto *new_root = reinterpret_cast<InternalPage *>(page->GetData());
    new_root->Init(new_root_id, INVALID_PAGE_ID, internal_max_size_);
    // 新根节点指向 old_node (左) 和 new_node (右)，以 key 分隔
    new_root->PopulateNewRoot(old_node->GetPageId(), key, new_node->GetPageId());

    // 更新子节点的父指针
    old_node->SetParentPageId(new_root_id);
    new_node->SetParentPageId(new_root_id);

    // 更新树的根 ID
    root_page_id_ = new_root_id;
    UpdateRootPageId(0); // 0 表示 Update 记录

    buffer_pool_manager_->UnpinPage(new_root_id, true);
    return;
  }

  // 正常情况：获取父节点
  page_id_t parent_id = old_node->GetParentPageId();
  auto *parent_page = buffer_pool_manager_->FetchPage(parent_id);
  auto *parent = reinterpret_cast<InternalPage *>(parent_page->GetData());

  new_node->SetParentPageId(parent_id);
  // 在父节点中，在 old_node 指针之后插入 <key, new_node>
  int new_size = parent->InsertNodeAfter(old_node->GetPageId(), key, new_node->GetPageId());

  // 如果父节点也满了，递归分裂
  if (new_size >= internal_max_size_) {
    auto *new_parent = Split(parent);
    // 内部节点分裂时，中间的 Key 会被"推"上去 (Push Up)，而不是像叶子节点那样复制 (Copy Up)
    // 这里 Split 函数内部处理了 Key 的移动逻辑，new_parent 的第一个 Key 就是要推上去的
    KeyType new_key = new_parent->KeyAt(0);
    InsertIntoParent(parent, new_key, new_parent, transaction);
    buffer_pool_manager_->UnpinPage(new_parent->GetPageId(), true);
  }

  // Unpin 父页面 (Fetch 产生的引用)
  buffer_pool_manager_->UnpinPage(parent_id, true);
}

/*****************************************************************************
 * REMOVE (删除)
 *****************************************************************************/
/*
 * 删除 Key
 */
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::Remove(const KeyType &key, Transaction *transaction) {
  auto *page = FindLeafPage(key, false, Operation::DELETE, transaction);
  if (page == nullptr) {
    return;
  }

  auto *leaf_page = reinterpret_cast<LeafPage *>(page->GetData());

  int old_size = leaf_page->GetSize();
  int new_size = leaf_page->RemoveAndDeleteRecord(key, comparator_);

  // Key 不存在
  if (new_size == old_size) {
    if (transaction != nullptr) {
      UnlockUnpinPages(transaction);
    }
    page->WUnlatch();
    buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
    return;
  }

  // 删除后检查是否需要合并 (Coalesce) 或借位 (Redistribute)
  bool should_delete = CoalesceOrRedistribute(leaf_page, transaction);

  if (transaction != nullptr) {
    UnlockUnpinPages(transaction);
  }
  page->WUnlatch();
  buffer_pool_manager_->UnpinPage(page->GetPageId(), true);

  // 如果页面需要被删除 (Coalesce 导致)，在 Unpin 后由 BufferPoolManager 回收
  if (should_delete) {
    buffer_pool_manager_->DeletePage(leaf_page->GetPageId());
  }
}

/*
 * 处理删除后的节点调整
 * @return true 如果当前节点 node 应该被删除 (因为它被合并到了兄弟节点中)
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
auto BPLUSTREE_TYPE::CoalesceOrRedistribute(N *node, Transaction *transaction) -> bool {
  // 如果是根节点
  if (node->IsRootPage()) {
    return AdjustRoot(node);
  }

  // 如果节点大小仍满足最小要求，无需操作
  if (node->GetSize() >= node->GetMinSize()) {
    return false;
  }

  // 获取父节点和兄弟节点
  page_id_t parent_id = node->GetParentPageId();
  auto *parent_page = buffer_pool_manager_->FetchPage(parent_id);
  auto *parent = reinterpret_cast<InternalPage *>(parent_page->GetData());

  // 找到 node 在父节点中的索引
  int index = parent->ValueIndex(node->GetPageId());

  // 策略 1: 尝试从左兄弟 (Left Sibling) 借或合并
  if (index > 0) {
    page_id_t left_sibling_id = parent->ValueAt(index - 1);
    auto *left_sibling_page = buffer_pool_manager_->FetchPage(left_sibling_id);
    auto *left_sibling = reinterpret_cast<N *>(left_sibling_page->GetData());

    // 1.1 如果左兄弟富余，借一个 (Redistribute)
    if (left_sibling->GetSize() > left_sibling->GetMinSize()) {
      Redistribute(left_sibling, node, parent, index, true);
      
      buffer_pool_manager_->UnpinPage(left_sibling_id, true);
      buffer_pool_manager_->UnpinPage(parent_id, true);
      return false; // 借位成功，无需删除 node
    }

    // 1.2 否则，与左兄弟合并 (Coalesce)
    // 将 node 合并进 left_sibling
    bool parent_should_delete = Coalesce(left_sibling, node, parent, index, transaction);
    
    buffer_pool_manager_->UnpinPage(left_sibling_id, true);
    buffer_pool_manager_->UnpinPage(parent_id, true);

    // 如果父节点因为这次合并也变空了，递归删除父节点
    if (parent_should_delete) {
      buffer_pool_manager_->DeletePage(parent_id);
    }
    return true;  // node 被合并了，应该被删除
  }

  // 策略 2: 尝试从右兄弟 (Right Sibling) 借或合并
  if (index < parent->GetSize() - 1) {
    page_id_t right_sibling_id = parent->ValueAt(index + 1);
    auto *right_sibling_page = buffer_pool_manager_->FetchPage(right_sibling_id);
    auto *right_sibling = reinterpret_cast<N *>(right_sibling_page->GetData());

    // 2.1 如果右兄弟富余，借一个
    if (right_sibling->GetSize() > right_sibling->GetMinSize()) {
      Redistribute(right_sibling, node, parent, index, false);
      
      buffer_pool_manager_->UnpinPage(right_sibling_id, true);
      buffer_pool_manager_->UnpinPage(parent_id, true);
      return false;
    }

    // 2.2 否则，与右兄弟合并
    // 将 right_sibling 合并进 node (注意参数顺序：保留 node，删除 right_sibling)
    // 这里的逻辑是将右边的合并到左边来
    bool parent_should_delete = Coalesce(node, right_sibling, parent, index + 1, transaction);

    buffer_pool_manager_->UnpinPage(parent_id, true);

    if (parent_should_delete) {
      buffer_pool_manager_->DeletePage(parent_id);
    }

    // 删除右兄弟页面
    buffer_pool_manager_->UnpinPage(right_sibling_id, true);
    buffer_pool_manager_->DeletePage(right_sibling_id);

    return false;  // node 本身被保留了下来 (吸收了右兄弟)，所以不返回 true
  }

  buffer_pool_manager_->UnpinPage(parent_id, false);
  return false;
}

/*
 * 根节点调整 (当根节点可能为空或变为单子节点时)
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::AdjustRoot(BPlusTreePage *old_root_node) -> bool {
  // 情况 1: 根是叶子且被删空了 -> 树变空
  if (old_root_node->IsLeafPage() && old_root_node->GetSize() == 0) {
    root_page_id_ = INVALID_PAGE_ID;
    UpdateRootPageId(0);
    return true; // 删除旧根
  }

  // 情况 2: 根是内部节点且只剩 1 个孩子 -> 孩子成为新根 (树高度 -1)
  if (!old_root_node->IsLeafPage() && old_root_node->GetSize() == 1) {
    auto *old_root = reinterpret_cast<InternalPage *>(old_root_node);
    page_id_t new_root_id = old_root->ValueAt(0); // 唯一的孩子

    auto *new_root_page = buffer_pool_manager_->FetchPage(new_root_id);
    auto *new_root = reinterpret_cast<BPlusTreePage *>(new_root_page->GetData());
    
    // 成为新根，没有父节点
    new_root->SetParentPageId(INVALID_PAGE_ID);

    root_page_id_ = new_root_id;
    UpdateRootPageId(0);

    buffer_pool_manager_->UnpinPage(new_root_id, true);
    return true; // 删除旧根
  }

  return false;
}

/*
 * 重分配 (Redistribute/Borrow)
 * 从邻居借一个键值对到当前节点，并更新父节点的分隔键
 * @param from_left: true 表示从左兄弟借，false 表示从右兄弟借
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
void BPLUSTREE_TYPE::Redistribute(N *neighbor_node, N *node, InternalPage *parent, int index, bool from_left) {
  if (node->IsLeafPage()) {
    auto *leaf_node = reinterpret_cast<LeafPage *>(node);
    auto *neighbor_leaf = reinterpret_cast<LeafPage *>(neighbor_node);

    if (from_left) {
      // 左兄弟的最右元素 -> 移动到 -> 当前节点的最左
      neighbor_leaf->MoveLastToFrontOf(leaf_node);
      // 更新父节点索引：当前节点的第一个 Key 变了
      parent->SetKeyAt(index, leaf_node->KeyAt(0));
    } else {
      // 右兄弟的最左元素 -> 移动到 -> 当前节点的最右
      neighbor_leaf->MoveFirstToEndOf(leaf_node);
      // 更新父节点索引：右兄弟的第一个 Key 变了 (index+1 指向右兄弟)
      parent->SetKeyAt(index + 1, neighbor_leaf->KeyAt(0));
    }
  } else {
    auto *internal_node = reinterpret_cast<InternalPage *>(node);
    auto *neighbor_internal = reinterpret_cast<InternalPage *>(neighbor_node);

    if (from_left) {
      // 内部节点借位涉及 Key 的旋转
      KeyType middle_key = parent->KeyAt(index);
      // 将 middle_key 下沉，并把左兄弟的一个子节点移过来
      neighbor_internal->MoveLastToFrontOf(internal_node, middle_key, buffer_pool_manager_);
      // 左兄弟移动后产生的新 Key 上升到父节点
      parent->SetKeyAt(index, internal_node->KeyAt(0));
    } else {
      KeyType middle_key = parent->KeyAt(index + 1);
      neighbor_internal->MoveFirstToEndOf(internal_node, middle_key, buffer_pool_manager_);
      parent->SetKeyAt(index + 1, neighbor_internal->KeyAt(0));
    }
  }
}

/*
 * 合并 (Coalesce/Merge)
 * 将右节点 (neighbor_node) 的所有内容移动到左节点 (node)
 * 注意：这里的参数命名可能有些混淆，通常逻辑是把 right 合并到 left
 */
INDEX_TEMPLATE_ARGUMENTS
template <typename N>
auto BPLUSTREE_TYPE::Coalesce(N *neighbor_node, N *node, InternalPage *parent, int index, Transaction *transaction)
    -> bool {
  // 获取父节点中对应的分隔键
  // 假设 node 在 index, neighbor_node 在 index-1 (左兄弟)，那么分隔键在 index
  KeyType middle_key = parent->KeyAt(index);

  if (node->IsLeafPage()) {
    auto *leaf_node = reinterpret_cast<LeafPage *>(node);
    auto *neighbor_leaf = reinterpret_cast<LeafPage *>(neighbor_node);
    // 将当前节点(右)的数据全部移动到邻居(左)
    leaf_node->MoveAllTo(neighbor_leaf);
  } else {
    auto *internal_node = reinterpret_cast<InternalPage *>(node);
    auto *neighbor_internal = reinterpret_cast<InternalPage *>(neighbor_node);
    // 内部节点合并，需要把父节点的 middle_key 拉下来放到合并后的节点中间
    internal_node->MoveAllTo(neighbor_internal, middle_key, buffer_pool_manager_);
  }

  // 从父节点中移除指向当前节点的指针 (因为已经合并到邻居了)
  parent->Remove(index);

  // 递归：父节点少了一个孩子，检查是否需要调整父节点
  return CoalesceOrRedistribute(parent, transaction);
}

/*****************************************************************************
 * INDEX ITERATOR (迭代器)
 *****************************************************************************/
/*
 * 创建指向最左叶子节点第一个元素的迭代器 (Begin)
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::Begin() -> INDEXITERATOR_TYPE {
  root_latch_.RLock();
  if (IsEmpty()) {
    root_latch_.RUnlock();
    return INDEXITERATOR_TYPE(nullptr, 0, buffer_pool_manager_);
  }

  // 找到最左叶子
  auto *page = buffer_pool_manager_->FetchPage(root_page_id_);
  auto *node = reinterpret_cast<BPlusTreePage *>(page->GetData());
  page->RLatch();
  root_latch_.RUnlock();

  while (!node->IsLeafPage()) {
    auto *internal = reinterpret_cast<InternalPage *>(node);
    page_id_t child_page_id = internal->ValueAt(0); // 最左孩子
    auto *child_page = buffer_pool_manager_->FetchPage(child_page_id);
    
    child_page->RLatch();
    page->RUnlatch();
    buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
    
    page = child_page;
    node = reinterpret_cast<BPlusTreePage *>(page->GetData());
  }

  // 返回迭代器，注意：迭代器持有页面的 Read Latch 和 Pin
  // 这里 RUnlatch 可能是因为迭代器实现不持有锁，只持有 Pin？
  // 根据 IndexIterator 代码，它没有显式管理锁，所以这里应该 Unlatch
  page->RUnlatch();
  return INDEXITERATOR_TYPE(reinterpret_cast<LeafPage *>(node), 0, buffer_pool_manager_);
}

/*
 * 创建指向特定 Key 的迭代器
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::Begin(const KeyType &key) -> INDEXITERATOR_TYPE {
  root_latch_.RLock();
  if (IsEmpty()) {
    root_latch_.RUnlock();
    return INDEXITERATOR_TYPE(nullptr, 0, buffer_pool_manager_);
  }

  // 使用 Search 逻辑找到叶子 (注意：这里手动实现了一遍 Search 逻辑，没有复用 FindLeafPage，可能是为了控制锁的粒度)
  auto *page = buffer_pool_manager_->FetchPage(root_page_id_);
  auto *node = reinterpret_cast<BPlusTreePage *>(page->GetData());
  page->RLatch();
  root_latch_.RUnlock();

  while (!node->IsLeafPage()) {
    auto *internal = reinterpret_cast<InternalPage *>(node);
    page_id_t child_page_id = internal->Lookup(key, comparator_);
    auto *child_page = buffer_pool_manager_->FetchPage(child_page_id);
    
    child_page->RLatch();
    page->RUnlatch();
    buffer_pool_manager_->UnpinPage(page->GetPageId(), false);
    
    page = child_page;
    node = reinterpret_cast<BPlusTreePage *>(page->GetData());
  }

  auto *leaf_page = reinterpret_cast<LeafPage *>(node);
  int index = leaf_page->KeyIndex(key, comparator_);
  
  page->RUnlatch();
  return INDEXITERATOR_TYPE(leaf_page, index, buffer_pool_manager_);
}

/*
 * 创建 End 迭代器 (空)
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::End() -> INDEXITERATOR_TYPE { return INDEXITERATOR_TYPE(nullptr, 0, buffer_pool_manager_); }

/**
 * 获取树根 ID
 */
INDEX_TEMPLATE_ARGUMENTS
auto BPLUSTREE_TYPE::GetRootPageId() -> page_id_t { return root_page_id_; }

/*****************************************************************************
 * 调试与工具函数
 *****************************************************************************/
// (省略了部分非核心的 Debug 函数注释)
INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_TYPE::UpdateRootPageId(int insert_record) {
  auto *header_page = static_cast<HeaderPage *>(buffer_pool_manager_->FetchPage(HEADER_PAGE_ID));
  if (insert_record != 0) {
    header_page->InsertRecord(index_name_, root_page_id_);
  } else {
    header_page->UpdateRecord(index_name_, root_page_id_);
  }
  buffer_pool_manager_->UnpinPage(HEADER_PAGE_ID, true);
}

// 模板实例化
template class BPlusTree<GenericKey<4>, RID, GenericComparator<4>>;
template class BPlusTree<GenericKey<8>, RID, GenericComparator<8>>;
template class BPlusTree<GenericKey<16>, RID, GenericComparator<16>>;
template class BPlusTree<GenericKey<32>, RID, GenericComparator<32>>;
template class BPlusTree<GenericKey<64>, RID, GenericComparator<64>>;

}  // namespace bustub