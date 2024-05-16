#ifndef BOARD_BEE_LIBS_JSON_NODE_H_
#define BOARD_BEE_LIBS_JSON_NODE_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include "../aliases.h"

namespace rose::json {

class Node;

// Represents a simple JSON property with a key/name and value.
struct Property {
  const char *name;
  Node *value;

  // Compares the memory addresses of the two value nodes.
  bool operator<(const Property &other) const { return value < other.value; }
};

using namespace boost::multi_index;

// Represents a JSON object. Can mostly be used as a std::map, but uses
// boost::multi_index::multi_index_container to keep properties in order.
using Object = multi_index_container<
    Property, indexed_by<
        ordered_unique<identity<Property>>,
        ordered_unique<member<Property, const char *, &Property::name>>
    >>;
// Represents a JSON array.
using Array = std::vector<Node *>;

// Reperesents an arbitrary JSON node with a type and a value.
class Node {
 public:
  enum class Type { kNull, kBool, kS64, kF64, kString, kArray, kObject };
  union Values {
    Values() : boolean(false) {}
    ~Values() = default;

    bool boolean;
    s64 n;
    f64 x;
    const char *string;
    Array *array;
    Object *object;
  };

  Node() : type_(Type::kNull) {}
  explicit Node(bool boolean);
  explicit Node(s64 n);
  explicit Node(f64 x);
  explicit Node(const char *string);
  // Makes a copy of `array` on the heap.
  explicit Node(const Array &array);
  // Makes a copy of `object` on the heap.
  explicit Node(const Object &object);

  // Returns a human-readable string to represent the type of this node.
  // Can throw if `type_` is invalid, but that generally shouldn't happen.
  const char *type_name() const;

  // Returns the value of this node expressed as a boolean.
  // Returns std::nullopt if the node doesn't represent a boolean.
  opt<bool> as_bool() const noexcept;
  // Returns the value of this node expressed as an s64.
  // Returns std::nullopt if the node doesn't represent an s64.
  opt<s64> as_s64() const noexcept;
  // Returns the value of this node expressed as an f64.
  // Returns std::nullopt if the node doesn't represent an f64.
  opt<f64> as_f64() const noexcept;
  // Returns the value of this node expressed as a C-string.
  // Returns std::nullopt if the node doesn't represent a C-string.
  opt<const char *> as_string() const noexcept;
  // Returns the value of this node expressed as an array.
  // Returns std::nullopt if the node doesn't represent an array.
  opt<Array *> as_array() const noexcept;
  // Returns the value of this node expressed as an object.
  // Returns std::nullopt if the node doesn't represent an object.
  opt<Object *> as_object() const noexcept;

  bool is_object() const noexcept { return type_ == Type::kObject; }
  bool is_array() const noexcept { return type_ == Type::kArray; }
  bool is_string() const noexcept { return type_ == Type::kString; }
  bool is_s64() const noexcept { return type_ == Type::kS64; }
  bool is_f64() const noexcept { return type_ == Type::kF64; }
  bool is_bool() const noexcept { return type_ == Type::kBool; }
  bool is_null() const noexcept { return type_ == Type::kNull; }

  // Sets this node's value and type to null.
  void set_value() noexcept { type_ = Type::kNull; }
  void set_value(bool boolean) noexcept;
  void set_value(s64 n) noexcept;
  void set_value(f64 x) noexcept;
  void set_value(const char *string) noexcept;
  // Sets this node's value to a copy of `array` on the heap.
  void set_value(const Array &array);
  // Sets this node's value to a copy of `object` on the heap.
  void set_value(const Object &object);

 private:
  Type type_;
  Values value_;
};

} // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_NODE_H_
