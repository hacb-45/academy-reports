#include <chrono>

#include <catch2/catch_test_macros.hpp>

#include "domain/teaching_calendar.h"

using namespace std::chrono;
using academy::Period;
using academy::TeachingCalendar;
using academy::AddDayResult;

TEST_CASE("a valid in-month day is added", "[domain][calendar]") {
    TeachingCalendar cal{Period{year{2026}, November}};
    REQUIRE(cal.addDay(year{2026}/November/3) == AddDayResult::Added);
    REQUIRE(cal.count() == 1);
    REQUIRE(cal.contains(year{2026}/November/3));
}

TEST_CASE("an impossible calendar date is rejected (FR-21)", "[domain][calendar]") {
    TeachingCalendar cal{Period{year{2026}, November}};
    REQUIRE(cal.addDay(year{2026}/November/31) == AddDayResult::InvalidDate);  // Nov has 30 days
    REQUIRE(cal.count() == 0);
}

TEST_CASE("a day outside the period's month is rejected (FR-21)", "[domain][calendar]") {
    TeachingCalendar cal{Period{year{2026}, November}};
    REQUIRE(cal.addDay(year{2026}/December/1) == AddDayResult::OutsideMonth);
    REQUIRE(cal.count() == 0);
}

TEST_CASE("a duplicate day is rejected (FR-21)", "[domain][calendar]") {
    TeachingCalendar cal{Period{year{2026}, November}};
    REQUIRE(cal.addDay(year{2026}/November/5) == AddDayResult::Added);
    REQUIRE(cal.addDay(year{2026}/November/5) == AddDayResult::Duplicate);  // second time
    REQUIRE(cal.count() == 1);
}

TEST_CASE("leap-day Feb 29 is accepted in a leap year (FR-22)", "[domain][calendar]") {
    TeachingCalendar cal{Period{year{2028}, February}};
    REQUIRE(cal.addDay(year{2028}/February/29) == AddDayResult::Added);
}

TEST_CASE("days() comes back sorted chronologically", "[domain][calendar]") {
    TeachingCalendar cal{Period{year{2026}, November}};
    cal.addDay(year{2026}/November/10);
    cal.addDay(year{2026}/November/3);
    cal.addDay(year{2026}/November/7);
    auto it = cal.days().begin();          // std::set iterates in sorted order
    REQUIRE(*it == year{2026}/November/3);  // earliest first
}
