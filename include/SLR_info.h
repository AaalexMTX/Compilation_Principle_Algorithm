#pragma once
#include<iostream>
#include<queue>
#include"../include/Common_data.h"


//��Ŀ
struct item {
	std::string Proleft;
	std::string ProCandidate;
	int point;

	//����!=
	bool operator != (const item& backitem)const {
		if (this->Proleft != backitem.Proleft ||
			this->ProCandidate != backitem.ProCandidate ||
			this->point != backitem.point) {
			return true;
		}
		return false;
	}
	item() :Proleft(""), ProCandidate(""), point(-1) {}
	//�ò���ʽ + ��λ�ù���item
	item(std::string Pleft, std::string PCan, int p)
		:Proleft(Pleft), ProCandidate(PCan), point(p) {}
};

//��Ŀ���ڵ�
struct items_Node {
	int state_id;			//�����Ŀ��
	std::vector<item>items_Set;	//�ڵ���������Ŀ
	int originNum;			//��¼��ʼ��Ŀ����

	//�޲ι���
	items_Node() :state_id(0), items_Set(0), originNum(0) {}
	//��������Ŀ�������ι���
	items_Node(const items_Node& extendNode) {
		this->state_id = extendNode.state_id;
		for (item i : extendNode.items_Set) {
			this->items_Set.push_back(i);
		}
		this->originNum = extendNode.originNum;
	}
	items_Node(const std::vector<item>& a) {
		this->state_id = 0;
		for (item i : a) {
			this->items_Set.push_back(i);
		}
		this->originNum = a.size();
	}
};

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