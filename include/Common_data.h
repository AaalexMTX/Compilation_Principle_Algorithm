#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<algorithm>
#include<iostream>
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

//--词法分析
//得到完整符号
std::string getFullChar(std::string candidate, int pos);

//更新指针为 下一个非空字符
int getBC(int pos, char line[]);

//--文法结构
extern struct grammarStruct {
	std::string S;
	std::set<std::string>Vt;
	std::unordered_map<std::string, int>Vn;
	std::unordered_map<std::string, std::vector<std::string>>P;

	grammarStruct() {};
};

//文件中读出文法
bool readGrammar(const char* GrammarFile, grammarStruct& grammar, std::vector<std::string>& Productions);

//文件扫描表达式录入文法
void scanCandidateIntoGrammar(char line[], grammarStruct& grammar, std::vector<std::string>& Productions);

//--SLR
//项目 -- 只是SLR分析的工具 SLR对象不持有
struct item {
	std::string Proleft;
	std::string ProCandidate;
	int point;

	//重载!=
	bool operator != (const item& backitem)const {
		if (this->Proleft != backitem.Proleft ||
			this->ProCandidate != backitem.ProCandidate ||
			this->point != backitem.point) {
			return true;
		}
		return false;
	}
	item() :Proleft(""), ProCandidate(""), point(-1) {}
	//用产生式 + 点位置构造item
	item(std::string Pleft, std::string PCan, int p)
		:Proleft(Pleft), ProCandidate(PCan), point(p) {}
};

//项目集节点
struct items_Node {
	int state_id;			//序号项目集
	std::vector<item>items_Set;	//节点中所有项目
	int originNum;			//记录初始项目数量

	//无参构造
	items_Node() :state_id(0), items_Set(0), originNum(0) {}
	//用已有项目集做含参构造
	items_Node(const items_Node& extendNode) {
		this->state_id = extendNode.state_id;
		for (item i : extendNode.items_Set) {
			this->items_Set.push_back(i);
		}
		this->originNum = extendNode.originNum;
	}
	items_Node(const std::vector<item>& a) {
		this->state_id = 0;
		for (item i : a) {
			this->items_Set.push_back(i);
		}
		this->originNum = a.size();
	}
};
