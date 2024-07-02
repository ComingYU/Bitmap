#include <gtest/gtest.h>
#include <roaring/roaring.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../src/bitmap.cc"
// #include <test_target.h>

using namespace std;



TEST(bitmap,tagtoid){
    TagIndex tagIndex;

    // 从文件读取数据构建标签倒排索引
    ifstream file("/root/db/bitmap/tests/data.csv");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            tagIndex.updateIndex(line);
        }
        file.close();
    } else {
        cerr << "Error opening file: " << strerror(errno) << "\n";
    }


    //测试and
    string input="tag1 and tag2 and tag3"; 
    std::vector<uint32_t> values;
    uint32_t size;   
    roaring_bitmap_t* andResult=searcher::tagtoid(tagIndex,input);
    ASSERT_TRUE(andResult);
    if (andResult) {
        size = roaring_bitmap_get_cardinality(andResult);
        values.resize(size);
        roaring_bitmap_to_uint32_array(andResult, values.data());
        ASSERT_EQ(2,values.size());
        ASSERT_EQ(1,static_cast<int>(values[0]));
        ASSERT_EQ(4,static_cast<int>(values[1]));
        roaring_bitmap_free(andResult);
    } else {
        cout << "andResult is empty" << endl;
    }

    //测试or
    input="tag2 or tag5 or tag6";  
    roaring_bitmap_t* orResult=searcher::tagtoid(tagIndex,input);
    ASSERT_TRUE(orResult);
    if (orResult) {
        size = roaring_bitmap_get_cardinality(orResult);
        values.resize(size);
        roaring_bitmap_to_uint32_array(orResult, values.data());
        ASSERT_EQ(6,values.size());
        ASSERT_EQ(1,static_cast<int>(values[0]));
        ASSERT_EQ(6,static_cast<int>(values[5]));
        roaring_bitmap_free(orResult);
    } else {
        cout << "orResult is empty" << endl;
    }

    //测试not
    input="tag1 not tag5";
    roaring_bitmap_t* notResult=searcher::tagtoid(tagIndex,input);
    ASSERT_TRUE(notResult);
    if (notResult) {
        size = roaring_bitmap_get_cardinality(notResult);
        values.resize(size);
        roaring_bitmap_to_uint32_array(notResult, values.data());
        ASSERT_EQ(1,values.size());
        ASSERT_EQ(1,static_cast<int>(values[0]));
        roaring_bitmap_free(notResult);
    } else {
        cout << "notResult is empty" << endl;
    }

    //交并差多标签查询
    input="tag1 and tag2 or tag4 not tag5";
    roaring_bitmap_t* allResult=searcher::tagtoid(tagIndex,input);
    ASSERT_TRUE(allResult);
    if (allResult) {
        size = roaring_bitmap_get_cardinality(allResult);
        values.resize(size);
        roaring_bitmap_to_uint32_array(allResult, values.data());
        ASSERT_EQ(1,values.size());
        ASSERT_EQ(1,static_cast<int>(values[0]));
        roaring_bitmap_free(allResult);
    } else {
        cout << "allResult is empty" << endl;
    }
}
TEST(bitmap,idtotag){
    TagIndex tagIndex;

    // 从文件读取数据构建标签倒排索引
    ifstream file("/root/db/bitmap/tests/data.csv");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            tagIndex.updateIndex(line);
        }
        file.close();
    } else {
        cerr << "Error opening file: " << strerror(errno) << "\n";
    }

    vector<string> tags=tagIndex.queryTagById(2);
    ASSERT_EQ(3,tags.size());
    ASSERT_EQ("tag1",tags[0]);
    ASSERT_EQ("tag4",tags[1]);
    ASSERT_EQ("tag5",tags[2]);


}