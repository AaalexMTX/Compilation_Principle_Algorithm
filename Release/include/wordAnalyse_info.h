#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include"../include/Common_data.h"

extern std::regex delimiterPattern;			//界符正则
extern std::regex operatePattern;			//运算符正则
extern const char* readExpressionFile;
char keyWords[][10]; 

//判断字符是否为数字
bool isNumber(char& c);

//判断字符是否为单词
bool isLetter(char& c);

//返回表达式 下一起始位置
int getBCExp(char line[], int pos);

//重载一个 做逆波兰式用
int getBCExp(std::string line, int pos);

//词法分析（Version1.0实验1用）边分析边输出
void wordChange1();

//读词法文件
bool readWordFile(const char* GrammarFile);

/*
@brief //词法分析（Version2.0LL1用）表达式类型转换
@param 1+2;
@return i+i;
*/
std::string ExpChange(char line[]);

//词法分析（Version3.0RPN用）适配逆波兰式的词法分析
std::pair<std::string, std::string> wordChange(std::string& line, int& linePf);
