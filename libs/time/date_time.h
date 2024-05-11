#ifndef BOARD_BEE_LIBS_TIME_DATE_TIME_H_
#define BOARD_BEE_LIBS_TIME_DATE_TIME_H_

#include <aliases.h>
#include <json.h>

#include <regex>

namespace rose::time {

static const std::regex kSimpleIso8601(
    R"~~(^(\d{4})-([0-1]\d)-([0-3]\d)T([0-2]\d):([0-5]\d):([0-6]\d)Z$)~~");

class DateTime {
 public:
  static DateTime FromJson(const json::Node &node);

  explicit DateTime(const str &iso_string);

  u16 year() const { return year_; }
  u8 month() const { return month_; }
  u8 day() const { return day_; }
  u8 hour() const { return hour_; }
  u8 minute() const { return minute_; }
  u8 second() const { return second_; }

  static bool IsValidDateTime(const str &string);
  static bool IsLeapYear(u16 year);
  static bool HasLeapSecond(u16 year, u8 month, u8 day);
  static u8 DaysInMonth(u16 year, u8 month);
  bool IsLeapYear() const { return IsLeapYear(year_); }
  bool HasLeapSecond() const { return HasLeapSecond(year_, month_, day_); }
  u8 DaysInMonth() const { return DaysInMonth(year_, month_); }
  json::Node ToJson() const;

  str DateString() const;
  str NiceDateString() const;
  str TimeString() const;
  str NiceTimeString() const;
  str AsString() const;
  str AsNiceString() const;

 private:
  u16 year_ : 14;
  u8 month_ : 4;
  u8 day_ : 5;
  u8 hour_ : 5;
  u8 minute_ : 6;
  u8 second_ : 6;
};

}  // namespace rose::time

#endif  // BOARD_BEE_LIBS_TIME_DATE_TIME_H_
