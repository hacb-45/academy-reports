#include "adapters/csv/csv.h"

#include <utility>

namespace academy::csv {

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

}  // namespace academy::csv
