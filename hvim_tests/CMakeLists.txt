project(HVimTests)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../src)

add_compile_options(-std=c++2a)
add_executable(hvimtestsrun MoveCursorTest.cpp)

target_link_libraries(hvimtestsrun gtest gtest_main)
