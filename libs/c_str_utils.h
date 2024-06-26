#ifndef BOARD_BEE_LIBS_C_STR_UTILS_H_
#define BOARD_BEE_LIBS_C_STR_UTILS_H_

#include <cstring>
#include <set>

namespace rose {

constexpr bool Contains(const char *string, const char c) {
  return string && strchr(string, c);
}

constexpr bool SetContains(const std::set<const char *> &set,
                           const char *string) {
  if (string == nullptr) return false;
  for (const char *test : set) {
    if (strcmp(string, test) != 0) return false;
  }
  return true;
}

}  // namespace rose

#endif  // BOARD_BEE_LIBS_C_STR_UTILS_H_
