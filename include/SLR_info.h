#pragma once
#include<iostream>
#include<queue>
#include<stack>
#include"../include/Common_data.h"

class SLRClass {
public:
	SLRClass() {};

	//����SLR �ķ�
	bool readSLRGrammar();
	//����Ŀ���ıհ�
	items_Node itemsNodeClosure(const items_Node& oriNode);
	//��ʼ��Ŀ�� ����x��goto��Ŀ��
	items_Node itemsNodeGoto(const items_Node& oriNode, std::string X);
	//���������Ŀ���Ƿ���ͬ
	bool itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode);
	//SLR������Ĺ���
	void SLRAnaTableConstruct(items_Node& oriNode);
	//VTN ȡ�����ʽ��Vt��Vn
	void constructVTN();
	//��ʽ�����������
	void printSLRTabel();
	//SLR����Ԥ�����
	void SLR_predict(std::string inputExpression);
	//SLR���� �����ջ
	void SLR_predict_AnalyseStack(std::string inputExpression);
	//����SLR����
	void run_ReadExp_SLR();

	//�ļ�ɨ��¼���ķ�
	void scan(char lineToken[]);
	//����ռ�
	void calculate_NullAble();
	//����First����
	void calculate_First();
	//����Follow����
	void calculate_Follow();

	
	//�ķ��ṹ
	grammarStruct grammar;

private:
	const char* readSLRGrammarFile = "./src/textFile/SLR/SLRGrammarText.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	std::vector<std::string>Productions;
	std::unordered_set<std::string>NullAble;
	std::unordered_map<std::string, std::unordered_set<std::string>>First, Follow;

	std::vector<items_Node>itemsNodeSet{};			//������Ŀ��
	std::unordered_map<int, std::vector<std::pair<std::string, std::string>>>Action;		//Action��
	std::unordered_map<int, std::vector<std::pair<std::string, std::string>>>Goto;		//Goto��
	std::stack<int>State_Stack;						//״̬ջ	int�ͷ��㴦�� ��λ��״̬ 
	std::stack<std::string>Symbol_Stack;					//����ջ
	std::vector<std::string>VNT;
};