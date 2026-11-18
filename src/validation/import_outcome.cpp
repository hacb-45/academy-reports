#include "validation/import_outcome.h"

#include <format>

namespace academy {

namespace {

// The "overloaded" idiom: bundles several lambdas into one callable object that
// has an operator() for each. std::visit picks the one matching the active type.
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

}  // namespace

OutcomeCounts tally(const std::vector<ImportOutcome>& outcomes) {
  OutcomeCounts counts;
  for (const auto& outcome : outcomes) {
    std::visit(overloaded{[&](const Accepted&) { ++counts.accepted; },
                          [&](const Warned&) { ++counts.warned; },
                          [&](const Rejected&) { ++counts.rejected; }},
               outcome);
  }
  return counts;
}

std::string describe(const ImportOutcome& outcome) {
  return std::visit(
      overloaded{
          [](const Accepted& a) { return std::format("accepted (student {})", a.id); },
          [](const Warned& w) { return std::format("warning (student {}): {}", w.id, w.reason); },
          [](const Rejected& r) { return std::format("row {} rejected: {}", r.row, r.reason); },
      },
      outcome);
}

bool isRejected(const ImportOutcome& outcome) { return std::holds_alternative<Rejected>(outcome); }

}  // namespace academy
