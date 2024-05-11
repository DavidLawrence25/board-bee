#ifndef BOARD_BEE_LIBS_JSON_STRUCTURE_H_
#define BOARD_BEE_LIBS_JSON_STRUCTURE_H_

#include <functional>

#include "../aliases.h"
#include "node.h"

namespace rose::json {

using NodePredicate = std::function<bool(const Node &)>;

class Structure {
 public:
  virtual ~Structure() = default;

  virtual bool Matches(const Node &node) const = 0;
};

class ObjectStructure final : public Structure {
 public:
  using Property = vector<NodePredicate>;

  bool Matches(const Node &node) const override;

  void AddRequiredProperty(const char *key, const Property &property);
  void AddOptionalProperty(const char *key, const Property &property);

 private:
  static bool Matches(const Node &node, const Property &property) {
    for (const auto &predicate : property) {
      if (!predicate(node)) return false;
    }
    return true;
  }

  HashMap<const char *, Property> required_properties_;
  HashMap<const char *, Property> optional_properties_;
};

class ArrayStructure final : public Structure {
 public:
  bool Matches(const Node &node) const override {
    for (const auto &predicate : predicates_) {
      if (!predicate(node)) return false;
    }
    return true;
  }

  void AddPredicate(const NodePredicate &predicate);

 private:
  vector<NodePredicate> predicates_;
};

inline bool StringNodeNotEmpty(const Node &node) {
  return *node.as_string().value() != '\0';
}

}  // namespace rose::json

#endif  // BOARD_BEE_LIBS_JSON_STRUCTURE_H_
