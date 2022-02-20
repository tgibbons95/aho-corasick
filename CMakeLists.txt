cmake_minimum_required(VERSION 3.16)
project(AhoCorasick)

enable_testing()

add_subdirectory("external_src")
add_subdirectory("src")
add_subdirectory("test")

install(DIRECTORY "include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")