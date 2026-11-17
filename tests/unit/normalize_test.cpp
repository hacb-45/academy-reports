#include <catch2/catch_test_macros.hpp>

#include "validation/normalize.h"

using academy::normalizeName;

TEST_CASE("lowercases ASCII", "[validation]") {
    REQUIRE(normalizeName("Ana Ribeiro") == "ana ribeiro");
    REQUIRE(normalizeName("MIGUEL COSTA") == "miguel costa");
}

TEST_CASE("trims leading and trailing whitespace", "[validation]") {
    REQUIRE(normalizeName("   ana ") == "ana");
    REQUIRE(normalizeName("\t ana\n") == "ana");
}

TEST_CASE("collapses internal whitespace runs to one space", "[validation]") {
    REQUIRE(normalizeName("ana   ribeiro") == "ana ribeiro");
    REQUIRE(normalizeName("  Ana \t  RIBEIRO ") == "ana ribeiro");   // the headline case
}

TEST_CASE("handles empty and all-whitespace input", "[validation]") {
    REQUIRE(normalizeName("") == "");
    REQUIRE(normalizeName("    ") == "");
}

TEST_CASE("single word, no spaces", "[validation]") {
    REQUIRE(normalizeName("Costa") == "costa");
}
