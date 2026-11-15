#include <iostream>
#include <span>
#include <string_view>

#include "util/version.h"

int main(int argc, char* argv[]) {

    // View argv as a span instead of doing raw pointer arithmetic.
    const std::span<char*> args(argv, static_cast<std::size_t>(argc));

    if (args.size() >= 2 && std::string_view{args[1]} == "--version") {
        std::cout << "academy " << academy::versionString() << '\n';
        return 0;
    }
    std::cout << "usage: academy --version\n";
    return 3;  // exit-code contract: 3 = usage error
}
