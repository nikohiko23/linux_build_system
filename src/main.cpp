#include <iostream>
#include "build_system.hpp"

int main() {
    std::cout << "Launching your own build system..." << std::endl;

    BuildSystem builder("/home/nik/Desktop/linux_build_system/src");
    builder.build();

    return 0;
}
