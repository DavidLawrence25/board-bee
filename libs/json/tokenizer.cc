#include <libs/json/tokenizer.h>

#include <libs/aliases.h>
#include <libs/arena_allocator.h>

#include <exception>

namespace rose::json {

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

opt<char> Tokenizer::Peek(const std::streamoff offset) const {
  if (offset == 0) return std::nullopt;
  input_.seekg(offset - 1, std::ios_base::cur);
  const s32 c = input_.get();
  input_.putback(static_cast<char>(c));
  input_.seekg(1 - offset, std::ios_base::cur);
  input_.clear();
  return c == EOF ? std::nullopt : mk_opt<char>(c);
}

void Tokenizer::Consume(const std::streamoff n) {
  input_.seekg(n, std::ios_base::cur);
}

bool Tokenizer::ReadKeyword(const str_view keyword) {
  for (u32 i = 0; i < keyword.size(); ++i) {
    const opt<char> c = Peek(i + 1);
    if (!c || c != keyword[i]) return false;
  }
  return true;
}

#define ALLOCATE_NEW_CHUNK_IF_FULL                                \
if (chars_written >= buffer_capacity) {                           \
  static_cast<void>(allocator_->Allocate<char>(kCharChunkSize));  \
  buffer_capacity += kCharChunkSize;                              \
}

const char *Tokenizer::ReadNumericLiteral() {
  static constexpr u64 kCharChunkSize = 4;
  char *string_buffer = allocator_->Allocate<char>(kCharChunkSize);
  size_t buffer_capacity = kCharChunkSize;
  u64 chars_written = 0;
  opt<char> c = Peek();
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
    string_buffer[chars_written++] = '-';
    ALLOCATE_NEW_CHUNK_IF_FULL;
    Consume();
    c = Peek();
  }
  while (c) {
    if (std::isdigit(c.value())) {
      string_buffer[chars_written++] = c.value();
      ALLOCATE_NEW_CHUNK_IF_FULL;
    } else if (c == '.') {
      const opt<char> next = Peek(2);
      if (!next || !std::isdigit(next.value())) {
        throw std::runtime_error("Decimals must be followed by a digit");
      }
      string_buffer[chars_written++] = '.';
      ALLOCATE_NEW_CHUNK_IF_FULL;
    } else {
      break;
    }
    Consume();
    c = Peek();
  }
  string_buffer[chars_written] = '\0';
  return string_buffer;
}

const char *Tokenizer::ReadStringLiteral() {
  static constexpr u64 kCharChunkSize = 8;
  char *string_buffer = allocator_->Allocate<char>(kCharChunkSize);
  size_t buffer_capacity = kCharChunkSize;
  u64 chars_written = 0;
  while (const opt<char> c = Peek()) {
    Consume();
    if (c == '"') {
      string_buffer[chars_written] = '\0';
      return string_buffer;
    }
    string_buffer[chars_written++] = c.value();
    ALLOCATE_NEW_CHUNK_IF_FULL;
    if (c == '\\') {
      opt<char> escaped = Peek();
      if (!escaped.has_value()) break;
      Consume();
      string_buffer[chars_written++] = escaped.value();
      ALLOCATE_NEW_CHUNK_IF_FULL;
    }
  }
  throw std::runtime_error("Hit EOF before end of string literal");
}

} // namespace rose::json
