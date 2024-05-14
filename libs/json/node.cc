#include "node.h"

#include <sstream>

#include "exceptions.h"

namespace rose::json {

Node::Node(const bool boolean) : type_(Type::kBool) {
  value_.boolean = boolean;
}

Node::Node(const s64 n) : type_(Type::kS64) {
  value_.n = n;
}

Node::Node(const f64 x) : type_(Type::kF64) {
  value_.x = x;
}

Node::Node(const char *string) : type_(Type::kString) {
  value_.string = string;
}

Node::Node(const Array &array) : type_(Type::kArray) {
  value_.array = new Array(array);
}

Node::Node(const Object &object) : type_(Type::kObject) {
  value_.object = new Object(object);
}

const char *Node::type_name() const {
  switch (type_) {
    case Type::kNull: return "null";
    case Type::kBool: return "bool";
    case Type::kS64: return "s64";
    case Type::kF64: return "f64";
    case Type::kString: return "str";
    case Type::kArray: return "Array";
    case Type::kObject: return "Object";
  }
  std::stringstream error_msg;
  error_msg << "Tried to get the name of type " << static_cast<s64>(type_);
  throw UndefinedTypeError(error_msg.str());
}

opt<bool> Node::as_bool() const {
  return is_bool() ? mk_opt<bool>(value_.boolean) : std::nullopt;
}

opt<s64> Node::as_s64() const {
  return is_s64() ? mk_opt<s64>(value_.n) : std::nullopt;
}

opt<f64> Node::as_f64() const {
  return is_f64() ? mk_opt<f64>(value_.x) : std::nullopt;
}

opt<const char *> Node::as_string() const {
  return is_string() ? mk_opt<const char *>(value_.string) : std::nullopt;
}

opt<Array *> Node::as_array() const {
  return is_array() ? mk_opt<Array *>(value_.array) : std::nullopt;
}

opt<Object *> Node::as_object() const {
  return is_object() ? mk_opt<Object *>(value_.object) : std::nullopt;
}

void Node::set_value(const bool boolean) {
  type_ = Type::kBool;
  value_.boolean = boolean;
}

void Node::set_value(const s64 n) {
  type_ = Type::kS64;
  value_.n = n;
}

void Node::set_value(const f64 x) {
  type_ = Type::kF64;
  value_.x = x;
}

void Node::set_value(const char *string) {
  type_ = Type::kString;
  value_.string = string;
}

void Node::set_value(const Array &array) {
  type_ = Type::kArray;
  value_.array = new Array(array);
}

void Node::set_value(const Object &object) {
  type_ = Type::kObject;
  value_.object = new Object(object);
}

}  // namespace rose::json
