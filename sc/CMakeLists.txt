cmake_minimum_required(VERSION 3.5)
project(SystemC_test)

set (CMAKE_PREFIX_PATH /usr/local/lib/cmake/SystemC/lib/cmake)
find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
    "C++ standard to build all targets. Supported values are 98, 11 and 14.")
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
    "with CMAKE_CXX_STANDARD selected C++ standard is requirement.")
set (CSV_CXX_STANDARD ${SystemC_CXX_STANDARD})

set (CMAKE_C_FLAGS "${CMKAE_C_FLAGS} -fsigned-char")
set (CMAKE_CXX_FLAGS "${CMKAE_CXX_FLAGS} -fsigned-char")

set (BUILD_GPROF_SUPPORT NO CACHE BOOL "Add -pg to build flags to support gprof for profiling")
if (${BUILD_GPROF_SUPPORT})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

set (BUILD_SHARED_LIBS true)
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# find_package(cli CONFIG REQUIRED)
# find_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory(main)