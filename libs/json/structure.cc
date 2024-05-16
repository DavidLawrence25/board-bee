#include "structure.h"

namespace rose::json {

bool ObjectStructure::Matches(const Node &node) const {
  if (!node.is_object()) return false;
  u64 required_properties_found = 0;
  for (const auto &[key, value] : *node.as_object().value()) {
    auto it = required_properties_.find(key);
    if (it != required_properties_.cend()) {
      if (!Matches(*value, it->second)) return false;
      ++required_properties_found;
      continue;
    }
    it = optional_properties_.find(key);
    // If we were to add support for arbitary keys (for some reason),
    // this if-statement would be much more complicated.
    if (it == required_properties_.cend()) return false;
    if (!value->is_null() && !Matches(*value, it->second)) return false;
  }
  return required_properties_found == required_properties_.size();
}

void ObjectStructure::AddRequiredProperty(const char *key,
                                          const Property &property) {
  required_properties_.emplace(key, property);
}

void ObjectStructure::AddOptionalProperty(const char *key,
                                          const Property &property) {
  optional_properties_.emplace(key, property);
}

void ArrayStructure::AddPredicate(const NodePredicate &predicate) {
  predicates_.push_back(predicate);
}

ArrayStructure &ArrayStructure::operator+=(const NodePredicate &predicate) {
  predicates_.push_back(predicate);
  return *this;
}

}  // namespace rose::json
