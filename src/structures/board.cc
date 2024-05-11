#include "board.h"

#include <json.h>

#include "event.h"
#include "item.h"

namespace bee {

using namespace rose::json;

bool Board::MatchesStructure(const Node &node) {
  return structure()->Matches(node);
}

bool Board::NestedStructures::MatchesMetadata(const Node &node) {
  return metadata()->Matches(node);
}

bool Board::NestedStructures::MatchesLabels(const Node &node) {
  if (!node.is_object()) return false;
  for (const auto &[key, value] : *node.as_object().value()) {
    if (!value->is_s64()) return false;
  }
  return true;
}

bool Board::NestedStructures::MatchesFlags(const Node &node) {
  if (!node.is_array()) return false;
  for (const Node *value : *node.as_array().value()) {
    if (!value->is_string() || !StringNodeNotEmpty(*value)) return false;
  }
  return true;
}

bool Board::NestedStructures::MatchesItems(const Node &node) {
  return items()->Matches(node);
}

bool Board::NestedStructures::MatchesEvents(const Node &node) {
  return events()->Matches(node);
}

bool Board::NestedStructures::MatchesItemGenerators(const Node &node) {
  return item_generators()->Matches(node);
}

bool Board::NestedStructures::MatchesEventGenerators(const Node &node) {
  return event_generators()->Matches(node);
}

const ObjectStructure *Board::NestedStructures::metadata() {
  if (metadata_) return metadata_;
  auto *tmp = new ObjectStructure();
  tmp->AddRequiredProperty("board_bee_version", {Node::is_f64});
  tmp->AddRequiredProperty("name", {Node::is_string, StringNodeNotEmpty});
  tmp->AddOptionalProperty("desc", {Node::is_string});
  tmp->AddRequiredProperty("flags", {MatchesFlags});
  tmp->AddOptionalProperty("labels", {MatchesLabels});
  metadata_ = tmp;
  return metadata_;
}

const ArrayStructure *Board::NestedStructures::items() {
  if (items_) return items_;
  auto *tmp = new ArrayStructure();
  tmp->AddPredicate([](const Node &node) -> bool {
    for (const Node *value : *node.as_array().value()) {
      if (!Item::MatchesStructure(*value)) return false;
    }
    return true;
  });
  items_ = tmp;
  return items_;
}

const ArrayStructure *Board::NestedStructures::events() {
  if (events_) return events_;
  auto *tmp = new ArrayStructure();
  tmp->AddPredicate([](const Node &node) -> bool {
    for (const Node *value : *node.as_array().value()) {
      if (!Event::MatchesStructure(*value)) return false;
    }
    return true;
  });
  events_ = tmp;
  return events_;
}

const ObjectStructure *Board::structure() {
  if (structure_) return structure_;
  auto *tmp = new ObjectStructure();
  tmp->AddRequiredProperty("__metadata__", {NestedStructures::MatchesMetadata});
  tmp->AddRequiredProperty("items", {NestedStructures::MatchesItems});
  tmp->AddRequiredProperty("events", {NestedStructures::MatchesEvents});
  tmp->AddRequiredProperty("item_generators",
                           {NestedStructures::MatchesItemGenerators});
  tmp->AddRequiredProperty("event_generators",
                           {NestedStructures::MatchesEventGenerators});
  structure_ = tmp;
  return structure_;
}

}  // namespace bee
