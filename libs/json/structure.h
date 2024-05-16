#ifndef BOARD_BEE_LIBS_JSON_STRUCTURE_H_
#define BOARD_BEE_LIBS_JSON_STRUCTURE_H_

#include <functional>

#include "../aliases.h"
#include "node.h"

namespace rose::json {

// Function which returns true if the given Node satisfies a certain condition.
using NodePredicate = std::function<bool(const Node &)>;

// Abstract structure of a Node.
class Structure {
 public:
  virtual ~Structure() = default;

  // Returns true if `node` matches all given predicates.
  virtual bool Matches(const Node &node) const = 0;
};

// Minimal required structure of an Object Node.
// Used to determine whether a given Node can be converted to a struct.
class ObjectStructure final : public Structure {
 public:
  using Property = vector<NodePredicate>;

  // Returns true if `node` is an Object with this structure.
  bool Matches(const Node &node) const override;

  // Adds a required property named `key`.
  // A matching Node must have a property named `key` that
  // matches the predicates specified in `property`.
  void AddRequiredProperty(const char *key, const Property &property);
  // Adds an optional property named `key`.
  // Nodes will match if they lack a property named `key`, have `key`
  // mapped to null, or satisfy all predicates specified in `property`.
  void AddOptionalProperty(const char *key, const Property &property);

 private:
  // Returns true if `node` satisfies all predicates specified in `property`.
  static bool Matches(const Node &node, const Property &property) {
    for (const auto &predicate : property) {
      if (!predicate(node)) return false;
    }
    return true;
  }

  // Required keys with a set of predicates that must be satisfied.
  HashMap<const char *, Property> required_properties_;
  // Optional keys with a set of predicates that must be satisfied
  // if the key is present and not mapped to null.
  HashMap<const char *, Property> optional_properties_;
};

// Minimal required structure of an Array Node.
// Used to determine whether a given Node can be converted to some container.
class ArrayStructure final : public Structure {
 public:
  // Returns true if `node` satisfies all predicates.
  bool Matches(const Node &node) const override {
    for (const auto &predicate : predicates_) {
      if (!predicate(node)) return false;
    }
    return true;
  }

  // Adds `predicate` to the vector of required predicates.
  void AddPredicate(const NodePredicate &predicate);
  // Adds `predicate` to the vector of required predicates.
  ArrayStructure &operator+=(const NodePredicate &predicate);

 private:
  vector<NodePredicate> predicates_;
};

// Returns true if `node` contains an empty string.
// Assumes type of `node` is a string for simplicity.
inline bool StringNodeNotEmpty(const Node &node) {
  return *node.as_string().value() != '\0';
}

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_STRUCTURE_H_
