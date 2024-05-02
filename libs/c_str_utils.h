#ifndef BOARD_BEE_LIBS_C_STR_UTILS_H_
#define BOARD_BEE_LIBS_C_STR_UTILS_H_

namespace rose {

constexpr bool Contains(const char *string, const char c) {
  if (string == nullptr) return false;
  const char *ptr = string;
  while (const char current = *ptr) {
    if (current == c) return true;
    ++ptr;
  }
  return false;
}

}  // namespace rose

#endif  // BOARD_BEE_LIBS_C_STR_UTILS_H_
