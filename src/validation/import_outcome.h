#pragma once

#include <cstddef>
#include <string>
#include <variant>
#include <vector>

namespace academy {

using StudentId = int;  // a simple id for now; a strong type comes later

// The three possible results of validating ONE import row. Each carries
// DIFFERENT data -- this is why a variant beats an enum here.
struct Accepted {
  StudentId id;                 // the row matched this student cleanly
};
struct Warned {
  StudentId id;                 // matched, but...
  std::string reason;           // ...via a fuzzy match -- logged for the admin
};
struct Rejected {
  std::size_t row;              // 1-based row number in the file
  std::string reason;           // why it was refused
};

// A sum type: an ImportOutcome is EXACTLY ONE of these at a time.
using ImportOutcome = std::variant<Accepted, Warned, Rejected>;

struct OutcomeCounts {
  std::size_t accepted = 0;
  std::size_t warned = 0;
  std::size_t rejected = 0;
};

// Count how many of each kind across many rows (the import summary).
[[nodiscard]] OutcomeCounts tally(const std::vector<ImportOutcome>& outcomes);

// A one-line human description of a single outcome.
[[nodiscard]] std::string describe(const ImportOutcome& outcome);

// True if this outcome is a Rejected.
[[nodiscard]] bool isRejected(const ImportOutcome& outcome);

}  // namespace academy
