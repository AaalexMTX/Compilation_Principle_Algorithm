#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<stack>
#include<unordered_map>
#include<unordered_set>
#include<regex>
#include<utility>
using namespace std;

//文法结构
struct grammerStruct {
	string S;
	set<string>Vt;
	unordered_map<string, int>Vn;
	unordered_map<string, vector<string>>P;
};

//文法读入
void readGrammar();

//格式化输出文法进文件
void formatPrintIntoFile();

//格式化输出文法
void formatPrint();

//更新指针为 下一个非空字符
int getBC(int pos, char line[]);

//文件扫描录入文法
void scan(char lineToken[]);

//消除 产生式的直接左递归
void directLeftRecursion(string proLeft, vector<string>proRight);

//消除 间接左递归
void remove_left_recursion();

//提取左因子
void remove_left_gene();

//得到完整符号
string getFullChar(string candidate, int pos);

//计算空集
void calculate_nullAble();

//计算first集合
void calculate_first();

//计算follow集合
void calculate_follow();

//预测分析表的构造
void construct_LL1Table();

//预测程序
bool LL1_predict(string inputExpression);