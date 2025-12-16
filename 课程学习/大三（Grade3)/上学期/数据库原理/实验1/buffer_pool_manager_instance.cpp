//===----------------------------------------------------------------------===//
//
//                          BusTub
//
// buffer_pool_manager_instance.cpp
//
// Identification: src/buffer/buffer_pool_manager.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/buffer_pool_manager_instance.h"

#include "common/exception.h"
#include "common/macros.h"

namespace bustub {

BufferPoolManagerInstance::BufferPoolManagerInstance(size_t pool_size, DiskManager *disk_manager, size_t replacer_k,
                                                     LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager), log_manager_(log_manager) {
  // 为缓冲池分配连续的内存空间
  pages_ = new Page[pool_size_];
  // 创建页表（用于映射page_id到frame_id）
  page_table_ = new ExtendibleHashTable<page_id_t, frame_id_t>(bucket_size_);
  // 创建LRU-K替换器
  replacer_ = new LRUKReplacer(pool_size, replacer_k);

  // 初始时，所有页帧都在空闲列表中
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_.emplace_back(static_cast<int>(i));
  }

  // ** 移除了 `throw NotImplementedException` **
}

BufferPoolManagerInstance::~BufferPoolManagerInstance() {
  delete[] pages_;
  delete page_table_;
  delete replacer_;
}

// 创建新页面
auto BufferPoolManagerInstance::NewPgImp(page_id_t *page_id) -> Page * {
  std::scoped_lock<std::mutex> lock(latch_);  // 获取锁保证线程安全

  frame_id_t frame_id;
  bool found_frame = false;

  // 1. 尝试从空闲列表获取可用帧
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
    found_frame = true;
  } else {
    // 2. 如果没有空闲帧，尝试从替换器中驱逐一个页面
    if (replacer_->Evict(&frame_id)) {
      found_frame = true;
      page_id_t old_page_id = pages_[frame_id].GetPageId();

      // 2a. 如果被驱逐页是脏页，需要先写回磁盘
      if (pages_[frame_id].IsDirty()) {
        disk_manager_->WritePage(old_page_id, pages_[frame_id].GetData());
        pages_[frame_id].is_dirty_ = false;
      }
      // 2b. 从页表中移除旧页面的映射
      page_table_->Remove(old_page_id);
    }
  }

  // 3. 如果没有找到可用帧（所有帧都被pin住），返回nullptr
  if (!found_frame) {
    return nullptr;
  }

  // 4. 分配新的页面ID
  *page_id = AllocatePage();

  // 5. 更新元数据和Page对象
  page_table_->Insert(*page_id, frame_id);           // 在页表中建立新映射
  replacer_->RecordAccess(frame_id);                 // 记录访问历史
  replacer_->SetEvictable(frame_id, false);          // 新获取的页面默认不可驱逐

  pages_[frame_id].ResetMemory();                    // 重置内存数据
  pages_[frame_id].page_id_ = *page_id;              // 设置页面ID（**修正：直接访问成员变量**）
  pages_[frame_id].pin_count_ = 1;                   // Pin计数设为1（表示正在使用）
  pages_[frame_id].is_dirty_ = false;                // 新页面是干净的

  return &pages_[frame_id];
}

// 获取指定页面（如果不在缓冲池则从磁盘读取）
auto BufferPoolManagerInstance::FetchPgImp(page_id_t page_id) -> Page * {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;

  // 1. 尝试在页表中查找页面是否已在缓冲池中
  if (page_table_->Find(page_id, frame_id)) {
    // 页面已在缓冲池中
    pages_[frame_id].pin_count_++;                   // 增加pin计数
    replacer_->RecordAccess(frame_id);               // 记录访问历史
    replacer_->SetEvictable(frame_id, false);        // 被pin住，不可驱逐
    return &pages_[frame_id];
  }

  // 2. 页面不在缓冲池中，需要获取一个帧来加载
  bool found_frame = false;
  // 2a. 尝试从空闲列表获取
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
    found_frame = true;
  } else {
    // 2b. 尝试从替换器驱逐
    if (replacer_->Evict(&frame_id)) {
      found_frame = true;
      page_id_t old_page_id = pages_[frame_id].GetPageId();

      // 如果被驱逐页是脏页，先写回磁盘
      if (pages_[frame_id].IsDirty()) {
        disk_manager_->WritePage(old_page_id, pages_[frame_id].GetData());
        pages_[frame_id].is_dirty_ = false;
      }
      // 从页表移除旧映射
      page_table_->Remove(old_page_id);
    }
  }

  // 3. 如果没有可用帧（所有帧都被pin），返回nullptr
  if (!found_frame) {
    return nullptr;
  }

  // 4. 从磁盘读取页面数据到帧中
  disk_manager_->ReadPage(page_id, pages_[frame_id].GetData());

  // 5. 更新元数据和Page对象
  page_table_->Insert(page_id, frame_id);           // 建立新映射
  replacer_->RecordAccess(frame_id);                // 记录访问
  replacer_->SetEvictable(frame_id, false);         // 不可驱逐

  pages_[frame_id].page_id_ = page_id;              // 设置页面ID（**修正：直接访问成员变量**）
  pages_[frame_id].pin_count_ = 1;                  // Pin计数设为1
  pages_[frame_id].is_dirty_ = false;               // 从磁盘读取的是干净页

  return &pages_[frame_id];
}

// 取消pin一个页面
auto BufferPoolManagerInstance::UnpinPgImp(page_id_t page_id, bool is_dirty) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;
  // 检查页面是否在缓冲池中
  if (!page_table_->Find(page_id, frame_id)) {
    return false;  // 页面不在缓冲池中
  }

  // 检查pin计数（如果已经是0，不能再unpin）
  if (pages_[frame_id].GetPinCount() == 0) {
    return false;
  }

  // 减少pin计数
  pages_[frame_id].pin_count_--;

  // 更新脏页标志：只有当调用者标记为dirty时才更新（如果已经脏了则保持脏）
  if (is_dirty) {
    pages_[frame_id].is_dirty_ = true;
  }

  // 如果pin计数降为0，设置该帧为可驱逐状态
  if (pages_[frame_id].GetPinCount() == 0) {
    replacer_->SetEvictable(frame_id, true);
  }

  return true;
}

// 刷新指定页面到磁盘
auto BufferPoolManagerInstance::FlushPgImp(page_id_t page_id) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;
  // 检查页面是否在缓冲池中
  if (!page_table_->Find(page_id, frame_id)) {
    return false;  // 页面不在缓冲池中
  }

  // 将页面数据写回磁盘
  disk_manager_->WritePage(page_id, pages_[frame_id].GetData());

  // 刷新后，页面不再是脏页
  pages_[frame_id].is_dirty_ = false;

  return true;
}

// 刷新所有页面到磁盘
void BufferPoolManagerInstance::FlushAllPgsImp() {
  std::scoped_lock<std::mutex> lock(latch_);

  // 遍历所有帧
  for (size_t i = 0; i < pool_size_; ++i) {
    page_id_t page_id = pages_[i].GetPageId();
    // 如果帧中包含有效的页面（不是空闲帧）
    if (page_id != INVALID_PAGE_ID) {
      // 强制刷新到磁盘
      disk_manager_->WritePage(page_id, pages_[i].GetData());
      pages_[i].is_dirty_ = false;  // 标记为干净
    }
  }
}

// 删除指定页面
auto BufferPoolManagerInstance::DeletePgImp(page_id_t page_id) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;
  // 1. 检查页面是否在缓冲池中
  if (page_table_->Find(page_id, frame_id)) {
    // 2. 如果在缓冲池中，检查pin计数
    if (pages_[frame_id].GetPinCount() > 0) {
      // 页面正在被使用，无法删除
      return false;
    }
    // 3. 从缓冲池中移除该页面
    page_table_->Remove(page_id);              // 从页表移除映射
    replacer_->Remove(frame_id);               // 从替换器中移除
    free_list_.push_back(frame_id);            // 帧归还到空闲列表

    // 重置Page对象的元数据
    pages_[frame_id].ResetMemory();
    pages_[frame_id].page_id_ = INVALID_PAGE_ID;  // 设置为无效页面ID（**修正：直接访问成员变量**）
    pages_[frame_id].pin_count_ = 0;
    pages_[frame_id].is_dirty_ = false;
  }

  // 4. 无论页面是否在缓冲池中，都通知磁盘管理器释放该页面
  DeallocatePage(page_id);
  return true;
}

// 分配新页面ID（简单的自增计数器）
auto BufferPoolManagerInstance::AllocatePage() -> page_id_t { return next_page_id_++; }

}  // namespace bustub