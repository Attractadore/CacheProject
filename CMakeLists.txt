cmake_minimum_required(VERSION 3.4)

project(CacheProject)

include(cmake/CMakeTemplates/CFlags.cmake)
include(cmake/CMakeTemplates/AddTestSuite.cmake)
include(cmake/CMakeTemplates/CopyToBuild.cmake)

add_subdirectory(modules)
add_subdirectory(test)
add_subdirectory(src/demo)

add_executable(BenchCache "src/BenchCache.c" "src/CalculateMisses.c")
target_link_libraries(BenchCache DataStructures)

copy_to_build("test/genRandom.py")
copy_to_build("test/genLoop.py")
copy_to_build("test/genNormal.py")
copy_to_build("test/genTemporal.py")
copy_to_build("test/plot.py")
copy_to_build("src/adapter.sh")
