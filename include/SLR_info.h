#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>

//项目
struct item {
	string Proleft;
	string ProCandidate;
	int point;

	//重载!=
	bool operator != (const item& backitem)const {
		if (this->Proleft != backitem.Proleft ||
			this->ProCandidate != backitem.ProCandidate ||
			this->point != backitem.point) {
			return true;
		}
		return false;
	}
	item() :Proleft(""), ProCandidate(""), point(-1) {}
	//用产生式 + 点位置构造item
	item(string Pleft, string PCan, int p)
		:Proleft(Pleft), ProCandidate(PCan), point(p) {}
};

//项目集节点
struct items_Node {
	int state_id;			//序号项目集
	vector<item>items_Set;	//节点中所有项目
	int originNum;			//记录初始项目数量

	//无参构造
	items_Node() :state_id(0), items_Set(0), originNum(0) {}
	//用已有项目集做含参构造
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

	//读入SLR 文法
	bool readSLRGrammar();
	//求项目集的闭包
	items_Node itemsNodeClosure(const items_Node& oriNode);
	//初始项目集 接收x的goto项目集
	items_Node itemsNodeGoto(const items_Node& oriNode, string X);
	//检测两个项目集是否相同
	bool itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode);
	//SLR分析表的构造
	void SLRAnaTableConstruct(items_Node& oriNode);
	//VTN 取出表达式中Vt和Vn
	void constructVTN();
	//格式化输出分析表
	void printSLRTabel();
	//SLR分析预测程序
	void SLR_predict(string inputExpression);
	//SLR分析 并输出栈
	void SLR_predict_AnalyseStack(string inputExpression);
	//启动SLR分析
	void run_ReadExp_SLR();

	//文件扫描录入文法
	void scan(char lineToken[]);
	//计算空集
	void calculate_NullAble();
	//计算First集合
	void calculate_First();
	//计算Follow集合
	void calculate_Follow();
private:
	//文法结构
	struct grammarStruct {
		string S;
		set<string>Vt;
		unordered_map<string, int>Vn;
		unordered_map<string, vector<string>>P;

		grammarStruct() {};
		//因为不能在 类中结构体外 初始化结构体
		grammarStruct(const string& s, const set<string>& vt, const unordered_map<string, int>& vn, const unordered_map<string, vector<string>>& p)
			: S(s), Vt(vt), Vn(vn), P(p) {}
	};

	const char* readSLRGrammarFile = "./src/textFile/SLR/SLRGrammarText.txt";
	const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";

	char lineToken[50] = {};		//记录文法结构(记录一行)
	char expLineToken[50] = {};		//记录表达式结构（一行）
	vector<string>Productions;
	unordered_set<string>NullAble;
	unordered_map<string, unordered_set<string>>First, Follow;

	vector<items_Node>itemsNodeSet{};		//所有项目集
	unordered_map<int, vector<pair<string, string>>>Action;		//Action表
	unordered_map<int, vector<pair<string, string>>>Goto;		//Goto表
	stack<int>State_Stack;					//状态栈	int型方便处理 多位数状态 
	stack<string>Symbol_Stack;				//符号栈
	vector<string>VNT;

};