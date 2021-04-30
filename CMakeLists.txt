cmake_minimum_required(VERSION 3.4)

project(CacheProject)

include(cmake/CMakeTemplates/CFlags.cmake)

add_subdirectory(modules)

add_subdirectory(src/demo)

add_executable(BenchCache "src/BenchCache.c" "src/CalculateMisses.c")
target_link_libraries(BenchCache DataStructures)
