#ifndef BOARD_BEE_LIBS_JSON_TOKENIZER_H_
#define BOARD_BEE_LIBS_JSON_TOKENIZER_H_

#include <istream>

#include "../aliases.h"
#include "../arena_allocator.h"

namespace rose::json {

struct Token {
  enum class Type { kLCurly, kRCurly, kColon, kString, kNumber,
                    kLSquare, kRSquare, kComma, kBoolean, kNull } type;
  const char *value;

  bool IsValue() const {
    return type == Type::kLCurly || type == Type::kString
        || type == Type::kNumber || type == Type::kLSquare
        || type == Type::kBoolean || type == Type::kNull;
  }
};

class Tokenizer {
 public:
  Tokenizer(std::istream &input, const sptr<ArenaAllocator> &allocator)
      : input_(input), allocator_(allocator) {}

  vector<Token> Tokenize();

 private:
  opt<char> Peek(std::streamoff offset = 1) const;
  void Consume(std::streamoff n = 1);

  bool ReadKeyword(str_view keyword);
  const char *ReadNumericLiteral();
  const char *ReadStringLiteral();

  std::istream &input_;
  sptr<ArenaAllocator> allocator_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_TOKENIZER_H_
