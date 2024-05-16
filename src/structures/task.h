#ifndef BOARD_BEE_SRC_STRUCTURES_TASK_H_
#define BOARD_BEE_SRC_STRUCTURES_TASK_H_

#include <aliases.h>
#include <json.h>
#include <rose_time.h>

#include "flags.h"

namespace bee {

class Task {
 public:
  class Dates {
   public:
    Dates(const rose::time::DateTime finish_by, const rose::time::DateTime due)
        : finish_by_(finish_by), due_(due) {}
    Dates(const rose::time::DateTime start_by,
          const rose::time::DateTime finish_by, const rose::time::DateTime due)
        : start_by_(start_by), finish_by_(finish_by), due_(due) {}

    opt<rose::time::DateTime> start_by() const { return start_by_; }
    rose::time::DateTime finish_by() const { return finish_by_; }
    rose::time::DateTime due() const { return due_; }

    static bool MatchesStructure(const rose::json::Node &node);

   private:
    static const rose::json::ObjectStructure *structure();

    opt<rose::time::DateTime> start_by_;
    rose::time::DateTime finish_by_;
    rose::time::DateTime due_;
    inline static const rose::json::ObjectStructure *structure_ = nullptr;
  };

  static Task FromJson(const rose::json::Node &node);

  str_view name() const { return name_; }
  opt<str_view> desc() const { return desc_; }
  opt<s32> label() const { return label_; }
  Flags flags() const { return flags_; }
  static void set_valid_labels(const HashMap<str, s32> *valid_labels);

  static bool IsLabelValid(str_view label);
  static opt<s32> LabelValue(str_view label);
  static bool MatchesStructure(const rose::json::Node &node);
  rose::json::Node ToJson() const;

 private:
  static const rose::json::ObjectStructure *structure();

  str name_;
  opt<str> desc_;
  opt<s32> label_;
  Flags flags_;
  inline static const HashMap<str, s32> *valid_labels_ = nullptr;
  inline static const rose::json::ObjectStructure *structure_ = nullptr;
};

}  // namespace bee

#endif  // BOARD_BEE_SRC_STRUCTURES_TASK_H_
