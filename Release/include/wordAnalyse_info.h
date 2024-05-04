#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include"../include/Common_data.h"

extern std::regex delimiterPattern;			//�������
extern std::regex operatePattern;			//���������
extern const char* readExpressionFile;
char keyWords[][10]; 

//�ж��ַ��Ƿ�Ϊ����
bool isNumber(char& c);

//�ж��ַ��Ƿ�Ϊ����
bool isLetter(char& c);

//���ر��ʽ ��һ��ʼλ��
int getBCExp(char line[], int pos);

//����һ�� ���沨��ʽ��
int getBCExp(std::string line, int pos);

//�ʷ�������Version1.0ʵ��1�ã��߷��������
void wordChange1();

//���ʷ��ļ�
bool readWordFile(const char* GrammarFile);

/*
@brief //�ʷ�������Version2.0LL1�ã����ʽ����ת��
@param 1+2;
@return i+i;
*/
std::string ExpChange(char line[]);

//�ʷ�������Version3.0RPN�ã������沨��ʽ�Ĵʷ�����
std::pair<std::string, std::string> wordChange(std::string& line, int& linePf);
