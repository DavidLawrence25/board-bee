#ifndef BOARD_BEE_LIBS_JSON_WRITER_H_
#define BOARD_BEE_LIBS_JSON_WRITER_H_

#include <libs/aliases.h>
#include <libs/json/node.h>

#include <ostream>

namespace rose::json {

class Writer {
 public:
  static constexpr u8 kIndentSize = 2;

  explicit Writer(std::ostream &output) : output_(output) {}
  Writer(const Writer &writer) = delete;
  Writer(Writer &&writer) = default;
  ~Writer() = default;

  void Write(const Node *node);

 private:
  void WriteObject(const Node *node);
  void WriteArray(const Node *node);
  void WriteString(const Node *node);
  void WriteS64(const Node *node);
  void WriteF64(const Node *node);
  void WriteBoolean(const Node *node);
  void WriteNull();

  void Indent();

  std::ostream &output_;
  u64 indent_level_ = 0;
  bool level_empty_ = true;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_WRITER_H_
