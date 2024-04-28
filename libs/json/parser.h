#ifndef BOARD_BEE_LIBS_JSON_PARSER_H_
#define BOARD_BEE_LIBS_JSON_PARSER_H_

#include <libs/aliases.h>
#include <libs/json/node.h>
#include <libs/json/tokenizer.h>

namespace rose::json {

class Parser {
 public:
  explicit Parser(vector<Token> &&tokens) : tokens_(tokens) {}

  sptr<Node> root() const { return root_; }

  void Parse();

 private:
  opt<Token> Peek(u32 offset = 1) const;
  void Consume(u32 n = 1);

  sptr<Node> ParseValue();
  sptr<Node> ParseObject();
  sptr<Node> ParseArray();
  sptr<Node> ParseString();
  sptr<Node> ParseNumber();
  sptr<Node> ParseBoolean();
  sptr<Node> ParseNull();

  vector<Token> tokens_;
  sptr<Node> root_;
  u64 i_ = 0;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_PARSER_H_
