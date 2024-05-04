#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<stack>
#include<utility>					//pair��
#include"../include/Common_data.h"

class LL1Class: public Grammar_set {
public:
	//LL1�������ļ�
	const char* readLL1GrammarFile = "./src/textFile/LL1/LL1GrammarText.txt";
	const char* readLL1ExpressionFile = "./src/textFile/LL1/LL1ExpressionWord.txt";
	//��LL1Ԥ����������ݹ�+�ṫ��ʽ�����ļ�
	const char* readtransLL1GrammarFile = "./src/textFile/LL1/LL1transGrammarText.txt";
	const char* writeLL1TransResultFile = "./src/textFile/LL1/LL1transResult.txt";

	LL1Class() {};
	// ���캯��
	/*LL1Class(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
		: grammar(s, vt, vn, p) {
	}*/

	//����ֱ����ݹ�
	void directLeftRecursion(std::string proLeft, std::vector<std::string>proRight);
	//���� �����ݹ�	
	void remove_left_recursion();
	//��ȡ������
	void remove_left_gene();
	//����LL1����
	void run_ReadExp_LL1();
	
	//Ԥ�������Ĺ���
	void construct_LL1Table();
	//Ԥ�����
	bool LL1_predict(std::string inputExpression);

private:

	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>LL1_table;
	std::stack<std::string>analyseStack;
};