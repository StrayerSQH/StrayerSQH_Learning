//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.h
//
// Identification: src/include/buffer/lru_k_replacer.h
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <limits>
#include <list>
#include <mutex>  // NOLINT
#include <unordered_map>
#include <vector>

#include "common/config.h"
#include "common/macros.h"

namespace bustub {

/**
 * LRUKReplacer implements the LRU-k replacement policy.
 * LRUKReplacer 实现了 LRU-K 页面置换策略。
 *
 * The LRU-k algorithm evicts a frame whose backward k-distance is maximum
 * of all frames. Backward k-distance is computed as the difference in time between
 * current timestamp and the timestamp of kth previous access.
 * LRU-K 算法会驱逐那些“向后 K 距离”最大的帧。
 * “向后 K 距离”的定义是：当前时间戳 - 上一次第 K 次访问的时间戳。
 *
 * A frame with less than k historical references is given
 * +inf as its backward k-distance. When multiple frames have +inf backward k-distance,
 * classical LRU algorithm is used to choose victim.
 * 如果一个帧的历史访问次数少于 K 次，它的 K 距离被视为 +inf (无穷大)。
 * 当有多个帧的距离都是无穷大时（都在冷数据队列中），使用传统的 LRU（先进先出）算法选择受害者。
 */
class LRUKReplacer {
 public:
  /**
   * TODO(P1): Add implementation
   *
   * @brief a new LRUKReplacer.
   * @param num_frames the maximum number of frames the LRUReplacer will be required to store
   * @param k the k value for LRU-k (即衡量冷热的阈值，通常为 2)
   */
  explicit LRUKReplacer(size_t num_frames, size_t k);

  DISALLOW_COPY_AND_MOVE(LRUKReplacer);

  /**
   * TODO(P1): Add implementation
   *
   * @brief Destroys the LRUReplacer.
   */
  ~LRUKReplacer() = default;

  /**
   * TODO(P1): Add implementation
   *
   * @brief Find the frame with largest backward k-distance and evict that frame. Only frames
   * that are marked as 'evictable' are candidates for eviction.
   *
   * 查找并驱逐一个帧。核心逻辑：
   * 1. 只有标记为 'evictable' (可驱逐) 的帧才能被选中。
   * 2. 优先驱逐 "访问次数 < k" 的帧（即 K 距离为 +inf）。
   * - 如果有多个，驱逐最早访问的那个 (FIFO/LRU)。
   * 3. 如果所有可驱逐帧的访问次数都 >= k，则驱逐 K 距离最大的帧。
   * - 即：当前时间 - 第 K 次前访问时间 最大的那个。
   *
   * Successful eviction of a frame should decrement the size of replacer and remove the frame's
   * access history.
   * 成功驱逐后，需要减少 replacer 的 size，并清除该帧的所有历史记录。
   *
   * @param[out] frame_id id of frame that is evicted. (输出参数：被驱逐的帧 ID)
   * @return true if a frame is evicted successfully, false if no frames can be evicted.
   */
  auto Evict(frame_id_t *frame_id) -> bool;

  /**
   * TODO(P1): Add implementation
   *
   * @brief Record the event that the given frame id is accessed at current timestamp.
   * Create a new entry for access history if frame id has not been seen before.
   *
   * 记录一次访问：
   * 1. 增加全局时间戳 current_timestamp_。
   * 2. 记录该 frame_id 的访问时间。
   * 3. 关键逻辑：如果该帧的访问次数达到 K 次，可能需要将其从 history_list_ (冷队列) 移动到 cache_list_ (热队列)。
   *
   * If frame id is invalid (ie. larger than replacer_size_), throw an exception. You can
   * also use BUSTUB_ASSERT to abort the process if frame id is invalid.
   *
   * @param frame_id id of frame that received a new access.
   */
  void RecordAccess(frame_id_t frame_id);

  /**
   * TODO(P1): Add implementation
   *
   * @brief Toggle whether a frame is evictable or non-evictable. This function also
   * controls replacer's size. Note that size is equal to number of evictable entries.
   *
   * 设置帧是否可被驱逐 (Pin/Unpin):
   * - set_evictable = true (Unpin): 页面没人用了，可以作为驱逐候选。
   * - set_evictable = false (Pin): 页面正在被使用，绝对不能被 Evict 选中。
   *
   * 需要维护 curr_size_ (当前可驱逐帧的数量)。
   *
   * If a frame was previously evictable and is to be set to non-evictable, then size should
   * decrement. If a frame was previously non-evictable and is to be set to evictable,
   * then size should increment.
   *
   * If frame id is invalid, throw an exception or abort the process.
   *
   * For other scenarios, this function should terminate without modifying anything.
   *
   * @param frame_id id of frame whose 'evictable' status will be modified
   * @param set_evictable whether the given frame is evictable or not
   */
  void SetEvictable(frame_id_t frame_id, bool set_evictable);

  /**
   * TODO(P1): Add implementation
   *
   * @brief Remove an evictable frame from replacer, along with its access history.
   * This function should also decrement replacer's size if removal is successful.
   *
   * 手动移除一个帧（通常在 BufferPoolManager::DeletePage 时调用）。
   * 清除该帧的所有历史记录和元数据。
   *
   * Note that this is different from evicting a frame, which always remove the frame
   * with largest backward k-distance. This function removes specified frame id,
   * no matter what its backward k-distance is.
   *
   * If Remove is called on a non-evictable frame, throw an exception or abort the
   * process.
   *
   * If specified frame is not found, directly return from this function.
   *
   * @param frame_id id of frame to be removed
   */
  void Remove(frame_id_t frame_id);

  /**
   * TODO(P1): Add implementation
   *
   * @brief Return replacer's size, which tracks the number of evictable frames.
   * 返回当前可驱逐帧的数量 (注意：不是总帧数，而是 set_evictable=true 的数量)。
   *
   * @return size_t
   */
  auto Size() -> size_t;

 private:
  // TODO(student): implement me! You can replace these member variables as you like.
  // Helper struct to store metadata for each frame
  // 辅助结构体：存储每个帧的元数据
  struct LRUKNode {
    bool is_evictable_{false};   // 默认为 false (不可驱逐/被Pin住)，直到显式调用 SetEvictable(true)
    std::list<size_t> history_;  // 历史访问时间戳链表。建议：最新的时间在 front，最旧的在 back。
  };

  // Helper functions to manage eviction lists
  // 建议实现的辅助函数：用于在下面两个 list 和 map 之间同步操作
  void AddToHistoryList(frame_id_t frame_id);
  void AddToCacheList(frame_id_t frame_id);
  void RemoveFromHistoryList(frame_id_t frame_id);
  void RemoveFromCacheList(frame_id_t frame_id);

  // Map to store all frame metadata
  // 哈希表：存储所有曾经被访问过的帧的信息 (frame_id -> Node)
  std::unordered_map<frame_id_t, LRUKNode> node_store_;

  // Lists for evictable frames
  // 两个核心队列，只存储 evictable=true 的帧 ID
  
  // 1. 冷数据队列 (History List): 
  // 存放访问次数 < k 的帧。
  // 使用 FIFO 策略（新加入的在尾部，驱逐时从头部拿，或者相反，取决于具体实现）。
  // 这些帧的 K 距离被视为 +inf。
  std::list<frame_id_t> history_list_;

  // 2. 热数据队列 (Cache List):
  // 存放访问次数 >= k 的帧。
  // 这里面的帧需要根据 K 距离进行比较来决定驱逐谁。
  std::list<frame_id_t> cache_list_;

  // Maps to find list iterators in O(1)
  // 迭代器映射：为了能在 O(1) 时间内从 list 中删除某个帧 (Remove 操作)
  // Key: frame_id, Value: 指向 history_list_ 或 cache_list_ 中对应元素的迭代器
  std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator> history_list_map_;
  std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator> cache_list_map_;

  size_t current_timestamp_{0}; // 全局逻辑时钟，每次 RecordAccess +1
  size_t curr_size_{0};         // 当前可驱逐帧的数量 (Size() 的返回值)
  size_t replacer_size_;        // Replacer 能管理的最大帧数 (构造函数传入)
  size_t k_;                    // K 值 (构造函数传入)
  std::mutex latch_;            // 线程安全锁
};

}  // namespace bustub