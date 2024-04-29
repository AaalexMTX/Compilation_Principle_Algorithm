#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring>
#include<regex>

//判断字符是否为数字
bool isNumber(char& c);

//判断字符是否为单词
bool isLetter(char& c);

//返回表达式 下一起始位置
int getBCExp(char line[], int pos);

//文件读入表达式并进行LL1分析
//void run_ReadExp_LL1();

//文件读入表达式 并进行SLR分析
//void run_ReadExp_SLR();

//表达式类型转换
std::string ExpChange(char line[]);