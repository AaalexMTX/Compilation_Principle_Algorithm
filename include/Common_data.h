#pragma once
#include<algorithm>
#include<regex>
#include<string>
#include<vector>
#include<set>
#include<unordered_map>
#include<unordered_set>

//放共同使用的全局方法 / 全局变量

// 不extern 只能在定义的文件使用
extern std::regex N;
extern std::regex T;							//正则

extern char lineToken[50];						//记录文法结构(记录一行)
extern char expLineToken[50];					//记录表达式结构（一行）

//文法结构
extern struct grammarStruct {
	std::string S;
	std::set<std::string>Vt;
	std::unordered_map<std::string, int>Vn;
	std::unordered_map<std::string, std::vector<std::string>>P;

	grammarStruct() {};
};

//得到完整符号
std::string getFullChar(std::string candidate, int pos);

//更新指针为 下一个非空字符
int getBC(int pos, char line[]);