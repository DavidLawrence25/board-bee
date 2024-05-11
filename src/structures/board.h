#ifndef BOARD_BEE_SRC_STRUCTURES_BOARD_H_
#define BOARD_BEE_SRC_STRUCTURES_BOARD_H_

#include <aliases.h>
#include <json.h>

#include <set>

#include "event.h"
#include "item.h"

namespace bee {

class Board {
 public:
  static Board FromJson(const rose::json::Node &node);

  Board(const f64 version, const str_view name)
      : version_(version), name_(name) {}

  static bool MatchesStructure(const rose::json::Node &node);
  rose::json::Node ToJson() const;

 private:
  class NestedStructures {
   public:
    static bool MatchesMetadata(const rose::json::Node &node);
    static bool MatchesLabels(const rose::json::Node &node);
    static bool MatchesFlags(const rose::json::Node &node);
    static bool MatchesItems(const rose::json::Node &node);
    static bool MatchesEvents(const rose::json::Node &node);
    static bool MatchesItemGenerators(const rose::json::Node &node);
    static bool MatchesEventGenerators(const rose::json::Node &node);

    static const rose::json::ObjectStructure *metadata();
    static const rose::json::ArrayStructure *items();
    static const rose::json::ArrayStructure *events();
    static const rose::json::ArrayStructure *item_generators();
    static const rose::json::ArrayStructure *event_generators();

   private:
    inline static const rose::json::ObjectStructure *metadata_ = nullptr;
    inline static const rose::json::ArrayStructure *items_ = nullptr;
    inline static const rose::json::ArrayStructure *events_ = nullptr;
    inline static const rose::json::ArrayStructure *item_generators_ = nullptr;
    inline static const rose::json::ArrayStructure *event_generators_ = nullptr;
  };

  static const rose::json::ObjectStructure *structure();

  f64 version_;
  str name_;
  opt<str> desc_;
  HashMap<str, s32> labels_;
  std::set<str> flags_;
  vector<Item> items_;
  vector<Event> events_;
  // vector<ItemGenerator> item_generators_;
  // vector<EventGenerator> event_generators_;
  inline static const rose::json::ObjectStructure *structure_ = nullptr;
};

}  // namespace bee

#endif  // BOARD_BEE_SRC_STRUCTURES_BOARD_H_
