#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace academy::csv {

// Parse ONE CSV record into its fields, per RFC 4180:
//   - fields separated by `delim` (comma by default)
//   - a field may be "quoted"; inside quotes, the delimiter is literal
//   - inside quotes, a literal double-quote is written as "" (escaped)
// (Embedded newlines inside quotes come in the next stage; this parses one line.)
//
// Examples:
//   a,b,c                 -> {"a", "b", "c"}
//   "hello, world",x      -> {"hello, world", "x"}
//   "she said ""hi""",y   -> {she said "hi", y}
[[nodiscard]] std::vector<std::string> parseRecord(std::string_view line, char delim = ',');

}  // namespace academy::csv
