cmake_minimum_required(VERSION 3.10)

project(LinuxBuildSystem)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к исходникам
include_directories(include)

# Файлы исходников
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(linux_build_system ${SOURCES})

# Добавим флаг компиляции
add_compile_options(-Wall -Wextra -Werror)

target_link_libraries(${PROJECT_NAME} pthread)
