#include "parser.h"

#include <sstream>

#include "../aliases.h"
#include "../c_str_utils.h"
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
  if (!first) throw std::runtime_error("Can't parse nothing");
  if (first.value().type == Token::Type::kLCurly) {
    root_ = ParseObject();
  } else if (first.value().type == Token::Type::kLSquare) {
    root_ = ParseArray();
  } else {
    throw std::runtime_error("JSON files must be objects or arrays");
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
  throw std::runtime_error("Next token does not represent a value");
}

Node *Parser::ParseObject() {
  const opt<Token> root_curly = Peek();
  if (!root_curly || root_curly.value().type != Token::Type::kLCurly) {
    throw std::runtime_error("Tried to parse an object without '{'");
  }
  Consume();
  Object object;
  opt<Token> token = Peek();
  if (!token) throw std::runtime_error("Expected token after '{'");
  while (token.value().type != Token::Type::kRCurly) {
    if (token.value().type != Token::Type::kString) {
      throw std::runtime_error("Expected '}' or key-value pair after '{'");
    }
    Consume();
    const opt<Token> colon = Peek();
    if (!colon || colon.value().type != Token::Type::kColon) {
      std::stringstream error_msg;
      error_msg << "Expected ':' after key \"" << token.value().value << '"';
      throw std::runtime_error(error_msg.str());
    }
    Consume();
    Node *value = ParseValue();
    object.emplace(token.value().value, value);
    token = Peek();
    if (!token) throw std::runtime_error("Expected token after key-value pair");
    if (token.value().type == Token::Type::kComma) {
      const opt<Token> lookahead = Peek(2);
      if (!lookahead) throw std::runtime_error("Expected token after ','");
      if (lookahead.value().type == Token::Type::kRCurly) {
        throw std::runtime_error("Trailing commas are not allowed");
      }
      Consume();
      token = lookahead;
    } else if (token.value().IsValue()) {
      throw std::runtime_error("Key-value pairs must be comma-delimited");
    }
  }
  Consume();
  auto *object_node = allocator_->Allocate<Node>();
  object_node->set_value(object);
  return object_node;
}

Node *Parser::ParseArray() {
  const opt<Token> root_square = Peek();
  if (!root_square || root_square.value().type != Token::Type::kLSquare) {
    throw std::runtime_error("Tried to parse an object without '['");
  }
  Consume();
  Array array;
  opt<Token> token = Peek();
  if (!token) throw std::runtime_error("Expected token after '['");
  while (token.value().type != Token::Type::kRSquare) {
    if (!token.value().IsValue()) {
      throw std::runtime_error("Expected ']' or value after '['");
    }
    array.emplace_back(ParseValue());
    token = Peek();
    if (!token) throw std::runtime_error("Expected token after value");
    if (token.value().type == Token::Type::kComma) {
      const opt<Token> lookahead = Peek(2);
      if (!lookahead) throw std::runtime_error("Expected token after ','");
      if (lookahead.value().type == Token::Type::kRSquare) {
        throw std::runtime_error("Trailing commas are not allowed");
      }
      Consume();
      token = lookahead;
    } else if (token.value().IsValue()) {
      throw std::runtime_error("Values must be comma-delimited");
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
