#include <libs/json/writer.h>

#include <libs/json/node.h>

#include <cmath>
#include <sstream>

namespace rose::json {

void Writer::Write(const Node *node) {
  if (node->is_object()) {
    WriteObject(node);
  } else if (node->is_array()) {
    WriteArray(node);
  } else if (node->is_string()) {
    WriteString(node);
  } else if (node->is_s64()) {
    WriteS64(node);
  } else if (node->is_f64()) {
    WriteF64(node);
  } else if (node->is_bool()) {
    WriteBoolean(node);
  } else if (node->is_null()) {
    WriteNull();
  } else {
    std::stringstream error_msg;
    error_msg << "Tried to write node with extraneous type at " << node;
    throw std::runtime_error(error_msg.str());
  }
}

void Writer::WriteObject(const Node *node) {
  output_ << '{';
  level_empty_ = true;
  ++indent_level_;
  for (const auto &[key, value] : *node->as_object().value()) {
    if (!level_empty_) output_ << ',';
    output_ << '\n';
    Indent();
    level_empty_ = false;
    output_ << '"' << key << "\": ";
    Write(value);
  }
  --indent_level_;
  if (!level_empty_) {
    output_ << '\n';
    Indent();
  }
  level_empty_ = false;
  output_ << '}';
}

void Writer::WriteArray(const Node *node) {
  output_ << '[';
  level_empty_ = true;
  ++indent_level_;
  for (const Node *value : *node->as_array().value()) {
    if (!level_empty_) output_ << ',';
    output_ << '\n';
    Indent();
    level_empty_ = false;
    Write(value);
  }
  --indent_level_;
  if (!level_empty_) {
    output_ << '\n';
    Indent();
  }
  level_empty_ = false;
  output_ << ']';
}

void Writer::WriteString(const Node *node) {
  output_ << '"' << node->as_string().value() << '"';
}

void Writer::WriteS64(const Node *node) {
  output_ << node->as_s64().value();
}

void Writer::WriteF64(const Node *node) {
  const f64 x = node->as_f64().value();
  output_ << x;
  if (fabs(fmod(x, 1.0)) == 0.0) output_ << ".0";
}

void Writer::WriteBoolean(const Node *node) {
  output_ << (node->as_bool().value() ? "true" : "false");
}

void Writer::WriteNull() { output_ << "null"; }

void Writer::Indent() { output_ << str(kIndentSize * indent_level_, ' '); }

}  // namespace rose::json
