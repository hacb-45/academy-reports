#pragma once
#include <string_view>

namespace academy {

// Declaration only: says "this function exists". The definition lives in
// version.cpp, compiled once. Every file that includes this header can call it.
[[nodiscard]] std::string_view versionString() noexcept;

}  // namespace academy
