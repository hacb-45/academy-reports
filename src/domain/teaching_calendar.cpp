#include "domain/teaching_calendar.h"

namespace academy {

TeachingCalendar::TeachingCalendar(Period p) : period_{p} {}

AddDayResult TeachingCalendar::addDay(std::chrono::year_month_day d) {
  if (!d.ok()) {
    return AddDayResult::InvalidDate;
  }
  if (!period_.contains(d)) {
    return AddDayResult::OutsideMonth;
  }
  auto [it, inserted] = days_.insert(d);
  if (!inserted) {
    return AddDayResult::Duplicate;
  }
  return AddDayResult::Added;
}

bool TeachingCalendar::contains(std::chrono::year_month_day d) const {
  return days_.contains(d);
}

std::size_t TeachingCalendar::count() const noexcept { return days_.size(); }

const std::set<std::chrono::year_month_day>& TeachingCalendar::days()
    const noexcept {
  return days_;
}

}  // namespace academy
