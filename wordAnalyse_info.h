#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring>
#include<regex>
using namespace std;

//�ж��ַ��Ƿ�Ϊ����
bool isNumber(char& c);

//�ж��ַ��Ƿ�Ϊ����
bool isLetter(char& c);

//���ر��ʽ ��һ��ʼλ��
int getBCExp(char line[], int pos);

void ReadExpression();

//���ʽ����ת��
string ExpChange(char line[]);