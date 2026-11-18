#include "validation/name_matcher.h"

#include <utility>

#include "validation/normalize.h"

namespace academy {

namespace {

// First and last token of a NORMALIZED name (single-spaced, trimmed).
// "ana maria ribeiro" -> {"ana", "ribeiro"}.  One token -> both equal it.
// (Provided for you -- the fiddly string_view scanning; focus on matchName.)
[[nodiscard]] std::pair<std::string_view, std::string_view> firstLast(std::string_view norm) {
  if (norm.empty()) return {{}, {}};
  const auto firstSpace = norm.find(' ');
  if (firstSpace == std::string_view::npos) return {norm, norm};
  const auto lastSpace = norm.rfind(' ');
  return {norm.substr(0, firstSpace), norm.substr(lastSpace + 1)};
}

}  // namespace

ImportOutcome matchName(std::string_view rawName, std::size_t row,
                        const std::vector<RosterEntry>& roster) {
  const std::string query = normalizeName(rawName);
  std::vector<StudentId> exactMatches;
  std::vector<StudentId> fuzzyMatches;

  for (const auto& entry : roster) {
    const std::string entryNorm = normalizeName(entry.name);
    if (entryNorm == query) {
      exactMatches.push_back(entry.id);
    }
    if (firstLast(entryNorm) == firstLast(query)) {
      fuzzyMatches.push_back(entry.id);
    }
  }

  if (exactMatches.size() == 1) {
    return Accepted{exactMatches.front()};  // single exact match
  }

  if (exactMatches.size() > 1) {
    return Rejected{row, "ambiguous: multiple exact matches"};
  }

  if (fuzzyMatches.size() == 1) {
    return Warned{fuzzyMatches.front(), "fuzzy match (ignored middle name/initial)"};
  } else if (fuzzyMatches.size() > 1) {
    return Rejected{row, "ambiguous: multiple fuzzy matches"};
  }

  return Rejected{row, "unknown name"};
}

}  // namespace academy
