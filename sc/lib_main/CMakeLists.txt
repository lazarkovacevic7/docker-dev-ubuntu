cmake_minimum_required(VERSION 3.5)

add_library(lib_main
        lib_main.cpp 
)

target_include_directories(lib_main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})