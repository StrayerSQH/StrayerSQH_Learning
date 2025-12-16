#include "buffer/lru_k_replacer.h"
#include <limits>

namespace bustub {

LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  if (curr_size_ == 0) {
    return false;
  }

  // 1. 尝试从 history_list_ (访问 < k 次) 驱逐 [cite: 834, 848, 895]
  //    按 LRU 策略 (最早的时间戳)
  if (!history_list_.empty()) {
    *frame_id = history_list_.back();  // back() 是 LRU 元素
    history_list_.pop_back();
    history_list_map_.erase(*frame_id);

    node_store_.erase(*frame_id);
    curr_size_--;
    return true;
  }

  // 2. 尝试从 cache_list_ (访问 >= k 次) 驱逐 [cite: 834, 847, 894]
  //    按最早的 k-th 时间戳
  if (!cache_list_.empty()) {
    frame_id_t victim_frame = -1;
    size_t earliest_k_ts = std::numeric_limits<size_t>::max();

    // 遍历 cache_list_ 找到 k-th 时间戳最早的帧
    for (frame_id_t fid : cache_list_) {
      auto &node = node_store_[fid];
      // history_.back() 存储的是最早的访问记录，即 k-th 访问时间戳
      size_t current_k_ts = node.history_.back();
      if (current_k_ts < earliest_k_ts) {
        earliest_k_ts = current_k_ts;
        victim_frame = fid;
      }
    }

    *frame_id = victim_frame;
    RemoveFromCacheList(victim_frame);  // 使用辅助函数 O(1) 删除
    node_store_.erase(victim_frame);
    curr_size_--;
    return true;
  }

  return false;
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id) {
  std::scoped_lock<std::mutex> lock(latch_);
  BUSTUB_ASSERT(static_cast<size_t>(frame_id) < replacer_size_, "Invalid frame ID");

  size_t timestamp = ++current_timestamp_;

  // 如果帧不存在则创建
  auto &node = node_store_[frame_id];
  size_t old_access_count = node.history_.size();

  // 记录访问
  node.history_.push_front(timestamp);
  if (node.history_.size() > k_) {
    node.history_.pop_back();
  }

  // 检查是否从 history_list_ "晋升" 到 cache_list_ [cite: 889, 899]
  if (node.is_evictable_ && old_access_count < k_ && node.history_.size() == k_) {
    RemoveFromHistoryList(frame_id);
    AddToCacheList(frame_id);
  }
}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
  std::scoped_lock<std::mutex> lock(latch_);
  BUSTUB_ASSERT(static_cast<size_t>(frame_id) < replacer_size_, "Invalid frame ID");

  auto node_it = node_store_.find(frame_id);
  if (node_it == node_store_.end()) {
    // 教程中没有明确说明，但如果一个从未被访问的帧被设置为可驱逐，
    // 我们应该记录它。然而，RecordAccess 总是先被调用。
    // 如果没有访问记录，我们忽略它。
    return;
  }

  auto &node = node_it->second;
  if (node.is_evictable_ == set_evictable) {
    return;
  }

  node.is_evictable_ = set_evictable;
  if (set_evictable) {
    // 变为可驱逐，加入正确的列表 [cite: 841]
    curr_size_++;
    if (node.history_.size() < k_) {
      AddToHistoryList(frame_id);
    } else {
      AddToCacheList(frame_id);
    }
  } else {
    // 变为不可驱逐，从列表中移除
    curr_size_--;
    if (history_list_map_.count(frame_id) != 0) {
      RemoveFromHistoryList(frame_id);
    } else if (cache_list_map_.count(frame_id) != 0) {
      RemoveFromCacheList(frame_id);
    }
  }
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
  std::scoped_lock<std::mutex> lock(latch_);
  BUSTUB_ASSERT(static_cast<size_t>(frame_id) < replacer_size_, "Invalid frame ID");

  auto node_it = node_store_.find(frame_id);
  if (node_it == node_store_.end()) {
    return;
  }

  if (!node_it->second.is_evictable_) {
    // .h 文件要求如果在一个不可驱逐的帧上调用 Remove，应抛出异常或中止
    BUSTUB_ASSERT(false, "Remove called on non-evictable frame");
  }

  // 从它所在的列表中移除
  curr_size_--;
  if (node_it->second.history_.size() < k_) {
    RemoveFromHistoryList(frame_id);
  } else {
    RemoveFromCacheList(frame_id);
  }

  // 移除所有历史记录
  node_store_.erase(node_it);
}

auto LRUKReplacer::Size() -> size_t {
  std::scoped_lock<std::mutex> lock(latch_);
  return curr_size_;
}

// --- 辅助函数实现 ---

void LRUKReplacer::AddToHistoryList(frame_id_t frame_id) {
  history_list_.push_front(frame_id);  // 新条目放在最前面 (MRU)
  history_list_map_[frame_id] = history_list_.begin();
}

void LRUKReplacer::AddToCacheList(frame_id_t frame_id) {
  cache_list_.push_front(frame_id);  // 顺序无关紧要，因为 Evict 会扫描
  cache_list_map_[frame_id] = cache_list_.begin();
}

void LRUKReplacer::RemoveFromHistoryList(frame_id_t frame_id) {
  auto it = history_list_map_.find(frame_id);
  if (it != history_list_map_.end()) {
    history_list_.erase(it->second);
    history_list_map_.erase(it);
  }
}

void LRUKReplacer::RemoveFromCacheList(frame_id_t frame_id) {
  auto it = cache_list_map_.find(frame_id);
  if (it != cache_list_map_.end()) {
    cache_list_.erase(it->second);
    cache_list_map_.erase(it);
  }
}

}  // namespace bustub