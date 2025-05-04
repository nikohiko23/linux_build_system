#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../include/build_system.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("Test getCppFiles returns correct list") {
    fs::create_directory("tests/tmp");
    std::ofstream("tests/tmp/test1.cpp");
    std::ofstream("tests/tmp/test2.cpp");

    BuildSystem builder("tests/tmp");
    auto files = builder.getCppFiles();

    CHECK(files.size() == 2);

    fs::remove_all("tests/tmp");
}

TEST_CASE("Test needsRebuild returns true on first build") {
    fs::create_directory("tests/tmp");
    std::ofstream("tests/tmp/test.cpp") << "int main() { return 0; }";

    BuildSystem builder("tests/tmp");
    builder.build();

    CHECK(builder.needsRebuild("tests/tmp/test.cpp") == false);

    fs::remove_all("tests/tmp");
}

