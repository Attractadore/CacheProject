find_package(CURL REQUIRED)

add_executable(demo "demo.c")
target_link_libraries(demo DataStructures CURL::libcurl)
