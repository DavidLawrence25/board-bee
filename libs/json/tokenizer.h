#ifndef BOARD_BEE_LIBS_JSON_TOKENIZER_H_
#define BOARD_BEE_LIBS_JSON_TOKENIZER_H_

#include <libs/aliases.h>

#include <fstream>

namespace rose::json {

struct Token {
  enum class Type { kLCurly, kRCurly, kColon, kString, kNumber,
                    kLSquare, kRSquare, kComma, kBoolean, kNull } type;
  str value;

  str repr() const;

  bool IsValue() const {
    return type == Type::kLCurly || type == Type::kString
        || type == Type::kNumber || type == Type::kLSquare
        || type == Type::kBoolean || type == Type::kNull;
  }
};

class Tokenizer {
 public:
  explicit Tokenizer(const str_view file_path)
      : fin_(mk_uptr<std::ifstream>(file_path.data())) {}
  explicit Tokenizer(uptr<std::ifstream> file_in) : fin_(std::move(file_in)) {}

  vector<Token> Tokenize();

 private:
  opt<char> Peek(u32 offset = 1) const;
  void Consume(u32 n = 1);

  bool ReadKeyword(str_view keyword);
  str ReadNumericLiteral();
  str ReadStringLiteral();

  uptr<std::ifstream> fin_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_TOKENIZER_H_
