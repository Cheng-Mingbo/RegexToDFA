cmake_minimum_required(VERSION 3.22)
project(RegexToDFA)

set(CMAKE_CXX_STANDARD 20)
aux_source_directory(. LIST_SRC)
add_executable(RegexToDFA ${LIST_SRC})
