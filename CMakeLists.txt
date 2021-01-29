cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/cmake/Modules/")

project(simpleJson CXX)

include(SemVer)
include(Publish)
include(FindDoxygen)

set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 14)
add_definitions(-Wall)

set(DOXYGEN_SKIP_DOT TRUE)
find_package(Doxygen)
    
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

add_subdirectory(tst)

#
# Enable test target
#
enable_testing()

add_test (NAME json COMMAND testcases 	    "[json]")
