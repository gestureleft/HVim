cmake_minimum_required(VERSION 3.2)

project(HVimSrc)

set(SOURCE_FILES main.cpp EditorConfig.h util.h CppLexer.cpp CppLexer.h)

add_compile_options(-std=c++2a)
add_compile_options(-Wall)

add_executable(hvim ${SOURCE_FILES})