cmake_minimum_required(VERSION 3.4)

project(CacheProject)

include(cmake/CMakeTemplates/CFlags.cmake)

add_subdirectory(modules)

add_subdirectory(src/demo)

add_library(CacheLib "src/DummyCache.c" "src/CachePolicy.c")
target_include_directories(CacheLib PUBLIC "include")
