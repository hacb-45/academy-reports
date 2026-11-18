#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "validation/import_outcome.h"

using namespace academy;

TEST_CASE("isRejected only for Rejected", "[validation]") {
  REQUIRE(isRejected(ImportOutcome{Rejected{3, "unknown name"}}));
  REQUIRE_FALSE(isRejected(ImportOutcome{Accepted{42}}));
  REQUIRE_FALSE(isRejected(ImportOutcome{Warned{42, "fuzzy"}}));
}

TEST_CASE("tally counts each kind", "[validation]") {
  std::vector<ImportOutcome> rows{
      Accepted{1}, Accepted{2}, Warned{3, "fuzzy"}, Rejected{4, "bad year"}, Accepted{5},
  };
  const auto c = tally(rows);
  REQUIRE(c.accepted == 3);
  REQUIRE(c.warned == 1);
  REQUIRE(c.rejected == 1);
}

TEST_CASE("describe renders each alternative", "[validation]") {
  REQUIRE(describe(ImportOutcome{Accepted{42}}) == "accepted (student 42)");
  REQUIRE(describe(ImportOutcome{Warned{7, "accent fold"}}) == "warning (student 7): accent fold");
  REQUIRE(describe(ImportOutcome{Rejected{9, "duplicate"}}) == "row 9 rejected: duplicate");
}
