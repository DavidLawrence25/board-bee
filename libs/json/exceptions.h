#ifndef BOARD_BEE_LIBS_JSON_EXCEPTIONS_H_
#define BOARD_BEE_LIBS_JSON_EXCEPTIONS_H_

#include <exception>

namespace rose::json {

// Value of `Node::type_` isn't a valid enum value.
class UndefinedTypeError final : public std::exception {
 public:
  UndefinedTypeError() : what_("Got an invalid type for JSON Node") {}
  explicit UndefinedTypeError(const char *what) : what_(what) {}
  explicit UndefinedTypeError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

// Tokenization process failed due to invalid JSON.
class TokenizationError final : public std::exception {
 public:
  TokenizationError() : what_("Failed to tokenize input") {}
  explicit TokenizationError(const char *what) : what_(what) {}
  explicit TokenizationError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

// Expected a Token that was not provided during the parsing phase.
class MissingTokenError final : public std::exception {
 public:
  MissingTokenError() : what_("Expected a Token, but couldn't find one") {}
  explicit MissingTokenError(const char *what) : what_(what) {}
  explicit MissingTokenError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

// Found a Token of the wrong type during the parsing phase.
class WrongTokenTypeError final : public std::exception {
 public:
  WrongTokenTypeError() : what_("Got wrong type of Token") {}
  explicit WrongTokenTypeError(const char *what) : what_(what) {}
  explicit WrongTokenTypeError(const str &string) : what_(string.c_str()) {}

  const char *what() const noexcept override { return what_; }

 private:
  const char *what_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_EXCEPTIONS_H_
