#include "event.h"

#include <aliases.h>
#include <json.h>
#include <rose_time.h>

namespace bee {

using namespace rose::json;
using namespace rose::time;

bool Event::Dates::MatchesStructure(const Node &node) {
  return structure()->Matches(node);
}

const ObjectStructure *Event::Dates::structure() {
  if (structure_) return structure_;
  auto *tmp = new ObjectStructure();
  auto string_node_date_time = [](const Node &node) -> bool {
    return DateTime::IsValidDateTime(node.as_string().value());
  };
  tmp->AddRequiredProperty("start", {Node::is_string, string_node_date_time});
  tmp->AddRequiredProperty("end", {Node::is_string, string_node_date_time});
  structure_ = tmp;
  return structure_;
}

bool Event::MatchesStructure(const Node &node) {
  return structure()->Matches(node);
}

const ObjectStructure *Event::structure() {
  if (structure_) return structure_;
  auto *tmp = new ObjectStructure();
  tmp->AddRequiredProperty("name", {Node::is_string, StringNodeNotEmpty});
  tmp->AddRequiredProperty("dates", {Dates::MatchesStructure});
  structure_ = tmp;
  return structure_;
}

}  // namespace bee
