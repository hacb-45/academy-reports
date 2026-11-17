#pragma once
#include <chrono>
#include <cstddef>
#include <set>

#include "domain/period.h"

namespace academy {

// Outcome of trying to add a teaching day. This is "errors as data": instead of
// throwing, addDay() RETURNS why it refused, and the caller decides what to do.
// (In M2 this graduates to a richer std::variant outcome.)
enum class AddDayResult {
    Added,         // success
    InvalidDate,   // e.g. 2026-11-31 -- no such calendar day (FR-21)
    OutsideMonth,  // valid date, but not in this period's month (FR-21)
    Duplicate      // already registered (FR-21)
};

// The set of days a class actually met, within one reporting period.
//
// Unlike Period, this type is NOT constexpr and owns a container, so its
// methods are DECLARED here and DEFINED in teaching_calendar.cpp -- the real
// header/source split (ODR: one definition, in the .cpp).
class TeachingCalendar {
public:
    explicit TeachingCalendar(Period p);

    // Try to register `d` as a teaching day. See AddDayResult for outcomes.
    AddDayResult addDay(std::chrono::year_month_day d);

    [[nodiscard]] bool contains(std::chrono::year_month_day d) const;
    [[nodiscard]] std::size_t count() const noexcept;

    // Read-only view of the days, kept sorted (std::set). ISO dates sort
    // chronologically, so iterating this is the class's meetings in order.
    [[nodiscard]] const std::set<std::chrono::year_month_day>& days() const noexcept;

private:
    Period period_;
    std::set<std::chrono::year_month_day> days_;
};

}  // namespace academy
