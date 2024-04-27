#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

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