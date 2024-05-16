#ifndef BOARD_BEE_LIBS_JSON_WRITER_H_
#define BOARD_BEE_LIBS_JSON_WRITER_H_

#include <ostream>

#include "../aliases.h"
#include "node.h"

namespace rose::json {

// Writes a JSON parse tree to an output stream.
class Writer {
 public:
  // Number of spaces to insert per indentation level.
  static constexpr u8 kIndentSize = 2;

  explicit Writer(std::ostream &output) : output_(output) {}
  // Copying a Writer could potentially break certain invariants.
  Writer(const Writer &writer) = delete;
  Writer(Writer &&writer) = default;
  ~Writer() = default;

  // Writes the contents of `node` to the output stream.
  void Write(const Node *node);

 private:
  // Writes the contents of `node` to the output stream.
  // Assumes `node` is a pointer to an Object Node.
  void WriteObject(const Node *node);
  // Writes the contents of `node` to the output stream.
  // Assumes `node` is a pointer to an Array Node.
  void WriteArray(const Node *node);
  // Writes the contents of `node` to the output stream.
  // Assumes `node` is a pointer to a string Node.
  void WriteString(const Node *node);
  // Writes the contents of `node` to the output stream.
  // Assumes `node` is a pointer to an s64 Node.
  void WriteS64(const Node *node);
  // Writes the contents of `node` to the output stream.
  // Assumes `node` is a pointer to an f64 Node.
  void WriteF64(const Node *node);
  // Writes the contents of `node` to the output stream.
  // Assumes `node` is a pointer to a boolean Node.
  void WriteBoolean(const Node *node);
  // Writes the string "null" to the output stream.
  void WriteNull();

  // Writes `indent_level_ * kIndentSize` spaces to the output stream.
  void Indent();

  std::ostream &output_;
  u64 indent_level_ = 0;
  bool level_empty_ = true;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_WRITER_H_
