#include <algorithm>
#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "util/version.h"

TEST_CASE("version string is not empty", "[util]") {
    REQUIRE_FALSE(academy::versionString().empty());
}

TEST_CASE("version string has major.minor.patch form", "[util]") {
    const auto v = academy::versionString();
    REQUIRE(std::count(v.begin(), v.end(), '.') == 2);
}

TEST_CASE("check that each part between the dots is a number", "[util]") {
    const auto v = academy::versionString();

    auto dot1 = v.find('.');
    REQUIRE(dot1 != std::string_view::npos);  
    auto dot2 = v.find('.', dot1 + 1);
    REQUIRE(dot2 != std::string_view::npos);

    auto major = v.substr(0, dot1);
    auto minor = v.substr(dot1 + 1, dot2 - dot1 - 1);
    auto patch = v.substr(dot2 + 1);

    auto isNumber = [](std::string_view s) {
        return !s.empty() &&
               std::all_of(s.begin(), s.end(),
                           [](char c) { return c >= '0' && c <= '9'; });
    };

    REQUIRE(isNumber(major));
    REQUIRE(isNumber(minor));
    REQUIRE(isNumber(patch));
}
