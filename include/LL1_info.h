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
#include<utility>		//pair��
using namespace std;

//�ķ��ṹ
struct grammerStruct {
	string S;
	set<string>Vt;
	unordered_map<string, int>Vn;
	unordered_map<string, vector<string>>P;
};

//�ķ�����
void readGrammar();

//LL1�ķ�����
void readLL1Grammar();

//SLR�ķ�����
void readSLRGrammar();

//��ʽ������ķ����ļ�
void formatPrintIntoFile();

//��ʽ������ķ����ļ�
void formatPrintLL1IntoFile();

//��ʽ������ķ�
void formatPrint();

//����ָ��Ϊ ��һ���ǿ��ַ�
int getBC(int pos, char line[]);

//�ļ�ɨ��¼���ķ�
void scan(char lineToken[]);

//���� ����ʽ��ֱ����ݹ�
void directLeftRecursion(string proLeft, vector<string>proRight);

//���� �����ݹ�	
void remove_left_recursion();

//��ȡ������
void remove_left_gene();

//�õ���������
string getFullChar(string candidate, int pos);

//����ռ�
void calculate_NullAble();

//����First����
void calculate_First();

//����Follow����
void calculate_Follow();

//Ԥ�������Ĺ���
void construct_LL1Table();

//Ԥ�����
bool LL1_predict(string inputExpression);