#ifndef BOARD_BEE_LIBS_JSON_PARSER_H_
#define BOARD_BEE_LIBS_JSON_PARSER_H_

#include <libs/aliases.h>
#include <libs/arena_allocator.h>
#include <libs/json/node.h>
#include <libs/json/tokenizer.h>

namespace rose::json {

class Parser {
 public:
  Parser(vector<Token> &&tokens, const sptr<ArenaAllocator> &allocator)
      : tokens_(tokens), root_(nullptr), allocator_(allocator) {}

  Node *root() const { return root_; }

  void Parse();

 private:
  opt<Token> Peek(u32 offset = 1) const;
  void Consume(u32 n = 1);

  Node *ParseValue();
  Node *ParseObject();
  Node *ParseArray();
  Node *ParseString();
  Node *ParseNumber();
  Node *ParseBoolean();
  Node *ParseNull();

  vector<Token> tokens_;
  Node *root_;
  u64 i_ = 0;
  sptr<ArenaAllocator> allocator_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_PARSER_H_
