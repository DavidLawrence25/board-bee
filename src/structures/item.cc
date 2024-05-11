#include "item.h"

#include <aliases.h>
#include <json.h>
#include <rose_time.h>

#include "flags.h"

namespace bee {

using namespace rose::json;
using namespace rose::time;

bool Item::Dates::MatchesStructure(const Node &node) {
  return structure()->Matches(node);
}

const ObjectStructure *Item::Dates::structure() {
  if (structure_) return structure_;
  auto *tmp = new ObjectStructure();
  auto string_node_date_time = [](const Node &node) -> bool {
    return DateTime::IsValidDateTime(node.as_string().value());
  };
  tmp->AddOptionalProperty("start_by",
                           {Node::is_string, string_node_date_time});
  tmp->AddRequiredProperty("finish_by",
                           {Node::is_string, string_node_date_time});
  tmp->AddRequiredProperty("due", {Node::is_string, string_node_date_time});
  structure_ = tmp;
  return structure_;
}

bool Item::MatchesStructure(const Node &node) {
  return structure()->Matches(node);
}

const ObjectStructure *Item::structure() {
  if (structure_) return structure_;
  auto *tmp = new ObjectStructure();
  auto string_node_not_empty = [](const Node &node) -> bool {
    return *node.as_string().value() != '\0';
  };
  auto string_node_valid_label = [](const Node &node) -> bool {
    return IsLabelValid(node.as_string().value());
  };
  auto f64_node_on_0_to_1 = [](const Node &node) -> bool {
    const f64 x = node.as_f64().value();
    return x >= 0.0 && x <= 1.0;
  };
  tmp->AddRequiredProperty("name", {Node::is_string, string_node_not_empty});
  tmp->AddOptionalProperty("desc", {Node::is_string});
  tmp->AddOptionalProperty("label", {Node::is_string, string_node_valid_label});
  tmp->AddRequiredProperty("flags", {Flags::MatchesStructure});
  tmp->AddOptionalProperty("dates", {Dates::MatchesStructure});
  tmp->AddOptionalProperty("completion", {Node::is_f64, f64_node_on_0_to_1});
  tmp->AddOptionalProperty("checklist", {/*???*/});
  structure_ = tmp;
  return structure_;
}

void Item::set_valid_labels(const HashMap<str, s32> *valid_labels) {
  valid_labels_ = valid_labels;
  structure_ = nullptr;
}

bool Item::IsLabelValid(const str_view label) {
  return valid_labels_->contains(label.data());
}

opt<s32> Item::LabelValue(const str_view label) {
  return IsLabelValid(label) ? mk_opt<s32>(valid_labels_->at(label.data()))
                             : std::nullopt;
}

}  // namespace bee
