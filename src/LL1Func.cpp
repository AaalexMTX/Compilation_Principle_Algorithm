#include"../include/LL1_info.h"
#include"../include/wordAnalyse_info.h"
#include"../include/Common_data.h"
using namespace std;

void LL1Class::formatPrint() {
	for (unordered_map<string, vector<string>>::iterator it = grammar.P.begin(); it != grammar.P.end(); it++) {
		cout << it->first << "->";
		for (vector<string>::iterator itVector = it->second.begin(); itVector != it->second.end();itVector++) {
			if (next(itVector) != it->second.end()) {
				cout << *itVector << "|";
			}
			else {
				cout << *itVector << ";" << endl;
			}
		}
	}
	cout << endl << "开始符号  S:  " << grammar.S;
	cout << endl << "  终结符 Vn:  ";
	for (unordered_map<string, int>::iterator it = grammar.Vn.begin(); it != grammar.Vn.end();it++) {
		cout << it->first << " ";
	}
	cout << endl << "非终结符 Vt:  ";
	for (set<string>::iterator it = grammar.Vt.begin(); it != grammar.Vt.end();it++) {
		cout << *it << " ";
	}
}

void LL1Class::formatPrintLL1IntoFile() {
	if (freopen(writeLL1TransResultFile, "w", stdout) != NULL) {
		//格式化输出文法
		formatPrint();
		//把标准输出改回控制台
		auto _ = freopen("CON", "w", stdout);
	}
	else {
		printf("LL1transResult.txt Not Exist");
	}
}

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

void LL1Class::calculate_NullAble() {
	bool NullAbleChange = true;
	while (NullAbleChange) {
		NullAbleChange = false;
		for (unordered_map<string, vector<string>>::iterator itProduct = grammar.P.begin();itProduct != grammar.P.end();itProduct++) {
			for (string itCandidate : (*itProduct).second) {
				//跳过加入NullAble 的产生式
				if (NullAble.find((*itProduct).first) != NullAble.end()) { continue; }

				if (itCandidate == "@") {
					NullAble.insert((*itProduct).first);
					NullAbleChange = true;
					break;
				}
				else {
					//候选式全含空 也属NullAble集
					int allEmpty = 0;
					for (int i = 0;i < itCandidate.length();) {
						string fullVn = getFullChar(itCandidate, i);
						if (NullAble.find(fullVn) != NullAble.end()) {
							allEmpty++;
						}
						else { break; }
						i += fullVn.length();
					}
					if (allEmpty == itCandidate.length()) {
						NullAble.insert((*itProduct).first);
						NullAbleChange = true;
					}
				}
			}
		}
	}
	/*cout << "NullAble" << endl;
	for (string a : NullAble) {
		cout << a << endl;
	}*/
}

void LL1Class::calculate_First() {
	unordered_map<string, unordered_set<string>>FirstCheck;
	do {
		//检测First是否更新
		First = FirstCheck;

		for (unordered_map<string, vector<string>>::iterator itP = grammar.P.begin();itP != grammar.P.end();itP++) {
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//遍历候选式的每一个符号
				for (int i = 0; i < (*itCandidate).length();) {
					string fullChar = getFullChar(*itCandidate, i);
					//非终结符
					if (grammar.Vn.find(fullChar) != grammar.Vn.end()) {
						//将这个非终结符的First集合 insert并入产生式左部符号的First集
						FirstCheck[(*itP).first].insert(FirstCheck[fullChar].begin(), FirstCheck[fullChar].end());

						//不能推空  直接结束
						if (NullAble.find(fullChar) == NullAble.end()) {
							break;
						}
					}
					//非终结符 或 空
					else {
						FirstCheck[(*itP).first].insert(fullChar);
						break;
					}
					i += fullChar.length();
				}
			}
		}
	} while (FirstCheck != First);

	/*cout << endl << "First" << endl;
	for (auto a : First) {
		cout << a.first << "->";
		for (string b : a.second) {
			cout << b << " ";
		}
		cout << endl;
	}*/
}

void LL1Class::calculate_Follow() {
	unordered_map<string, unordered_set<string>>FollowCheck;
	FollowCheck[grammar.S].insert("#");
	do {
		Follow = FollowCheck;
		for (unordered_map<string, vector<string>>::iterator itP = grammar.P.begin();itP != grammar.P.end();itP++) {
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//temp 先拿 Follow(P->first)
				unordered_set<string>temp = FollowCheck[(*itP).first];

				//反向遍历候选式的每一个符号
				for (int i = (*itCandidate).length() - 1; i >= 0;i--) {
					//反着遍历 可能先碰到'
					if ((*itCandidate)[i] == '\'') {
						continue;
					}
					string fullChar = getFullChar(*itCandidate, i);
					//非终结符
					if (grammar.Vn.find(fullChar) != grammar.Vn.end()) {
						//Follow(M) U= temp    跳过@
						for (string symbol : temp) {
							if (symbol != "@") {
								FollowCheck[fullChar].insert(symbol);
							}
						}

						if (NullAble.find(fullChar) == NullAble.end()) {
							//非空 temp = First（M)
							temp = First[fullChar];
						}
						else {
							//空 temp += First(M)
							temp.insert(First[fullChar].begin(), First[fullChar].end());
						}
					}
					//终结符 并跳过空字
					if (grammar.Vt.find(fullChar) != grammar.Vt.end()) {
						temp = { fullChar };
					}
				}
			}
		}
	} while (FollowCheck != Follow);

	/*cout << endl << "Follow" << endl;
	for (auto a : Follow) {
		cout << a.first << "->";
		for (string b : a.second) {
			cout << b << " ";
		}
		cout << endl;
	}*/
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

	/*cout << endl << "LL1_Table" << endl;
	for (unordered_map<string, vector<pair<string, string>>>::iterator A = LL1_table.begin(); A != LL1_table.end(); A++) {
		cout << A->first << " -- ";
		for (vector<pair<string, string>>::iterator B = A->second.begin(); B != A->second.end(); B++) {
			cout << B->first << "," << B->second << "  ";
		}
		cout << endl;
	}*/
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

//void readGrammar() {
//	FILE* fp = fopen(READGRAMMARFileName, "r");
//	//无判断 文件打开异常会中止程序
//	if (fp == NULL) {
//		cout << "NO file";
//	}
//	else {
//		//std::memset(lineToken, '\0', sizeof(lineToken));
//		while (fgets(lineToken, 50, fp) != NULL) {
//			scan(lineToken);
//		}
//		//文件句柄开了得关
//		fclose(fp);
//	}
//}

//bool readSLRGrammar() {
//	FILE* fp = fopen(readSLRGrammarFile, "r");
//	//无判断 文件打开异常会中止程序
//	if (fp == NULL) {
//		cout << "SLRGrammarFile Not Exist";
//		return false;
//	}
//	else {
//		while (fgets(lineToken, 50, fp) != NULL) {
//			scan(lineToken);
//		}
//		//文件句柄开了得关
//		fclose(fp);
//	}
//	return true;
//}

/*  1.分析栈状态   2.其他文法的适配性检测
*
 L->E
E->E+T|T;
T->T*F|F;
F->(E)|i;

test data
10;
1+2;
(1+2)*3+(5+6*7);
((1+2)*3+4;
1+2+3+(*4+5);
(a+b)*(c+d);
((ab3+de4)**5)+1;

1 + 1 + ab * 2
12 + 18
12 * 21
12 * abc + 1
(12+1)
(1)
((1*2)
(1)*(3)
(1++float)
(1*1+int)

test grammar
i(+i)*n (*i|@)

L->S
S->AB;
A->i|A+i
B->*i|@

9
12+1
13+ a+b
21 + 33 * abc
12 * a
*12

*/