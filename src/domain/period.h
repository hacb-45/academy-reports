#pragma once
#include <chrono>
#include <stdexcept>

namespace academy {

// A reporting period = one calendar month, e.g. November 2026.
//
// This type is `constexpr`-capable and owns nothing but a year_month value,
// so it needs no destructor, copy, or move code (Rule of 0).
//
// NOTE: the whole class lives in this header on purpose. A `constexpr` function
// is implicitly `inline`, and to be usable in a constant expression from
// another .cpp the compiler must SEE its body -- so constexpr types are
// header-only. (The .h/.cpp split you practise in TeachingCalendar instead.)
class Period {
public:

    constexpr Period(std::chrono::year y, std::chrono::month m)
        : ym_{y, m} {
            if (!m.ok()) throw std::invalid_argument("bad month");
        // TODO: if (!m.ok()) throw ...   (need <stdexcept>)
    }

    [[nodiscard]] constexpr std::chrono::year  year()  const noexcept { return ym_.year(); }
    [[nodiscard]] constexpr std::chrono::month month() const noexcept { return ym_.month(); }

    // Number of days in this month (28/29/30/31), leap years handled.
    [[nodiscard]] constexpr unsigned daysInMonth() const noexcept {
        std::chrono::year_month_day_last last{ ym_.year(), std::chrono::month_day_last{ ym_.month()}};
        unsigned n = static_cast<unsigned>(last.day());
        return n;
    }

    // True if `d` falls inside this month (same year AND same month).
    [[nodiscard]] constexpr bool contains(std::chrono::year_month_day d) const noexcept {
        return d.year() == ym_.year() && d.month() == ym_.month();
    }

    // Defaulted three-way comparison gives ==, !=, <, <=, >, >= for free.
    auto operator<=>(const Period&) const = default;

private:
    std::chrono::year_month ym_;
};

}  // namespace academy
