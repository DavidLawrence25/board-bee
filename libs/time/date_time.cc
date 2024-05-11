#include "date_time.h"

#include <cstring>
#include <regex>
#include <sstream>

#include "exceptions.h"

namespace rose::time {

DateTime::DateTime(const str &iso_string) {
  std::smatch match_results;
  if (!std::regex_match(iso_string, match_results, kSimpleIso8601)) {
    std::stringstream error_msg;
    error_msg << "String \"" << iso_string
              << "\" is not compliant with ISO 8601";
    throw BadDateTimeException(error_msg.str());
  }
  year_ = atoi(match_results[1].str().c_str());
  month_ = atoi(match_results[2].str().c_str());
  if (month_ == 0 || month_ > 12) {
    std::stringstream error_msg;
    error_msg << "Month " << month_ << " is not on the interval [1, 12]";
    throw BadDateTimeException(error_msg.str());
  }
  day_ = atoi(match_results[3].str().c_str());
  const u8 days_in_month = DaysInMonth(year_, month_);
  if (day_ == 0 || day_ > days_in_month) {
    std::stringstream error_msg;
    error_msg << "Day " << day_ << " is not on the interval [1, "
              << days_in_month << ']';
    throw BadDateTimeException(error_msg.str());
  }
  hour_ = atoi(match_results[4].str().c_str());
  if (hour_ > 23) {
    std::stringstream error_msg;
    error_msg << "Hour " << hour_ << " is not on the interval [0, 23]";
    throw BadDateTimeException(error_msg.str());
  }
  minute_ = atoi(match_results[5].str().c_str());
  if (minute_ > 59) {
    std::stringstream error_msg;
    error_msg << "Minute " << minute_ << " is not on the interval [0, 59]";
    throw BadDateTimeException(error_msg.str());
  }
  second_ = atoi(match_results[6].str().c_str());
  if (second_ > 60) {
    std::stringstream error_msg;
    error_msg << "Second " << second_ << " is not on the interval [0, 60]";
    throw BadDateTimeException(error_msg.str());
  }
  if (hour_ != 23 || minute_ != 59 || second_ != 60 || HasLeapSecond()) return;
  std::stringstream error_msg;
  error_msg << NiceDateString() << " did not have a leap second";
  throw BadDateTimeException(error_msg.str());
}

bool DateTime::IsValidDateTime(const str &string) {
  try {
    static_cast<void>(DateTime(string));
  } catch (const BadDateTimeException &_) {
    return false;
  }
  return true;
}

bool DateTime::IsLeapYear(const u16 year) {
  const bool a = year % 4;
  const bool b = year % 100;
  const bool c = year % 400;
  return !(a || b ^ c);
}

bool DateTime::HasLeapSecond(const u16 year, const u8 month, const u8 day) {
  const bool is_jun_30 = month == 6 && day == 30;
  const bool is_dec_31 = month == 12 && day == 31;
  if (!(is_jun_30 || is_dec_31)) return false;
  if (year < 1972 || year > 2016) return false;
  if (year == 1972) return true;
  if (year < 1980) return is_dec_31;
  if (year == 1980 || year == 1984 || year == 1986) return false;
  if (year < 1986) return is_jun_30;
  if (year == 1988 || year == 1991) return false;
  if (year < 1991) return is_dec_31;
  if (year < 1995) return is_jun_30;
  if (year == 1995) return is_dec_31;
  if (year == 1996) return false;
  if (year == 1997) return is_jun_30;
  if (year == 1998) return is_dec_31;
  if (year == 2005 || year == 2008 || year == 2016) return is_dec_31;
  if (year == 2012 || year == 2015) return is_jun_30;
  return false;
}

u8 DateTime::DaysInMonth(const u16 year, const u8 month) {
  if (month == 0 || month > 12) return 0;
  if (month == 2) return IsLeapYear(year) ? 29 : 28;
  const u8 kDays[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return kDays[month - 1];
}

str DateTime::DateString() const {
  char out[9] = "00000000";
  out[0] += year_ / 1000;          // NOLINT(*-narrowing-conversions)
  out[1] += (year_ % 1000) / 100;  // NOLINT(*-narrowing-conversions)
  out[2] += (year_ % 100) / 10;    // NOLINT(*-narrowing-conversions)
  out[3] += year_ % 10;            // NOLINT(*-narrowing-conversions)
  const auto &[month_quotient, month_remainder] = std::div(month_, 10);
  out[4] += month_quotient;   // NOLINT(*-narrowing-conversions)
  out[5] += month_remainder;  // NOLINT(*-narrowing-conversions)
  const auto &[day_quotient, day_remainder] = std::div(day_, 10);
  out[6] += day_quotient;   // NOLINT(*-narrowing-conversions)
  out[7] += day_remainder;  // NOLINT(*-narrowing-conversions)
  return out;
}

str DateTime::NiceDateString() const {
  char out[11] = "0000-00-00";
  out[0] += year_ / 1000;          // NOLINT(*-narrowing-conversions)
  out[1] += (year_ % 1000) / 100;  // NOLINT(*-narrowing-conversions)
  out[2] += (year_ % 100) / 10;    // NOLINT(*-narrowing-conversions)
  out[3] += year_ % 10;            // NOLINT(*-narrowing-conversions)
  const auto &[month_quotient, month_remainder] = std::div(month_, 10);
  out[5] += month_quotient;   // NOLINT(*-narrowing-conversions)
  out[6] += month_remainder;  // NOLINT(*-narrowing-conversions)
  const auto &[day_quotient, day_remainder] = std::div(day_, 10);
  out[8] += day_quotient;   // NOLINT(*-narrowing-conversions)
  out[9] += day_remainder;  // NOLINT(*-narrowing-conversions)
  return out;
}

str DateTime::TimeString() const {
  char out[9] = "T000000Z";
  const auto &[hour_quotient, hour_remainder] = std::div(hour_, 10);
  out[1] += hour_quotient;   // NOLINT(*-narrowing-conversions)
  out[2] += hour_remainder;  // NOLINT(*-narrowing-conversions)
  const auto &[minute_quotient, minute_remainder] = std::div(minute_, 10);
  out[3] += minute_quotient;   // NOLINT(*-narrowing-conversions)
  out[4] += minute_remainder;  // NOLINT(*-narrowing-conversions)
  const auto &[second_quotient, second_remainder] = std::div(second_, 10);
  out[5] += second_quotient;   // NOLINT(*-narrowing-conversions)
  out[6] += second_remainder;  // NOLINT(*-narrowing-conversions)
  return out;
}

str DateTime::NiceTimeString() const {
  char out[11] = "T00:00:00Z";
  const auto &[hour_quotient, hour_remainder] = std::div(hour_, 10);
  out[1] += hour_quotient;   // NOLINT(*-narrowing-conversions)
  out[2] += hour_remainder;  // NOLINT(*-narrowing-conversions)
  const auto &[minute_quotient, minute_remainder] = std::div(minute_, 10);
  out[4] += minute_quotient;   // NOLINT(*-narrowing-conversions)
  out[5] += minute_remainder;  // NOLINT(*-narrowing-conversions)
  const auto &[second_quotient, second_remainder] = std::div(second_, 10);
  out[7] += second_quotient;   // NOLINT(*-narrowing-conversions)
  out[8] += second_remainder;  // NOLINT(*-narrowing-conversions)
  return out;
}

str DateTime::AsString() const {
  char out[17];
  strncpy(out, DateString().c_str(), 8);
  strncat(out, TimeString().c_str(), 8);
  return out;
}

str DateTime::AsNiceString() const {
  char out[21];
  strncpy(out, NiceDateString().c_str(), 10);
  strncat(out, NiceTimeString().c_str(), 10);
  return out;
}

}  // namespace bee
