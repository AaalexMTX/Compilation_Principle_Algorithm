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

//�õ���������
string getFullChar(string candidate, int pos);

//����ָ��Ϊ ��һ���ǿ��ַ�
int getBC(int pos, char line[]);

////�ķ�����
//void readGrammar();
//
////LL1�ķ�����  ---
//bool readLL1Grammar();
//
////SLR�ķ�����
//bool readSLRGrammar();
//
////��ʽ������ķ����ļ�
//void formatPrintIntoFile();
//
////��ʽ������ķ����ļ�
//void formatPrintLL1IntoFile();
//
////��ʽ������ķ�
//void formatPrint();
//
////�ļ�ɨ��¼���ķ�
//void scan(char lineToken[]);
//
////���� ����ʽ��ֱ����ݹ�	
//void directLeftRecursion(string proLeft, vector<string>proRight);
//
////���� �����ݹ�	
//void remove_left_recursion();
//
////��ȡ������
//void remove_left_gene();
//
////����ռ�
//void calculate_NullAble();
//
////����First����
//void calculate_First();
//
////����Follow����
//void calculate_Follow();
//
////Ԥ�������Ĺ���
//void construct_LL1Table();
//
////Ԥ�����
//bool LL1_predict(string inputExpression);

class LL1Class {
public:
	const char* readLL1GrammarFile = "./src/textFile/LL1/LL1grammarText.txt";
	const char* writeLL1TransResultFile = "./src/textFile/LL1/LL1transResult.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	LL1Class(){};
	// ���캯��
	LL1Class(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
		: grammar(s, vt, vn, p) {
	}
	
	//LL1�ķ�����
	bool readLL1Grammar();					
	//�ļ�ɨ��¼���ķ�
	void scan(char lineToken[]);
	//��ʽ������ļ�
	void formatPrint();
	//��ʽ������ķ����ļ�
	void formatPrintLL1IntoFile();
	//����ֱ����ݹ�
	void directLeftRecursion(string proLeft, vector<string>proRight);
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
	bool LL1_predict(string inputExpression);

private:
	//�ķ��ṹ
	struct grammarStruct {
		string S;
		set<string>Vt;
		unordered_map<string, int>Vn;
		unordered_map<string, vector<string>>P;

		grammarStruct() {};
		//��Ϊ������ ���нṹ���� ��ʼ���ṹ��
		grammarStruct(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
			: S(s), Vt(vt), Vn(vn), P(p) {}
	};
	grammarStruct grammar;
	char lineToken[50] = {};		//��¼�ķ��ṹ(��¼һ��)
	char expLineToken[50] = {};		//��¼���ʽ�ṹ��һ�У�
	unordered_set<string>NullAble;
	unordered_map<string, unordered_set<string>>First, Follow;
	unordered_map<string, vector<pair<string, string>>>LL1_table;
	stack<string>analyseStack;
	vector<string>Productions;
};