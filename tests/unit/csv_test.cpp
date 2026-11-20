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

using academy::csv::parseDocument;
using academy::csv::stripBom;
using Rows = std::vector<std::vector<std::string>>;

TEST_CASE("stripBom removes a UTF-8 BOM only when present", "[csv]") {
  REQUIRE(stripBom("\xEF\xBB\xBF" "hello") == "hello");
  REQUIRE(stripBom("hello") == "hello");
}

TEST_CASE("document splits rows on LF", "[csv]") {
  REQUIRE(parseDocument("a,b\nc,d") == Rows{{"a", "b"}, {"c", "d"}});
}

TEST_CASE("document tolerates CRLF", "[csv]") {
  REQUIRE(parseDocument("a,b\r\nc,d") == Rows{{"a", "b"}, {"c", "d"}});
}

TEST_CASE("trailing newline does not create an empty row", "[csv]") {
  REQUIRE(parseDocument("a,b\n") == Rows{{"a", "b"}});
}

TEST_CASE("empty document yields no rows", "[csv]") {
  REQUIRE(parseDocument("") == Rows{});
}

TEST_CASE("document strips a leading BOM", "[csv]") {
  REQUIRE(parseDocument("\xEF\xBB\xBF" "name,year\nAna,8") ==
          Rows{{"name", "year"}, {"Ana", "8"}});
}

TEST_CASE("newline inside quotes is kept as field data", "[csv]") {
  REQUIRE(parseDocument("\"line1\nline2\",x") == Rows{{"line1\nline2", "x"}});
}
