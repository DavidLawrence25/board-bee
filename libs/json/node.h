#ifndef BOARD_BEE_LIBS_JSON_NODE_H_
#define BOARD_BEE_LIBS_JSON_NODE_H_

#include <libs/aliases.h>

#include <map>

namespace rose::json {

class Node;

using Object = std::map<str, Node *>;
using Array = std::vector<Node *>;

class Node {
 public:
  enum class Type { kObject, kArray, kString, kS64, kF64, kBool, kNull };
  union Values {
    Values() : boolean(false) {}
    ~Values() = default;

    Object *object;
    Array *array;
    const char *string;
    s64 n;
    f64 x;
    bool boolean;
  };

  Node() : type_(Type::kNull) {}
  explicit Node(const Object &object) : type_(Type::kObject) {
    value_.object = new Object(object);
  }
  explicit Node(const Array &array) : type_(Type::kArray) {
    value_.array = new Array(array);
  }
  explicit Node(const char *string) : type_(Type::kString) {
    value_.string = string;
  }
  explicit Node(const s64 n) : type_(Type::kS64) { value_.n = n; }
  explicit Node(const f64 x) : type_(Type::kF64) { value_.x = x; }
  explicit Node(const bool boolean) : type_(Type::kBool) {
    value_.boolean = boolean;
  }

  opt<Object *> as_object() const {
    return is_object() ? mk_opt<Object *>(value_.object) : std::nullopt;
  }
  opt<Array *> as_array() const {
    return is_array() ? mk_opt<Array *>(value_.array) : std::nullopt;
  }
  opt<const char *> as_string() const {
    return is_string() ? mk_opt<const char *>(value_.string) : std::nullopt;
  }
  opt<s64> as_s64() const {
    return is_s64() ? mk_opt<s64>(value_.n) : std::nullopt;
  }
  opt<f64> as_f64() const {
    return is_f64() ? mk_opt<f64>(value_.x) : std::nullopt;
  }
  opt<bool> as_bool() const {
    return is_bool() ? mk_opt<bool>(value_.boolean) : std::nullopt;
  }

  bool is_object() const { return type_ == Type::kObject; }
  bool is_array() const { return type_ == Type::kArray; }
  bool is_string() const { return type_ == Type::kString; }
  bool is_s64() const { return type_ == Type::kS64; }
  bool is_f64() const { return type_ == Type::kF64; }
  bool is_bool() const { return type_ == Type::kBool; }
  bool is_null() const { return type_ == Type::kNull; }

  void set_value(const Object &object) {
    type_ = Type::kObject;
    value_.object = new Object(object);
  }
  void set_value(const Array &array) {
    type_ = Type::kArray;
    value_.array = new Array(array);
  }
  void set_value(const char *string) {
    type_ = Type::kString;
    value_.string = string;
  }
  void set_value(const s64 n) {
    type_ = Type::kS64;
    value_.n = n;
  }
  void set_value(const f64 x) {
    type_ = Type::kF64;
    value_.x = x;
  }
  void set_value(const bool boolean) {
    type_ = Type::kBool;
    value_.boolean = boolean;
  }
  void set_value() { type_ = Type::kNull; }

 private:
  Type type_;
  Values value_;
};

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_NODE_H_
