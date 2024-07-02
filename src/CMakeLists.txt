# 添加库
add_library(bitmap bitmap.cc)

# 添加头文件目录
target_include_directories(bitmap PRIVATE ${CMAKE_SOURCE_DIR}/third/CRoaring-4.0.0/include)

# 链接CRoaring库
target_link_libraries(bitmap PRIVATE roaring)
