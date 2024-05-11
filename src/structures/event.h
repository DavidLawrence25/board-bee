#ifndef BOARD_BEE_SRC_STRUCTURES_EVENT_H_
#define BOARD_BEE_SRC_STRUCTURES_EVENT_H_

#include <aliases.h>
#include <json.h>

#include "date_time.h"

namespace bee {

class Event {
 public:
  class Dates {
   public:
    Dates(const DateTime start, const DateTime end) : start_(start),
                                                      end_(end) {}

    DateTime start() const { return start_; }
    DateTime end() const { return end_; }

    static bool MatchesStructure(const rose::json::Node &node);

   private:
    static const rose::json::ObjectStructure *structure();

    DateTime start_;
    DateTime end_;
    inline static const rose::json::ObjectStructure *structure_ = nullptr;
  };

  static Event FromJson(const rose::json::Node &node);

  Event(const str_view name, const Dates dates) : name_(name), dates_(dates) {}

  static bool MatchesStructure(const rose::json::Node &node);
  rose::json::Node ToJson() const;

 private:
  static const rose::json::ObjectStructure *structure();

  str name_;
  Dates dates_;
  inline static const rose::json::ObjectStructure *structure_ = nullptr;
};

}  // namespace bee

#endif  // BOARD_BEE_SRC_STRUCTURES_EVENT_H_
