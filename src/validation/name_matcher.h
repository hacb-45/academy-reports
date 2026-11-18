#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "validation/import_outcome.h"

namespace academy {

// One roster entry to match against (the class's enrolled students).
struct RosterEntry {
  StudentId id;
  std::string name;  // as registered; matched case/space/-insensitively
};

// Match a teacher-supplied name (from import row `row`, 1-based) against the
// roster, following the spec's rules:
//   - exact normalized match           -> Accepted
//   - single fuzzy candidate           -> Warned  (logged, admin reconciles)
//   - no match, or 2+ candidates        -> Rejected
// Returns the outcome as an ImportOutcome variant.
[[nodiscard]] ImportOutcome matchName(std::string_view rawName, std::size_t row,
                                      const std::vector<RosterEntry>& roster);

}  // namespace academy
