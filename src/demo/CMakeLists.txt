find_package(CURL)

if(CURL_FOUND)
    add_executable(demo "demo.c")
    target_link_libraries(demo DataStructures CURL::libcurl)
endif()
