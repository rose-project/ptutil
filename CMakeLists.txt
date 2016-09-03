
cmake_minimum_required(VERSION 2.8)

SET(PROJECT_NAME "ptutil")

project(${PROJECT_NAME})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

include_directories(
    ${CMAKE_CURRENT_SRC_DIR}/lib
    ${CMAKE_CURRENT_SRC_DIR}/src
    )

add_subdirectory(lib)
add_subdirectory(src)

