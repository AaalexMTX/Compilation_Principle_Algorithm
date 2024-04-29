#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>

//��Ŀ
struct item {
	string Proleft;
	string ProCandidate;
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
	item(string Pleft, string PCan, int p)
		:Proleft(Pleft), ProCandidate(PCan), point(p) {}
};

//��Ŀ���ڵ�
struct items_Node {
	int state_id;			//�����Ŀ��
	vector<item>items_Set;	//�ڵ���������Ŀ
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
	items_Node(const vector<item>& a) {
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
	items_Node itemsNodeGoto(const items_Node& oriNode, string X);
	//���������Ŀ���Ƿ���ͬ
	bool itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode);
	//SLR������Ĺ���
	void SLRAnaTableConstruct(items_Node& oriNode);
	//VTN ȡ�����ʽ��Vt��Vn
	void constructVTN();
	//��ʽ�����������
	void printSLRTabel();
	//SLR����Ԥ�����
	void SLR_predict(string inputExpression);
	//SLR���� �����ջ
	void SLR_predict_AnalyseStack(string inputExpression);
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

	const char* readSLRGrammarFile = "./src/textFile/SLR/SLRGrammarText.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	char lineToken[50] = {};		//��¼�ķ��ṹ(��¼һ��)
	char expLineToken[50] = {};		//��¼���ʽ�ṹ��һ�У�
	vector<string>Productions;
	unordered_set<string>NullAble;
	unordered_map<string, unordered_set<string>>First, Follow;

	vector<items_Node>itemsNodeSet{};		//������Ŀ��
	unordered_map<int, vector<pair<string, string>>>Action;		//Action��
	unordered_map<int, vector<pair<string, string>>>Goto;		//Goto��
	stack<int>State_Stack;					//״̬ջ	int�ͷ��㴦�� ��λ��״̬ 
	stack<string>Symbol_Stack;				//����ջ
	vector<string>VNT;

};