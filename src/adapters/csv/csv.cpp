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

std::string quoteField(std::string_view field, char delim) {
  const bool needsQuote =
      field.find(delim) != std::string_view::npos || field.find('"') != std::string_view::npos ||
      field.find('\n') != std::string_view::npos || field.find('\r') != std::string_view::npos;

  if (!needsQuote) {
    return std::string{field};
  }

  std::string quoted;
  quoted.push_back('"');
  for (const char c : field) {
    if (c == '"') {
      quoted.push_back('"');  // double the quote
    }
    quoted.push_back(c);
  }
  quoted.push_back('"');
  return quoted;
}

std::string writeDocument(const std::vector<std::vector<std::string>>& rows, char delim) {
  // TODO:
  //  - start the result with the BOM: std::string out{kBom};
  //  - for each row: join quoteField(field, delim) with `delim` between fields,
  //    then append "\r\n" after the row.
  //  - return out;
  std::string out{kBom};
  for (const auto& row : rows) {
    for (std::size_t i = 0; i < row.size(); ++i) {
      out += quoteField(row[i], delim);
      if (i + 1 < row.size()) {
        out += delim;
      }
    }
    out += "\r\n";
  }
  return out;  // STUB
}

}  // namespace academy::csv
