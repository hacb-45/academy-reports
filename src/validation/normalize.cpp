#include "validation/normalize.h"

namespace academy {

namespace {

// Deliberately NOT std::isspace / std::tolower: those are locale-dependent and
// take an int, with UB on negative char values. We define explicit ASCII rules.
[[nodiscard]] bool isAsciiSpace(char c) {
  switch (c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
    case '\v':
      return true;
    default:
      return false;
  }
}

[[nodiscard]] char toAsciiLower(char c) {
  if ((c >= 'A') && (c <= 'Z')) {
    return static_cast<char>(c + ('a' - 'A'));
  }
  return c;
}

}  // namespace

std::string normalizeName(std::string_view raw) {
  bool pendingSpace = false;
  std::string result;
  for (char c : raw) {
    if (isAsciiSpace(c)) {
      if (!result.empty()) {
        pendingSpace = true;
      }
    } else {
      if (pendingSpace) {
        result.push_back(' ');
        pendingSpace = false;
      }
      result.push_back(toAsciiLower(c));
    }
  }
  return result;
}

}  // namespace academy
