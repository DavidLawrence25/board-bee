#include <libs/json/parser.h>

#include <libs/aliases.h>
#include <libs/json/node.h>
#include <libs/json/tokenizer.h>

#include <sstream>

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

sptr<Node> Parser::ParseValue() {
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

sptr<Node> Parser::ParseObject() {
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
    object.emplace(token.value().value, ParseValue());
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
  return mk_sptr<Node>(object);
}

sptr<Node> Parser::ParseArray() {
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
  return mk_sptr<Node>(array);
}

sptr<Node> Parser::ParseString() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kString) return nullptr;
  Consume();
  return mk_sptr<Node>(token.value().value);
}

sptr<Node> Parser::ParseNumber() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kNumber) return nullptr;
  Consume();
  return token.value().value.contains('.')
      ? mk_sptr<Node>(std::stod(token.value().value))
      : mk_sptr<Node>(static_cast<s64>(std::stoi(token.value().value)));
}

sptr<Node> Parser::ParseBoolean() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kBoolean) return nullptr;
  Consume();
  return mk_sptr<Node>(token.value().value == "true");
}

sptr<Node> Parser::ParseNull() {
  const opt<Token> token = Peek();
  if (!token || token.value().type != Token::Type::kNull) return nullptr;
  Consume();
  return mk_sptr<Node>();
}

}  // namespace rose::json
