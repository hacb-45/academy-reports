#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace academy::csv {

// Parse ONE CSV record into its fields, per RFC 4180 (one line, no embedded
// newlines). See csv.cpp for examples.
[[nodiscard]] std::vector<std::string> parseRecord(std::string_view line, char delim = ',');

// Remove a leading UTF-8 BOM (EF BB BF) if present; otherwise return unchanged.
[[nodiscard]] std::string_view stripBom(std::string_view s);

// Parse a WHOLE CSV document into rows of fields. BOM-tolerant, accepts CRLF
// and LF line endings, and is quote-aware: a newline inside a quoted field is
// kept as data, not treated as a record separator. Empty input -> no rows.
[[nodiscard]] std::vector<std::vector<std::string>> parseDocument(std::string_view text,
                                                                  char delim = ',');

}  // namespace academy::csv
