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

// Quote one field for output IF it needs it: a field containing the delimiter,
// a double-quote, CR or LF is wrapped in "..." with internal quotes doubled
// (defensive quoting, CSV-04). Otherwise returned unchanged.
[[nodiscard]] std::string quoteField(std::string_view field, char delim = ',');

// Serialize rows to a CSV document for the Windows-facing path: a leading UTF-8
// BOM (CSV-01), CRLF line endings (CSV-02), fields quoted defensively (CSV-04).
[[nodiscard]] std::string writeDocument(const std::vector<std::vector<std::string>>& rows,
                                        char delim = ',');

}  // namespace academy::csv
