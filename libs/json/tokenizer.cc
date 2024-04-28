#include <libs/json/tokenizer.h>

#include <libs/aliases.h>

#include <exception>
#include <sstream>

namespace rose::json {

str Token::repr() const {
  if (type == Type::kString) {
    std::stringstream sout;
    sout << '"' << value << '"';
    return sout.str();
  }
  if (type == Type::kBoolean || type == Type::kNumber) return value;
  if (type == Type::kNull) return "null";
  if (type == Type::kColon) return ":";
  if (type == Type::kComma) return ",";
  if (type == Type::kLCurly) return "{";
  if (type == Type::kRCurly) return "}";
  if (type == Type::kLSquare) return "[";
  if (type == Type::kRSquare) return "]";
  throw std::runtime_error("what the f*ck");
}

vector<Token> Tokenizer::Tokenize() {
  vector<Token> tokens;
  while (const opt<char> c = Peek()) {
    if (c == '-' || std::isdigit(c.value())) {
      tokens.emplace_back(Token::Type::kNumber, ReadNumericLiteral());
      continue;
    }
    if (c == 'n' && ReadKeyword("null")) {
      Consume(4);
      tokens.emplace_back(Token::Type::kNull);
      continue;
    }
    if (c == 't' && ReadKeyword("true")) {
      Consume(4);
      tokens.emplace_back(Token::Type::kBoolean, "true");
      continue;
    }
    if (c == 'f' && ReadKeyword("false")) {
      Consume(5);
      tokens.emplace_back(Token::Type::kBoolean, "false");
      continue;
    }
    Consume();
    if (std::isspace(c.value())) continue;
    if (c == '"') {
      tokens.emplace_back(Token::Type::kString, ReadStringLiteral());
    } else if (c == ':') {
      tokens.emplace_back(Token::Type::kColon);
    } else if (c == ',') {
      tokens.emplace_back(Token::Type::kComma);
    } else if (c == '{') {
      tokens.emplace_back(Token::Type::kLCurly);
    } else if (c == '}') {
      tokens.emplace_back(Token::Type::kRCurly);
    } else if (c == '[') {
      tokens.emplace_back(Token::Type::kLSquare);
    } else if (c == ']') {
      tokens.emplace_back(Token::Type::kRSquare);
    } else {
      throw std::runtime_error("Tokenization failed");
    }
  }
  return tokens;
}

opt<char> Tokenizer::Peek(const u32 offset) const {
  if (offset == 0) return std::nullopt;
  fin_->seekg(offset - 1, std::ios_base::cur);
  const s32 c = fin_->get();
  fin_->putback(static_cast<char>(c));
  fin_->seekg(1 - static_cast<s64>(offset), std::ios_base::cur);
  fin_->clear();
  return c == EOF ? std::nullopt : mk_opt<char>(c);
}

void Tokenizer::Consume(const u32 n) { fin_->seekg(n, std::ios_base::cur); }

bool Tokenizer::ReadKeyword(const str_view keyword) {
  for (u64 i = 0; i < keyword.size(); ++i) {
    const opt<char> c = Peek(i + 1);
    if (!c || c != keyword[i]) return false;
  }
  return true;
}

str Tokenizer::ReadNumericLiteral() {
  opt<char> c = Peek();
  std::stringstream sout;
  if (c == '0') {
    const opt<char> next = Peek(2);
    if (next && std::isdigit(next.value())) {
      throw std::runtime_error("Numeric literals cannot start with '0' "
                               "immediately followed by another digit");
    }
  } else if (c == '-') {
    const opt<char> next = Peek(2);
    if (!next || !std::isdigit(next.value())) {
      throw std::runtime_error("Negative numbers must have a digit after '-'");
    }
    sout << '-';
    Consume();
    c = Peek();
  }
  while (c) {
    if (std::isdigit(c.value())) {
      sout << c.value();
    } else if (c == '.') {
      const opt<char> next = Peek(2);
      if (!next || !std::isdigit(next.value())) {
        throw std::runtime_error("Decimals must be followed by a digit");
      }
      sout << '.';
    } else {
      break;
    }
    Consume();
    c = Peek();
  }
  return sout.str();
}

str Tokenizer::ReadStringLiteral() {
  std::stringstream sout;
  while (const opt<char> c = Peek()) {
    Consume();
    if (c == '"') return sout.str();
    sout << c.value();
    if (c == '\\') {
      opt<char> escaped = Peek();
      if (!escaped.has_value()) break;
      Consume();
      sout << escaped.value();
    }
  }
  throw std::runtime_error("Hit EOF before end of string literal");
}

} // namespace rose::json
