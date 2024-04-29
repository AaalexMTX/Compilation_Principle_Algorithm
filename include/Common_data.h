#pragma once
#include<regex>
#include<string>
#include<set>
#include<unordered_map>
#include<unordered_set>

//放共同使用的全局方法 / 全局变量

// 不extern 只能在定义的文件使用
extern std::regex N;
extern std::regex T;							//正则

//得到完整符号
std::string getFullChar(std::string candidate, int pos);

//更新指针为 下一个非空字符
int getBC(int pos, char line[]);

