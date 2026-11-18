#include <string>
#include <variant>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "validation/name_matcher.h"

using namespace academy;

namespace {
const std::vector<RosterEntry> kRoster{
    {1, "Ana Ribeiro"},
    {2, "Miguel Costa"},
    {3, "Ana Maria Sousa"},
};
}  // namespace

TEST_CASE("exact match (after normalization) accepts", "[validation]") {
  auto out = matchName("  ana   RIBEIRO ", 1, kRoster);
  REQUIRE(std::holds_alternative<Accepted>(out));
  REQUIRE(std::get<Accepted>(out).id == 1);
}

TEST_CASE("middle name -> single fuzzy candidate warns", "[validation]") {
  auto out = matchName("Ana Maria Ribeiro", 2, kRoster);   // first=ana last=ribeiro -> id 1 only
  REQUIRE(std::holds_alternative<Warned>(out));
  REQUIRE(std::get<Warned>(out).id == 1);
}

TEST_CASE("unknown name is rejected", "[validation]") {
  auto out = matchName("Joao Silva", 7, kRoster);
  REQUIRE(std::holds_alternative<Rejected>(out));
  REQUIRE(std::get<Rejected>(out).row == 7);
}

TEST_CASE("ambiguous fuzzy match is rejected", "[validation]") {
  const std::vector<RosterEntry> roster{
      {1, "Ana Ribeiro"},
      {5, "Ana Teixeira Ribeiro"},   // same first+last as id 1
  };
  auto out = matchName("Ana M Ribeiro", 3, roster);  // no exact; matches both -> ambiguous
  REQUIRE(std::holds_alternative<Rejected>(out));
}

TEST_CASE("duplicate exact roster entries are rejected as ambiguous", "[validation]") {
  const std::vector<RosterEntry> roster{{1, "Ana Ribeiro"}, {9, "ana ribeiro"}};
  auto out = matchName("Ana Ribeiro", 4, roster);
  REQUIRE(std::holds_alternative<Rejected>(out));
}
