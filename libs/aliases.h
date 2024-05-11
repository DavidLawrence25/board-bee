#ifndef BOARD_BEE_LIBS_ALIASES_H_
#define BOARD_BEE_LIBS_ALIASES_H_

#include <cstdint>
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using f32 = float;
using f64 = double;

using str = std::string;
using str_view = std::string_view;

template <typename T>
using opt = std::optional<T>;

template <typename T, typename... Args>
opt<T> mk_opt(Args&&... args) { return std::make_optional<T>(args...); }

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T, typename... Args>
uptr<T> mk_uptr(Args&&... args) { return std::make_unique<T>(args...); }

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T, typename... Args>
sptr<T> mk_sptr(Args&&... args) { return std::make_shared<T>(args...); }

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using std::vector;

#endif  // BOARD_BEE_LIBS_ALIASES_H_
