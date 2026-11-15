#include <chrono>

#include <catch2/catch_test_macros.hpp>

#include "domain/period.h"

using namespace std::chrono;
using academy::Period;

TEST_CASE("daysInMonth handles 28/29/30/31", "[domain][calendar]") {
    REQUIRE(Period{year{2026}, November}.daysInMonth()  == 30);  // 30-day month
    REQUIRE(Period{year{2026}, December}.daysInMonth()  == 31);  // 31-day month
    REQUIRE(Period{year{2027}, February}.daysInMonth()  == 28);  // common year
    REQUIRE(Period{year{2028}, February}.daysInMonth()  == 29);  // leap year (FR-22)
}

TEST_CASE("contains is true only within the same month", "[domain][calendar]") {
    const Period nov{year{2026}, November};
    REQUIRE(nov.contains(year{2026}/November/15));
    REQUIRE_FALSE(nov.contains(year{2026}/December/1));   // next month
    REQUIRE_FALSE(nov.contains(year{2025}/November/15));  // same month, wrong year
}

TEST_CASE("Period comparisons come from operator<=>", "[domain]") {
    REQUIRE(Period{year{2026}, November} == Period{year{2026}, November});
    REQUIRE(Period{year{2026}, November} <  Period{year{2026}, December});
    REQUIRE(Period{year{2026}, December} >  Period{year{2026}, November});
}

TEST_CASE("daysInMonth is usable at compile time (constexpr)", "[domain]") {
    static_assert(Period{year{2028}, February}.daysInMonth() == 29);
    SUCCEED();
}
