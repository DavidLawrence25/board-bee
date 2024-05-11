#ifndef BOARD_BEE_SRC_EXCEPTIONS_H_
#define BOARD_BEE_SRC_EXCEPTIONS_H_

#include <exception>

namespace bee {

class BadDateTimeException final : public std::exception {
 public:
  BadDateTimeException()
      : what_("DateTime constructor given non-ISO-8601 string") {}
  explicit BadDateTimeException(const char *what) : what_(what) {}
  explicit BadDateTimeException(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

}  // namespace bee

#endif  // BOARD_BEE_SRC_EXCEPTIONS_H_
