#include "core/greeting.hpp"

namespace core {

std::string build_greeting(const std::string& name) {
    return "Hola, " + name + "!";
}

} // namespace core
