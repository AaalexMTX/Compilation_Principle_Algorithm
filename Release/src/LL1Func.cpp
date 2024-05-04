#include"../include/LL1_info.h"
#include"../include/wordAnalyse_info.h"
#include"../include/Common_data.h"
using namespace std;

void LL1Class::directLeftRecursion(string proLeft, vector<string>proRight) {
	vector<string>originPro, newPro;
	for (vector<string>::iterator it = proRight.begin(); it < proRight.end(); it++) {
		//测左递归
		if ((*it).find(proLeft) == 0) {
			//取 候选式非左侧子串的部分
			string backSub = (*it).substr(proLeft.length());
			newPro.push_back(backSub);
		}
		else {
			originPro.push_back(*it);
		}
	}
	//非空说明含有左递归 更新文法
	if (!newPro.empty()) {
		//清空旧产生式
		grammar.P[proLeft].clear();

		//更新分号的数量统一存在 A中  如 A' A''  -> A中记录下次添加'的数量
		unordered_map<string, int>::iterator itProLeft = grammar.Vn.find(string(1, proLeft[0]));
		string FHCount = string(itProLeft->second, '\'');
		itProLeft->second++;
		//加了新非终结符A'  更新Vn 
		grammar.Vn[proLeft + FHCount] = 1;

		bool endInsertEmpty = false;
		for (vector<string>::iterator it = originPro.begin(); it < originPro.end(); it++) {
			//不能插入形如 A-> @A'
			if ((*it) == "@") { endInsertEmpty = true; }
			else {
				// A->e  -- A->eA'
				grammar.P[proLeft].push_back((*it) + proLeft + FHCount);
			}
		}
		//特意将 @更新至末尾
		if (endInsertEmpty) { grammar.P[proLeft].push_back("@"); }
		for (vector<string>::iterator it = newPro.begin();it < newPro.end(); it++) {
			// A->Abc  --  A'->bcA'
			grammar.P[proLeft + FHCount].push_back((*it) + proLeft + FHCount);
		}
		//末尾添加空字
		grammar.P[proLeft + FHCount].push_back("@");
	}
}

void LL1Class::remove_left_recursion() {
	for (unordered_map<string, vector<string>>::iterator itI = grammar.P.begin(); itI != grammar.P.end(); itI++) {
		//对产生式itI尝试代入 其他所有itJ
		for (unordered_map<string, vector<string>>::iterator itJ = grammar.P.begin(); itJ != itI;itJ++) {
			if (itJ == itI) { continue; }
			//接收可能发生代入生成的新产生式
			vector<string> newPro;

			//itIK遍历itI的vector候选式数组 对每一个候选式尝试代入ItJ左部
			for (vector<string>::iterator itIK = itI->second.begin();itIK < itI->second.end(); itIK++) {
				//若产生间接左递归
				if ((*itIK).find(itJ->first) == 0) {
					//将itJ所有候选式代入该*itIK
					for (vector<string>::iterator itJK = itJ->second.begin(); itJK != itJ->second.end();itJK++) {
						newPro.push_back(*itJK + (*itIK).substr(itJ->first.length()));
					}
				}
				//对itI中不以 itJ开头的直接放入newPro
				else {
					newPro.push_back(*itIK);
				}
			}
			itI->second = newPro;
		}
		//消除直接左递归
		directLeftRecursion(itI->first, itI->second);
	}
}

void LL1Class::remove_left_gene() {
	unordered_map<string, vector<string>>candidate;
	//遍历文法的所有产生式
	for (unordered_map<string, vector<string>>::iterator itI = grammar.P.begin();itI != grammar.P.end();itI++) {
		candidate.clear();
		//遍历所有候选式
		for (vector<string>::iterator itIK = itI->second.begin();itIK < itI->second.end();itIK++) {
			//以开头字符为键
			string startStr = string(1, (*itIK)[0]);
			//后续字符串为值 录入candidate
			string subVal = (*itIK).substr(1);
			candidate[startStr].push_back(subVal);
		}

		itI->second.clear();
		//遍该存候选式的map 测是否需要提公因式
		for (unordered_map<string, vector<string>>::iterator itJ = candidate.begin();itJ != candidate.end();itJ++) {
			if (itJ->second.size() == 1) {
				// A-> r1|r2  -- A->r1|r2 
				itI->second.push_back(itJ->first + itJ->second[0]);
			}
			else {
				//进了else 就表示 需要提公因式
				unordered_map<string, int>::iterator itProLeft = grammar.Vn.find(string(1, itI->first[0]));
				string FHCount = string(itProLeft->second, '\'');
				itProLeft->second++;
				//加了新非终结符A'  更新Vn 
				string newVN = itI->first[0] + FHCount;
				grammar.Vn[newVN] = 1;

				//给A 加上 aA' 项
				itI->second.push_back(itJ->first + itProLeft->first + FHCount);
				bool endInsertEmpty = false;
				//对所有有公因式的候选式
				for (vector<string>::iterator itJK = itJ->second.begin();itJK != itJ->second.end();itJK++) {
					//A->ab1|ab2|a  --	A'->b1|b2|@
					if (*itJK != "") {
						grammar.P[newVN].push_back(*itJK);
					}
					else { endInsertEmpty = true; }
				}
				if (endInsertEmpty) { grammar.P[newVN].push_back("@"); }
			}
		}
	}
}

void LL1Class::construct_LL1Table() {
	for (unordered_map<string, vector<string>>::iterator itP = grammar.P.begin(); itP != grammar.P.end();itP++) {
		for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
			//遍历产生式每个符号
			for (int i = 0;i < (*itCandidate).length();) {
				string fullChar = getFullChar(*itCandidate, i);

				//终结符
				if (grammar.Vn.find(fullChar) != grammar.Vn.end()) {
					//将M 的First集 加入 分析表
					for (unordered_set<string>::iterator itfullCharFirst = First[fullChar].begin(); itfullCharFirst != First[fullChar].end(); itfullCharFirst++) {
						LL1_table[itP->first].push_back({ *itfullCharFirst, *itCandidate });
					}
					//非空 不继续遍历候选式了
					if (NullAble.find(fullChar) == NullAble.end()) { break; }
				}
				//非终结符
				else if (grammar.Vt.find(fullChar) != grammar.Vt.end()) {
					LL1_table[itP->first].push_back({ fullChar, *itCandidate });
					break;
				}
				//@
				else {
					//将M 的Follow集 加入 分析表
					for (unordered_set<string>::iterator itfullCharFollow = Follow[itP->first].begin(); itfullCharFollow != Follow[itP->first].end();itfullCharFollow++) {
						LL1_table[itP->first].push_back({ *itfullCharFollow, *itCandidate });
					}
				}
				i += fullChar.length();
			}
		}
	}

	cout << endl << " ---- LL1_table ----  " << endl;
	for (unordered_map<string, vector<pair<string, string>>>::iterator a = LL1_table.begin(); a != LL1_table.end(); a++) {
		cout << a->first << " -- ";
		for (vector<pair<string, string>>::iterator b = a->second.begin(); b != a->second.end(); b++) {
			cout << b->first << "," << b->second << "  ";
		}
		cout << endl;
	}
}

bool LL1Class::LL1_predict(string inputExpression) {
	while (!analyseStack.empty()) { analyseStack.pop(); }
	analyseStack.push("#");
	analyseStack.push(grammar.S);

	inputExpression += "#";

	while (!analyseStack.empty()) {
		string top = analyseStack.top();
		analyseStack.pop();
		string inputExpTop = inputExpression.substr(0, 1);
		//终结符
		if (grammar.Vn.find(top) != grammar.Vn.end()) {
			string predictString = "";
			for (vector<pair<string, string>>::iterator itLL1Table = LL1_table[top].begin();
				itLL1Table != LL1_table[top].end(); itLL1Table++) {
				if ((*itLL1Table).first == inputExpTop) {
					predictString = (*itLL1Table).second;
					break;
				}
			}
			if (predictString == "@") { continue; }

			//LL(1)中存在对应关系则入栈候选式 否则错误
			if (predictString != "") {
				//反向入栈
				string fullChar;
				for (int i = predictString.length() - 1; i >= 0;i--) {
					if (predictString[i] == '\'') { continue; }
					fullChar = getFullChar(predictString, i);
					analyseStack.push(fullChar);
				}
			}
			else { return false; }

		}
		else {
			//相同非终极符同时出栈 否则错误 
			if (top == inputExpTop) {
				inputExpression = inputExpression.substr(1);
			}
			else { return false; }
		}
	}
	return inputExpression.empty();
}

void LL1Class::run_ReadExp_LL1() {
	FILE* fp = fopen(readLL1ExpressionFile, "r");
	if (fp == NULL) {
		cout << "Expression File Not Exist";
	}
	else {
		while (fgets(expLineToken, 50, fp) != NULL) {
			cout << expLineToken;
			if (LL1_predict(ExpChange(expLineToken))) { cout << "YES" << endl; }
			else cout << "NO" << endl;
		}
		fclose(fp);
	}
}
