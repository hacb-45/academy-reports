cmake_minimum_required(VERSION 3.22)
project(academy_reports VERSION 0.1.0 LANGUAGES CXX)

# --- Language standard (binding: C++20, no compiler extensions) -------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put executables in build/<preset>/bin/ (the QA handbook expects this path)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Warnings-as-errors, applied to OUR targets only ------------------------
# An INTERFACE library carries settings, not code. Linking it to a target
# gives that target these flags. Third-party code (Catch2) doesn't get them.
add_library(academy_warnings INTERFACE)
if(MSVC)
  target_compile_options(academy_warnings INTERFACE /W4 /permissive- /WX)
else()
  target_compile_options(academy_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror)
endif()

# --- Core library: all the real code, everything except main() --------------
# Both the app and the tests link this, so the tests exercise the same code.
add_library(academy_core
  src/util/version.cpp
  src/domain/teaching_calendar.cpp
  src/validation/normalize.cpp
)
target_include_directories(academy_core PUBLIC src)
target_link_libraries(academy_core PRIVATE academy_warnings)

# --- The application ---------------------------------------------------------
add_executable(academy src/main.cpp)
target_link_libraries(academy PRIVATE academy_core academy_warnings)

# --- Tests (Catch2 v3, pinned tag) -------------------------------------------
include(FetchContent)
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()
add_executable(academy_tests
  tests/unit/version_test.cpp
  tests/unit/period_test.cpp
  tests/unit/teaching_calendar_test.cpp
  tests/unit/normalize_test.cpp
)
target_link_libraries(academy_tests PRIVATE
  academy_core academy_warnings Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(academy_tests)   # each TEST_CASE becomes a CTest test
