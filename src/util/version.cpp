#include "util/version.h"

namespace academy {

std::string_view versionString() noexcept {
    return "0.1.0";  // a string literal lives for the whole program, so the view never dangles
}

}  // namespace academy
