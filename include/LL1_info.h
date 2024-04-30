#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<stack>
#include<utility>					//pair��
#include"../include/Common_data.h"

class LL1Class {
public:
	const char* readLL1GrammarFile = "./src/textFile/LL1/LL1GrammarText.txt";
	const char* writeLL1TransResultFile = "./src/textFile/LL1/LL1transResult.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	LL1Class() {};
	// ���캯��
	/*LL1Class(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
		: grammar(s, vt, vn, p) {
	}*/

	//LL1�ķ�����
	bool readLL1Grammar();
	//�ļ�ɨ��¼���ķ�
	void scan(char lineToken[]);
	//��ʽ������ļ�
	void formatPrint();
	//��ʽ������ķ����ļ�
	void formatPrintLL1IntoFile();
	//����ֱ����ݹ�
	void directLeftRecursion(std::string proLeft, std::vector<std::string>proRight);
	//���� �����ݹ�	
	void remove_left_recursion();
	//��ȡ������
	void remove_left_gene();
	//����LL1����
	void run_ReadExp_LL1();
	//����ռ�
	void calculate_NullAble();
	//����First����
	void calculate_First();
	//����Follow����
	void calculate_Follow();
	//Ԥ�������Ĺ���
	void construct_LL1Table();
	//Ԥ�����
	bool LL1_predict(std::string inputExpression);

	//�ķ��ṹ
	grammarStruct grammar;
private:

	std::unordered_set<std::string>NullAble;
	std::unordered_map<std::string, std::unordered_set<std::string>>First, Follow;
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>LL1_table;
	std::stack<std::string>analyseStack;
	std::vector<std::string>Productions;
};