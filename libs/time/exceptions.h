#ifndef BOARD_BEE_LIBS_TIME_EXCEPTIONS_H_
#define BOARD_BEE_LIBS_TIME_EXCEPTIONS_H_

#include <exception>

namespace rose::time {

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

}  // namespace rose::time

#endif  // BOARD_BEE_LIBS_TIME_EXCEPTIONS_H_
