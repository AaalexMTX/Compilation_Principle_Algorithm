#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring>
#include<regex>

//�ж��ַ��Ƿ�Ϊ����
bool isNumber(char& c);

//�ж��ַ��Ƿ�Ϊ����
bool isLetter(char& c);

//���ر��ʽ ��һ��ʼλ��
int getBCExp(char line[], int pos);

//�ļ�������ʽ������LL1����
//void run_ReadExp_LL1();

//�ļ�������ʽ ������SLR����
//void run_ReadExp_SLR();

//���ʽ����ת��
std::string ExpChange(char line[]);