#ifndef BOARD_BEE_SRC_STRUCTURES_FLAGS_H_
#define BOARD_BEE_SRC_STRUCTURES_FLAGS_H_

#include <aliases.h>
#include <json.h>

#include <set>

namespace bee {

class Flags {
 public:
  static Flags FromJson(const rose::json::Node &node);

  static void set_valid_flags(const std::set<str> *valid_flags);

  static bool MatchesStructure(const rose::json::Node &node);
  rose::json::Node ToJson() const;

 private:
  static const rose::json::ObjectStructure *structure();

  HashMap<str, bool> data_;
  inline static const std::set<str> *valid_flags_ = nullptr;
  inline static const rose::json::ObjectStructure *structure_ = nullptr;
};

}  // namespace bee

#endif  // BOARD_BEE_SRC_STRUCTURES_FLAGS_H_
