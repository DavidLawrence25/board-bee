#ifndef BOARD_BEE_LIBS_JSON_NODE_H_
#define BOARD_BEE_LIBS_JSON_NODE_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <libs/aliases.h>

namespace rose::json {

class Node;

struct Property {
  const char *name;
  Node *value;

  bool operator<(const Property &other) const { return value < other.value; }
};

using namespace boost::multi_index;

using Object = multi_index_container<
    Property, indexed_by<
        ordered_unique<identity<Property>>,
        ordered_unique<member<Property, const char *, &Property::name>>
    >>;
using Array = std::vector<Node *>;

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
  explicit Node(const Array &array);
  explicit Node(const Object &object);

  opt<bool> as_bool() const;
  opt<s64> as_s64() const;
  opt<f64> as_f64() const;
  opt<const char *> as_string() const;
  opt<Array *> as_array() const;
  opt<Object *> as_object() const;

  bool is_object() const { return type_ == Type::kObject; }
  bool is_array() const { return type_ == Type::kArray; }
  bool is_string() const { return type_ == Type::kString; }
  bool is_s64() const { return type_ == Type::kS64; }
  bool is_f64() const { return type_ == Type::kF64; }
  bool is_bool() const { return type_ == Type::kBool; }
  bool is_null() const { return type_ == Type::kNull; }

  void set_value() { type_ = Type::kNull; }
  void set_value(bool boolean);
  void set_value(s64 n);
  void set_value(f64 x);
  void set_value(const char *string);
  void set_value(const Array &array);
  void set_value(const Object &object);

 private:
  Type type_;
  Values value_;
};

} // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_NODE_H_
