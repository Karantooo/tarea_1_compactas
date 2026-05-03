#include <iostream>

#include "core/greeting.hpp"

int main() {
    std::cout << core::build_greeting("main_app") << '\n';
    return 0;
}
