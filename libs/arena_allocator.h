#ifndef BOARD_BEE_LIBS_ARENA_ALLOCATOR_H_
#define BOARD_BEE_LIBS_ARENA_ALLOCATOR_H_

#include "aliases.h"

#include <functional>  // I don't know why they put std::byte here.
#include <sstream>

namespace rose {

class ArenaAllocator {
 public:
  explicit ArenaAllocator(const size_t bytes) : bytes_(bytes) {
    buffer_ = static_cast<std::byte *>(malloc(bytes_));
    start_ = buffer_;
    end_ = buffer_ + bytes_;
    pos_ = start_;
  }
  ArenaAllocator(const ArenaAllocator &other) = delete;
  ArenaAllocator &operator=(const ArenaAllocator &other) = delete;
  ArenaAllocator(ArenaAllocator &&other) = default;
  ArenaAllocator &operator=(ArenaAllocator &&other) = default;
  ~ArenaAllocator() { free(buffer_); }

  size_t free_bytes() const { return end_ - pos_; }

  template <typename T>
  T *Allocate(const u64 num_objects = 1) {
    if (num_objects == 0) {
      throw std::runtime_error("Tried to allocate room for 0 objects");
    }
    if (num_objects * sizeof(T) > free_bytes()) {
      std::stringstream error_msg("Tried to allocate ");
      error_msg << sizeof(T) << " bytes, but only " << free_bytes()
                << "are free";
      throw std::runtime_error(error_msg.str());
    }
    void *old_pos = pos_;
    pos_ += num_objects * sizeof(T);
    return static_cast<T *>(old_pos);
  }

 private:
  size_t bytes_;
  std::byte *buffer_;
  std::byte *start_;
  std::byte *end_;
  std::byte *pos_;
};

}  // namespace rose

#endif  // BOARD_BEE_LIBS_ARENA_ALLOCATOR_H_
