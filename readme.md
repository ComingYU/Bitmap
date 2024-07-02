## 目录结构

工程建议在Linux下实现，目录结构：

- include   头文件
- src       源文件 -bitmap.cc
- tests     单元测试 -bitmapTest.cc & data.csv
- third     第三方库

## 编译

```bash
mkdir build
cd build
cmake ..
make
./bin/btest
```

## 说明
使用Roaring bitmaps压缩bitmap

## 接口
id->{tag}接口：TagIndex.queryTagById(非静态)  
tag->bitmap接口：TagIndex.queryTag(非静态)  
tag->id方法：searcher.tagtoid(静态)  
