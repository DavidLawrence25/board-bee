#include "parser.h"

#include <sstream>

#include "../aliases.h"
#include "../c_str_utils.h"
#include "exceptions.h"
#include "node.h"
#include "tokenizer.h"

namespace rose::json {

opt<Token> Parser::Peek(const u32 offset) const {
  if (i_ + offset - 1 >= tokens_.size()) return std::nullopt;
  return mk_opt<Token>(tokens_.at(i_ + offset - 1));
}

void Parser::Consume(const u32 n) {
  i_ = std::min(i_ + n, tokens_.size() - 1);
}

void Parser::Parse() {
  const opt<Token> first = Peek();
  if (!first) throw MissingTokenError("Can't parse nothing");
  if (first.value().type == Token::Type::kLCurly) {
    root_ = ParseObject();
  } else if (first.value().type == Token::Type::kLSquare) {
    root_ = ParseArray();
  } else {
    throw WrongTokenTypeError("JSON files must be objects or arrays");
  }
}

Node *Parser::ParseValue() {
  const opt<Token> token = Peek();
  if (!token) return nullptr;  // I'm not sold on this quite yet.
  if (token.value().type == Token::Type::kLCurly) return ParseObject();
  if (token.value().type == Token::Type::kString) return ParseString();
  if (token.value().type == Token::Type::kNumber) return ParseNumber();
  if (token.value().type == Token::Type::kLSquare) return ParseArray();
  if (token.value().type == Token::Type::kBoolean) return ParseBoolean();
  if (token.value().type == Token::Type::kNull) return ParseNull();
  throw WrongTokenTypeError("Next token does not represent a value");
}

Node *Parser::ParseObject() {
  const opt<Token> root_curly = Peek();
  if (!root_curly) throw MissingTokenError();
  if (root_curly.value().type != Token::Type::kLCurly) {
    throw WrongTokenTypeError("Expected '{' at start of object Node");
  }
  Consume();
  Object object;
  opt<Token> token = Peek();
  if (!token) throw MissingTokenError("Expected Token after '{'");
  while (token.value().type != Token::Type::kRCurly) {
    if (token.value().type != Token::Type::kString) {
      throw WrongTokenTypeError("Expected '}' or key-value pair after '{'");
    }
    Consume();
    const opt<Token> colon = Peek();
    if (!colon) {
      std::stringstream error_msg;
      error_msg << "Expected Token after key \"" << token.value().value << '"';
      throw MissingTokenError(error_msg.str());
    }
    if (colon.value().type != Token::Type::kColon) {
      std::stringstream error_msg;
      error_msg << "Expected ':' after key \"" << token.value().value << '"';
      throw WrongTokenTypeError(error_msg.str());
    }
    Consume();
    Node *value = ParseValue();
    object.emplace(token.value().value, value);
    token = Peek();
    if (!token) throw MissingTokenError("Expected Token after key-value pair");
    if (token.value().type == Token::Type::kComma) {
      const opt<Token> lookahead = Peek(2);
      if (!lookahead) throw MissingTokenError("Expected Token after ','");
      if (lookahead.value().type == Token::Type::kRCurly) {
        throw WrongTokenTypeError("Trailing commas are not allowed");
      }
      Consume();
      token = lookahead;
    } else if (token.value().IsValue()) {
      throw WrongTokenTypeError("Key-value pairs must be comma-delimited");
    }
  }
  Consume();
  auto *object_node = allocator_->Allocate<Node>();
  object_node->set_value(object);
  return object_node;
}

Node *Parser::ParseArray() {
  const opt<Token> root_square = Peek();
  if (!root_square) throw MissingTokenError();
  if (root_square.value().type != Token::Type::kLSquare) {
    throw WrongTokenTypeError("Expected '[' at start of array Node");
  }
  Consume();
  Array array;
  opt<Token> token = Peek();
  if (!token) throw MissingTokenError("Expected Token after '['");
  while (token.value().type != Token::Type::kRSquare) {
    if (!token.value().IsValue()) {
      throw WrongTokenTypeError("Expected ']' or value after '['");
    }
    array.emplace_back(ParseValue());
    token = Peek();
    if (!token) throw MissingTokenError("Expected token after value");
    if (token.value().type == Token::Type::kComma) {
      const opt<Token> lookahead = Peek(2);
      if (!lookahead) throw MissingTokenError("Expected token after ','");
      if (lookahead.value().type == Token::Type::kRSquare) {
        throw WrongTokenTypeError("Trailing commas are not allowed");
      }
      Consume();
      token = lookahead;
    } else if (token.value().IsValue()) {
      throw WrongTokenTypeError("Values must be comma-delimited");
    }
  }
  Consume();
  auto *array_node = allocator_->Allocate<Node>();
  array_node->set_value(array);
  return array_node;
}

Node *Parser::ParseString() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kString) return nullptr;
  Consume();
  auto *string_node = allocator_->Allocate<Node>();
  string_node->set_value(token.value().value);
  return string_node;
}

Node *Parser::ParseNumber() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kNumber) return nullptr;
  Consume();
  auto *num_node = allocator_->Allocate<Node>();
  if (Contains(token.value().value, '.')) {
    num_node->set_value(std::stod(token.value().value));
  } else {
    num_node->set_value(static_cast<s64>(std::stoi(token.value().value)));
  }
  return num_node;
}

Node *Parser::ParseBoolean() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kBoolean) return nullptr;
  Consume();
  auto *bool_node = allocator_->Allocate<Node>();
  bool_node->set_value(*token.value().value == 't');
  return bool_node;
}

Node *Parser::ParseNull() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kNull) return nullptr;
  Consume();
  auto *null_node = allocator_->Allocate<Node>();
  null_node->set_value();
  return null_node;
}

}  // namespace rose::json
