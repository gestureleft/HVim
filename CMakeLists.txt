cmake_minimum_required(VERSION 3.2)

project(HVim)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(hvim_tests)
add_subdirectory(src)