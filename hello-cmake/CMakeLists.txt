cmake_minimum_required(VERSION 3.16)

project(hello-cmake LANGUAGES C CXX)

add_executable(
    hello-cmake
    hello.c
    helloprint.c
)

target_link_libraries(
    hello-cmake
    PRIVATE
)

target_include_directories(
    hello-cmake
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS hello-cmake DESTINATION bin)