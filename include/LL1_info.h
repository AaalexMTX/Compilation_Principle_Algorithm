#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<stack>
#include<utility>					//pair对
#include"../include/Common_data.h"

class LL1Class {
public:
	const char* readLL1GrammarFile = "./src/textFile/LL1/LL1GrammarText.txt";
	const char* writeLL1TransResultFile = "./src/textFile/LL1/LL1transResult.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	LL1Class() {};
	// 构造函数
	/*LL1Class(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
		: grammar(s, vt, vn, p) {
	}*/

	//LL1文法读入
	bool readLL1Grammar();
	//文件扫描录入文法
	void scan(char lineToken[]);
	//格式化输出文件
	void formatPrint();
	//格式化输出文法进文件
	void formatPrintLL1IntoFile();
	//消除直接左递归
	void directLeftRecursion(std::string proLeft, std::vector<std::string>proRight);
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
	bool LL1_predict(std::string inputExpression);

	//文法结构
	grammarStruct grammar;
private:

	std::unordered_set<std::string>NullAble;
	std::unordered_map<std::string, std::unordered_set<std::string>>First, Follow;
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>LL1_table;
	std::stack<std::string>analyseStack;
	std::vector<std::string>Productions;
};