#pragma once
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

//文法结构
struct grammarStruct {
	std::string S;
	std::set<std::string>Vt;
	std::unordered_map<std::string, int>Vn;
	std::unordered_map<std::string, std::vector<std::string>>P;

	//grammarStruct() {};
	//grammarStruct(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
	//	: S(s), Vt(vt), Vn(vn), P(p) {}
};

//得到完整符号
std::string getFullChar(std::string candidate, int pos);

//更新指针为 下一个非空字符
int getBC(int pos, char line[]);