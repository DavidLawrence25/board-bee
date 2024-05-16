#ifndef BOARD_BEE_LIBS_JSON_PARSER_H_
#define BOARD_BEE_LIBS_JSON_PARSER_H_

#include "../aliases.h"
#include "../arena_allocator.h"
#include "node.h"
#include "tokenizer.h"

namespace rose::json {

// Parses a vector of Tokens into a parse tree.
class Parser {
 public:
  // `allocator` is used to allocate Nodes contiguously on the heap.
  Parser(vector<Token> &&tokens, const sptr<ArenaAllocator> &allocator)
      : tokens_(tokens), allocator_(allocator) {}
  Parser(const Parser &other) = default;
  Parser &operator=(const Parser &other) = default;
  Parser(Parser &&other) = default;
  Parser &operator=(Parser &&other) = default;
  ~Parser() = default;

  // Returns the root node in the parse tree.
  // Returns nullptr if the Parse method has not been called yet.
  Node *root() const noexcept { return root_; }

  // Parses the vector of Tokens into a parse tree.
  // Result can be accessed by calling the root method.
  void Parse();

 private:
  // Returns the Token `offset - 1` Tokens ahead.
  // Returns std::nullopt if no such Token exists.
  opt<Token> Peek(u32 offset = 1) const;
  // Moves the read position `n` Tokens ahead. Stops at the end if needed.
  void Consume(u32 n = 1);

  // Returns a pointer to a new value Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseValue();
  // Returns a pointer to a new Object Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseObject();
  // Returns a pointer to a new Array Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseArray();
  // Returns a pointer to a new string Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseString();
  // Returns a pointer to a new number Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseNumber();
  // Returns a pointer to a new boolean Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseBoolean();
  // Returns a pointer to a new null Node.
  // Throws either a WrongTokenTypeError or MissingTokenError upon failure.
  Node *ParseNull();

  vector<Token> tokens_;
  Node *root_ = nullptr;
  // Current index into `tokens_`.
  u64 i_ = 0;
  // Pointer to an ArenaAllocator used for allocating Nodes on the heap.
  sptr<ArenaAllocator> allocator_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_PARSER_H_
