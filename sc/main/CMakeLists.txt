cmake_minimum_required(VERSION 3.5)

set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (CMAKE_PREFIX_PATH /usr/local/lib/cmake/SystemC)

include_directories(${CMAKE_PREFIX_PATH}/include)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
find_package(SystemCLanguage CONFIG REQUIRED)

add_executable(main
        main.cpp 
)

set_property(SOURCE main.cpp APPEND_STRING PROPERTY COMPILE_FLAGS "-fno-var-tracking-assignments")
target_include_directories(main PRIVATE ${main_SOURCE_DIR})
target_link_libraries(main SystemC::systemc)
# target_link_libraries(main sc_impl_folder)
# target_link_libraries(main argparse)