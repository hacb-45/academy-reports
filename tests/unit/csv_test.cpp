#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "adapters/csv/csv.h"

using academy::csv::parseRecord;
using Fields = std::vector<std::string>;

TEST_CASE("plain unquoted fields", "[csv]") {
  REQUIRE(parseRecord("a,b,c") == Fields{"a", "b", "c"});
}

TEST_CASE("empty fields are preserved", "[csv]") {
  REQUIRE(parseRecord("a,,c") == Fields{"a", "", "c"});
  REQUIRE(parseRecord("a,b,") == Fields{"a", "b", ""});   // trailing empty
  REQUIRE(parseRecord("") == Fields{""});                 // empty line = one empty field
}

TEST_CASE("quoted field with an embedded delimiter", "[csv]") {
  REQUIRE(parseRecord(R"("hello, world",x)") == Fields{"hello, world", "x"});
}

TEST_CASE("escaped double-quote inside a quoted field", "[csv]") {
  REQUIRE(parseRecord(R"("she said ""hi""",y)") == Fields{R"(she said "hi")", "y"});
}

TEST_CASE("semicolon is literal, not a delimiter", "[csv]") {
  // CSV-03: attendance lists use ';' and must survive as data inside a field
  REQUIRE(parseRecord(R"(a,"2026-11-03;2026-11-05",d)") ==
          Fields{"a", "2026-11-03;2026-11-05", "d"});
}

TEST_CASE("delimiter can be overridden", "[csv]") {
  REQUIRE(parseRecord("a;b;c", ';') == Fields{"a", "b", "c"});
}
