#pragma once
#include<queue>
#include<stack>
#include"../include/Common_data.h"


class RPNClass {
public:
	const char* readRPNGrammarFile = "./src/textFile/RPN/RPNGrammarText.txt";
	const char* readRPNExpressionFile = "./src/textFile/RPN/RPNExpressionWord.txt";
	//文法结构
	grammarStruct grammar;
	std::vector<std::string>Productions;

	RPNClass() {};

	//求项目集的闭包
	items_Node itemsNodeClosure(const items_Node& oriNode);
	//初始项目集 接收x的goto项目集
	items_Node itemsNodeGoto(const items_Node& oriNode, std::string X);
	//检测两个项目集是否相同
	bool itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode);
	//SLR分析表的构造
	void SLRAnaTableConstruct(items_Node& oriNode);
	//VTN 取出表达式中Vt和Vn
	void constructVTN();
	//格式化输出分析表
	void printSLRTabel();
	//SLR分析预测程序
	void SLR_predict(std::string inputExpression);
	//SLR分析 并输出栈
	void SLR_predict_AnalyseStack(std::string inputExpression);
	//启动SLR分析
	void run_ReadExp_SLR();

	//计算空集
	void calculate_NullAble();
	//计算First集合
	void calculate_First();
	//计算Follow集合
	void calculate_Follow();

	//语义动作
	void semanticAction(int actionIndex, std::pair<std::string, std::string>& proLeftNode);
	//计算逆波兰式的语义值
	int calcReversePolistNotation(std::string& RPN);
	//逆波兰式语义分析 分析栈
	void SLR_predict_SemanticAnalyse_AnalyseStack(const char* line);
	//开启RPN分析（可以提出来）
	void runn_ReadExp_RPN();


private:

	std::unordered_set<std::string>NullAble;
	std::unordered_map<std::string, std::unordered_set<std::string>>First, Follow;

	std::vector<items_Node>itemsNodeSet{};			//所有项目集
	std::unordered_map<int, std::vector<std::pair<std::string, std::string>>>Action;		//Action表
	std::unordered_map<int, std::vector<std::pair<std::string, std::string>>>Goto;		//Goto表
	std::stack<int>State_Stack;						//状态栈	int型方便处理 多位数状态 
	std::stack<std::string>Symbol_Stack;			//符号栈
	std::vector<std::string>VNT;

	std::stack<std::pair<std::string, std::string>>Symbol_Stack_Semantic;	//符号栈语义分析版
	std::stack<std::string>Semantic_Stack;			//语义栈
};