#ifndef BOARD_BEE_LIBS_JSON_EXCEPTIONS_H_
#define BOARD_BEE_LIBS_JSON_EXCEPTIONS_H_

#include <exception>

namespace rose::json {

class UndefinedTypeError final : public std::exception {
 public:
  UndefinedTypeError() : what_("Got an invalid type for JSON Node") {}
  explicit UndefinedTypeError(const char *what) : what_(what) {}
  explicit UndefinedTypeError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

class TokenizationError final : public std::exception {
 public:
  TokenizationError() : what_("Failed to tokenize input") {}
  explicit TokenizationError(const char *what) : what_(what) {}
  explicit TokenizationError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

class MissingTokenError final : public std::exception {
 public:
  MissingTokenError() : what_("Expected a Token, but couldn't find one") {}
  explicit MissingTokenError(const char *what) : what_(what) {}
  explicit MissingTokenError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

class WrongTokenTypeError final : public std::exception {
 public:
  WrongTokenTypeError() : what_("Got wrong type of Token"), malloced(false) {}
  explicit WrongTokenTypeError(const char *what) : what_(what),
                                                   malloced(false) {}
  explicit WrongTokenTypeError(const str &string) : what_(string.c_str()),
                                                    malloced(false) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
  const bool malloced;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_EXCEPTIONS_H_
