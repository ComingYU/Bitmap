
# 指定头文件搜索目录
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/third/benchmark/include)

# 添加可执行程序目标
add_executable(btest bitmapTest.cc test.cc)
# 链接库
target_link_libraries(btest PRIVATE gtest roaring benchmark)
