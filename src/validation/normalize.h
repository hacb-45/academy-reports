#pragma once

#include <string>
#include <string_view>

namespace academy {

// Normalize a name for matching: trim ends, collapse internal whitespace runs to
// a single space, and ASCII-lowercase. "  Ana   RIBEIRO " -> "ana ribeiro".
//
// Takes a string_view (borrows the caller's bytes, no copy), returns a fresh
// std::string (owns the transformed bytes -- the input may be shorter/different).
//
// ASCII-only for now; Unicode-aware casefolding/accent-folding comes later.
[[nodiscard]] std::string normalizeName(std::string_view raw);

}  // namespace academy
