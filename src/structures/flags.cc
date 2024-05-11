#include "flags.h"

namespace bee {

using namespace rose::json;

void Flags::set_valid_flags(const std::set<str> *valid_flags) {
  valid_flags_ = valid_flags;
}

bool Flags::MatchesStructure(const Node &node) {
  return structure()->Matches(node);
}

const ObjectStructure *Flags::structure() {
  if (structure_) return structure_;
  auto *tmp = new ObjectStructure();
  if (valid_flags_) {
    for (const str &flag : *valid_flags_) {
      tmp->AddRequiredProperty(flag.c_str(), {Node::is_bool});
    }
  }
  structure_ = tmp;
  return structure_;
}

}  // namespace bee
