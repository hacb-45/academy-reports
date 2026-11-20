#include "adapters/csv/csv.h"

#include <utility>

namespace academy::csv {

namespace {
constexpr std::string_view kBom = "\xEF\xBB\xBF";
}  // namespace

std::vector<std::string> parseRecord(std::string_view line, char delim) {
  std::vector<std::string> fields;
  std::string current;  // the field being built
  bool inQuotes = false;

  for (std::size_t i = 0; i < line.size(); ++i) {
    const char c = line[i];

    if (inQuotes) {
      if (c == '"') {
        // look at the NEXT char: "" is an escaped quote, otherwise the quote ends
        if (i + 1 < line.size() && line[i + 1] == '"') {
          current.push_back('"');
          ++i;  // consume the second quote of the pair
        } else {
          inQuotes = false;  // closing quote
        }
      } else {
        current.push_back(c);  // any other char inside quotes is literal
      }
    } else {
      if (c == '"') {
        inQuotes = true;  // opening quote
      } else if (c == delim) {
        fields.push_back(std::move(current));  // end of field: MOVE it in
        current.clear();                       // current is valid-but-empty after a move
      } else {
        current.push_back(c);
      }
    }
  }

  fields.push_back(std::move(current));  // the final field (after the last delim)
  return fields;
}

std::string_view stripBom(std::string_view s) {
  return s.starts_with(kBom) ? s.substr(kBom.size()) : s;
}

std::vector<std::vector<std::string>> parseDocument(std::string_view text, char delim) {
  std::vector<std::vector<std::string>> rows;
  std::vector<std::string> row;
  std::string current;
  bool inQuotes = false;

  text = stripBom(text);

  for (std::size_t i = 0; i < text.size(); ++i) {
    const char c = text[i];
    if (inQuotes) {
      if (c == '"') {
        if (i + 1 < text.size() && text[i + 1] == '"') {
          current.push_back('"');
          ++i;
        } else {
          inQuotes = false;
        }
      } else {
        current.push_back(c);  // embedded newlines land here -> preserved
      }
    } else {
      if (c == '"') {
        inQuotes = true;
      } else if (c == delim) {
        row.push_back(std::move(current));
        current.clear();
      } else if (c == '\r') {
        // ignore '\r' so CRLF is tolerated -- do nothing here.
      } else if (c == '\n') {
        row.push_back(std::move(current));
        current.clear();
        rows.push_back(std::move(row));
        row.clear();
      } else {
        current.push_back(c);
      }
    }
  }

  if (!current.empty() || !row.empty()) {
    row.push_back(std::move(current));
    rows.push_back(std::move(row));
  }
  return rows;
}

}  // namespace academy::csv
