##
# @file CMakeLists.txt
# @brief
#
# @author
# @email
#
cmake_minimum_required(VERSION 3.12)

# 工程名
project(proj3 LANGUAGES CXX VERSION 0.1.0)
message(STATUS "proj3 -- a bitmap inverted index project")

# c++版本
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设定输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# 编译目录
add_subdirectory(third)
add_subdirectory(src)
add_subdirectory(tests)

