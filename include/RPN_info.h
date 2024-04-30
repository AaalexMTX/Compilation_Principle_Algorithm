#pragma once
#include<queue>
#include<stack>
#include"../include/Common_data.h"


class RPNClass {
public:
	const char* readRPNGrammarFile = "./src/textFile/RPN/RPNGrammarText.txt";
	const char* readRPNExpressionFile = "./src/textFile/RPN/RPNExpressionWord.txt";
	//�ķ��ṹ
	grammarStruct grammar;
	std::vector<std::string>Productions;

	RPNClass() {};

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

	//����ռ�
	void calculate_NullAble();
	//����First����
	void calculate_First();
	//����Follow����
	void calculate_Follow();

	//���嶯��
	void semanticAction(int actionIndex, std::pair<std::string, std::string>& proLeftNode);
	//�����沨��ʽ������ֵ
	int calcReversePolistNotation(std::string& RPN);
	//�沨��ʽ������� ����ջ
	void SLR_predict_SemanticAnalyse_AnalyseStack(const char* line);
	//����RPN�����������������
	void runn_ReadExp_RPN();


private:

	std::unordered_set<std::string>NullAble;
	std::unordered_map<std::string, std::unordered_set<std::string>>First, Follow;

	std::vector<items_Node>itemsNodeSet{};			//������Ŀ��
	std::unordered_map<int, std::vector<std::pair<std::string, std::string>>>Action;		//Action��
	std::unordered_map<int, std::vector<std::pair<std::string, std::string>>>Goto;		//Goto��
	std::stack<int>State_Stack;						//״̬ջ	int�ͷ��㴦�� ��λ��״̬ 
	std::stack<std::string>Symbol_Stack;			//����ջ
	std::vector<std::string>VNT;

	std::stack<std::pair<std::string, std::string>>Symbol_Stack_Semantic;	//����ջ���������
	std::stack<std::string>Semantic_Stack;			//����ջ
};