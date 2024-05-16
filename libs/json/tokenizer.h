#ifndef BOARD_BEE_LIBS_JSON_TOKENIZER_H_
#define BOARD_BEE_LIBS_JSON_TOKENIZER_H_

#include <istream>

#include "../aliases.h"
#include "../arena_allocator.h"

namespace rose::json {

// Represents a token made during the tokenization phase of JSON parsing.
struct Token {
  // The type of the token.
  enum class Type { kLCurly, kRCurly, kColon, kString, kNumber,
                    kLSquare, kRSquare, kComma, kBoolean, kNull } type;
  // The string representation of the token (if applicable).
  const char *value;

  // Returns true if this token can be reasonably
  // interpreted as a JSON value (or the start of one).
  bool IsValue() const {
    return type == Type::kLCurly || type == Type::kString
        || type == Type::kNumber || type == Type::kLSquare
        || type == Type::kBoolean || type == Type::kNull;
  }
};

// Tokenizes a stream containing JSON data.
class Tokenizer {
 public:
  // `allocator` is used to allocate strings contigously on the heap.
  Tokenizer(std::istream &input, const sptr<ArenaAllocator> &allocator)
      : input_(input), allocator_(allocator) {}

  // Tokenizes `input_` and returns the resulting vector of Tokens.
  vector<Token> Tokenize();

 private:
  // Returns the character `offset - 1` characters ahead.
  opt<char> Peek(std::streamoff offset = 1) const;
  // Moves the stream position `offset` characters ahead.
  void Consume(std::streamoff n = 1);

  // Returns true if the start of the remaining characters
  // in the stream match the characters in `keyword`.
  bool ReadKeyword(str_view keyword);
  // Returns a C-string representation of a numeric literal.
  // Data is read from the current stream position onwards.
  const char *ReadNumericLiteral();
  // Returns a C-string representation of a string literal.
  // Data is read from the current stream position onwards, and the result will
  // not contain any unescaped '"' characters.
  const char *ReadStringLiteral();

  std::istream &input_;
  // Pointer to an ArenaAllocator used for allocating strings on the heap.
  sptr<ArenaAllocator> allocator_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_TOKENIZER_H_
