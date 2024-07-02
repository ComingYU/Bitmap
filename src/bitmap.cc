#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <roaring/roaring.h>

using namespace std;

class TagIndex {
private:
    unordered_map<string, roaring_bitmap_t*> tagIndex;  // 标签到 Roaring Bitmap 的映射
    unordered_map<uint32_t, vector<string>> idIndex;  //存储id到tag的映射 

public:
    // 构造函数初始化
    TagIndex() {}


    // 从CSV行中解析id和tags，并更新标签倒排索引
    void updateIndex(const string& line) {
        istringstream ss(line);
        string id, tag;

        // 读取id
        getline(ss, id, '|');
        id = id.substr(0, id.find_last_not_of(" \t\n\r\f\v") + 1);  // 去除末尾空白字符

        // 读取tags
        while (getline(ss, tag, '|')) {
            tag = tag.substr(tag.find_first_not_of(" \t\n\r\f\v"));
            roaring_bitmap_t* bitmap = tagIndex[tag];
            if (!bitmap) {
                bitmap = roaring_bitmap_create();
                tagIndex[tag] = bitmap;
            }
            roaring_bitmap_add(bitmap, stoi(id));  // 将id添加到对应的标签的Bitmap中
            idIndex[stoi(id)].push_back(tag);
        }
    }

    // 查询指定标签对应的Roaring Bitmap
    roaring_bitmap_t* queryTag(const string& tag) {
        auto it = tagIndex.find(tag);
        if (it != tagIndex.end()) {
            return it->second;
        } else {
            return nullptr;  // 找不到该标签对应的Bitmap
        }
    }

    // 通过id查询tag
    vector<string> queryTagById(uint32_t id){
        auto it = idIndex.find(id);
        if(it != idIndex.end()){
            return it->second;
        }else{
            return {};
        }

    }

    // 进行多标签的交集操作
    roaring_bitmap_t* intersectTags(const vector<string>& tags) {
        roaring_bitmap_t* result = nullptr;
        for (const auto& tag : tags) {
            roaring_bitmap_t* bitmap = tagIndex[tag];
            if (!bitmap) {
                return nullptr;  // 如果有任何一个标签没有对应的Bitmap，则返回空指针
            }
            if (!result) {
                result = roaring_bitmap_copy(bitmap);
            } else {
                roaring_bitmap_and_inplace(result, bitmap);  // 求交集
            }
        }
        return result;
    }

    // 进行多标签的并集操作
    roaring_bitmap_t*  unionTags(const vector<string>& tags){
        roaring_bitmap_t* result=nullptr;
        for(const auto& tag :tags){
            roaring_bitmap_t* bitmap = tagIndex[tag];
            if(!bitmap){
                return nullptr;
            }
            if(!result){
                result=roaring_bitmap_copy(bitmap);
            }else{
                roaring_bitmap_or_inplace(result,bitmap);
            }
            
        }
        return result;
    }

    // 多标签的差操作
    roaring_bitmap_t*  differenceTags(const vector<string>& tags){
        roaring_bitmap_t* result=nullptr;
        for(const auto& tag :tags){
            roaring_bitmap_t* bitmap = tagIndex[tag];
            if(!bitmap){
                return nullptr;
            }
            if(!result){
                result=roaring_bitmap_copy(bitmap);
            }else{
                roaring_bitmap_and_inplace(result,bitmap);
            }
            
        }
        return result;
    }

};

class analyser{
    public:
    static void splitTags(const string& input, vector<string>& andTags, vector<string>& orTags, vector<string>& notTags);
    
};
//分解输入的查询语句，根据交并差存储到不同的vector
void analyser::splitTags(const string& input, vector<string>& andTags, vector<string>& orTags, vector<string>& notTags) {
    istringstream stream(input);
    string token;
    
    // 当前操作符，初始为 and
    string currentOperator = "and";

    while (stream >> token) {
        if (token == "and" || token == "or" || token == "not") {
            currentOperator = token;  // 更新当前操作符
        } else {
            // 去除前后可能存在的空格
            token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
            token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
            
            if (currentOperator == "and") {
                andTags.push_back(token);
            } else if (currentOperator == "or") {
                orTags.push_back(token);
            } else if (currentOperator == "not") {
                notTags.push_back(token);
            }
        }
    }
};
class searcher{
    public:
    static roaring_bitmap_t* tagtoid(TagIndex tagIndex,string input);
};
//通过tag查询id
roaring_bitmap_t* searcher::tagtoid(TagIndex tagIndex,string input){
    //分解输入
    vector<string> andTags,orTags,notTags;
    analyser::splitTags(input,andTags,orTags,notTags);
    

    // 示例多标签交集操作
    roaring_bitmap_t* andResult = tagIndex.intersectTags(andTags);
    roaring_bitmap_t* orResult = tagIndex.unionTags(orTags);
    roaring_bitmap_t* notResult = tagIndex.differenceTags(notTags);
    roaring_bitmap_t* Result=nullptr;

    if(orResult)
    Result=roaring_bitmap_or(andResult,orResult);
    else
    Result=andResult;

    if(notResult)
    Result=roaring_bitmap_andnot(Result,notResult);
    return Result;
};
