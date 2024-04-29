#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<stack>
#include<unordered_map>
#include<unordered_set>
#include<regex>
#include<utility>		//pair对
using namespace std;

//文法结构
struct grammerStruct {
	string S;
	set<string>Vt;
	unordered_map<string, int>Vn;
	unordered_map<string, vector<string>>P;
};

//得到完整符号
string getFullChar(string candidate, int pos);

//更新指针为 下一个非空字符
int getBC(int pos, char line[]);

////文法读入
//void readGrammar();
//
////LL1文法读入  ---
//bool readLL1Grammar();
//
////SLR文法读入
//bool readSLRGrammar();
//
////格式化输出文法进文件
//void formatPrintIntoFile();
//
////格式化输出文法进文件
//void formatPrintLL1IntoFile();
//
////格式化输出文法
//void formatPrint();
//
////文件扫描录入文法
//void scan(char lineToken[]);
//
////消除 产生式的直接左递归	
//void directLeftRecursion(string proLeft, vector<string>proRight);
//
////消除 间接左递归	
//void remove_left_recursion();
//
////提取左因子
//void remove_left_gene();
//
////计算空集
//void calculate_NullAble();
//
////计算First集合
//void calculate_First();
//
////计算Follow集合
//void calculate_Follow();
//
////预测分析表的构造
//void construct_LL1Table();
//
////预测程序
//bool LL1_predict(string inputExpression);

class LL1Class {
public:
	const char* readLL1GrammarFile = "./src/textFile/LL1/LL1grammarText.txt";
	const char* writeLL1TransResultFile = "./src/textFile/LL1/LL1transResult.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	LL1Class(){};
	// 构造函数
	LL1Class(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
		: grammar(s, vt, vn, p) {
	}
	
	//LL1文法读入
	bool readLL1Grammar();					
	//文件扫描录入文法
	void scan(char lineToken[]);
	//格式化输出文件
	void formatPrint();
	//格式化输出文法进文件
	void formatPrintLL1IntoFile();
	//消除直接左递归
	void directLeftRecursion(string proLeft, vector<string>proRight);
	//消除 间接左递归	
	void remove_left_recursion();
	//提取左因子
	void remove_left_gene();
	//启动LL1分析
	void run_ReadExp_LL1();					
	//计算空集
	void calculate_NullAble();
	//计算First集合
	void calculate_First();
	//计算Follow集合
	void calculate_Follow();
	//预测分析表的构造
	void construct_LL1Table();
	//预测程序
	bool LL1_predict(string inputExpression);

private:
	//文法结构
	struct grammarStruct {
		string S;
		set<string>Vt;
		unordered_map<string, int>Vn;
		unordered_map<string, vector<string>>P;

		grammarStruct() {};
		//因为不能在 类中结构体外 初始化结构体
		grammarStruct(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
			: S(s), Vt(vt), Vn(vn), P(p) {}
	};
	grammarStruct grammar;
	char lineToken[50] = {};		//记录文法结构(记录一行)
	char expLineToken[50] = {};		//记录表达式结构（一行）
	unordered_set<string>NullAble;
	unordered_map<string, unordered_set<string>>First, Follow;
	unordered_map<string, vector<pair<string, string>>>LL1_table;
	stack<string>analyseStack;
	vector<string>Productions;
};