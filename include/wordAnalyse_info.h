#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring>
#include<regex>
#include"../include/Common_data.h"


//�ж��ַ��Ƿ�Ϊ����
bool isNumber(char& c);

//�ж��ַ��Ƿ�Ϊ����
bool isLetter(char& c);

//���ر��ʽ ��һ��ʼλ��
int getBCExp(char line[], int pos);

//����һ�� ���沨��ʽ��
int getBCExp(std::string line, int pos);

//���ʽ����ת��
std::string ExpChange(char line[]);

//�����沨��ʽ�Ĵʷ�����
std::pair<std::string, std::string> wordChange(std::string& line, int& linePf);
