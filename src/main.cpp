#include <iostream>
#include "build_system.hpp"

int main() {
    std::cout << "Launching your own build system..." << std::endl;

    BuildSystem builder("src");
    builder.build();

    return 0;
}
