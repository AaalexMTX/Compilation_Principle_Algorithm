#include"LL1_info.h"
	
//全局变量
const char* READGRAMMARFileName = "grammarText.txt";
const char* WRITEGRAMMARFileName = "LL1resultFile.txt";
const int MAXLEN = 50;
char lineToken[MAXLEN] = {};
regex N("[A-Z]");
regex T("[a-z0-9*+()&#]");
grammerStruct grammer;
unordered_set<string>nullAble;
unordered_map<string, unordered_set<string>>first, follow;
unordered_map<string, vector<pair<string,string>>>LL1_table;
stack<string>analyseStack;

void readGrammar() {
	FILE* fp = fopen(READGRAMMARFileName, "r");
	//无判断 文件打开异常会中止程序
	if (fp == NULL) {
		cout << "NO file";
	}
	else {
		while (fgets(lineToken, MAXLEN, fp) != NULL) {
			scan(lineToken);
		}
	}
}

void formatPrintIntoFile() {
	if (freopen(WRITEGRAMMARFileName, "w", stdout) != NULL) {
		formatPrint();
	}
	fclose(stdout);
}

int getBC(int pos, char line[]) {
	while (line[pos] == ' ' || line[pos] == '|' || line[pos] == '\t') {
		pos++;
	}
	return pos;
}

void scan(char line[]) {
	string product = line;
	//记录产生式的 左部 和 右部候选式
	string product_left, product_right;
	int startPos = 0, length = 0;
	if (product.find('-', startPos) != string::npos) {
		//取产生式左部
		startPos = getBC(startPos, line);
		while (regex_match(string(1, line[startPos + length]), N)) {
			length++;
		}
		product_left = product.substr(startPos, length);
		//取产生式右部
		startPos = product.find('-', startPos) + 2;
		startPos = getBC(startPos, line);
		char moveCheck = line[startPos];
		length = 0;
		while (moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0' && startPos <= product.length() - 1) {
			while (moveCheck != ' ' && moveCheck != '|' && moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0') {
				length++;
				moveCheck = line[startPos + length];
			}
			//起始位置+长度	取候选式 
			product_right = product.substr(startPos, length);
			grammer.P[product_left].push_back(product_right);
			//更新下一次读取的起始位置 及 长度
			startPos = getBC(startPos + length + 1, line);
			moveCheck = line[startPos];
			length = 0;
		}
	}
	//读入Vn Vt -- 无法读入开始时文法 以A' 等两位做 非终结符的
	for (int i = 0;i < product.length(); i++) {
		if (product[i] == '\n' || product[i] == '\0') { break; }

		string checkString = string(1, product[i]);
		if (regex_match(checkString, N)) {
			grammer.Vn[checkString] = 1;
			// Vn集合中的一个元素 是开始符号S
			if (grammer.S == "") { grammer.S = checkString; }
		}
		else if (regex_match(checkString, T)) {
			grammer.Vt.insert(checkString);
		}
	}
}

void formatPrint() {
	for (unordered_map<string, vector<string>>::iterator it = grammer.P.begin(); it != grammer.P.end(); it++) {
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
	cout << endl << "开始符号  S:  " << grammer.S;
	cout << endl << "  终结符 Vn:  ";
	for (unordered_map<string, int>::iterator it = grammer.Vn.begin(); it != grammer.Vn.end();it++) {
		cout << it->first << " ";
	}
	cout << endl << "非终结符 Vt:  ";
	for (set<string>::iterator it = grammer.Vt.begin(); it != grammer.Vt.end();it++) {
		cout << *it << " ";
	}
}

void directLeftRecursion(string proLeft, vector<string>proRight) {
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
		grammer.P[proLeft].clear();

		//更新分号的数量统一存在 A中  如 A' A''  -> A中记录下次添加'的数量
		unordered_map<string, int>::iterator itProLeft = grammer.Vn.find(string(1,proLeft[0]));
		string FHCount = string(itProLeft->second, '\'');
		itProLeft->second++;
		//加了新非终结符A'  更新Vn 
		grammer.Vn[proLeft + FHCount] = 1;

		bool endInsertEmpty = false;
		for (vector<string>::iterator it = originPro.begin(); it < originPro.end(); it++) {
			//不能插入形如 A-> @A'
			if ((*it) == "@") { endInsertEmpty = true; }
			else {
				// A->e  -- A->eA'
				grammer.P[proLeft].push_back((*it) + proLeft + FHCount);
			}
		}
		//特意将 @更新至末尾
		if (endInsertEmpty) { grammer.P[proLeft].push_back("@"); }
		for (vector<string>::iterator it = newPro.begin();it < newPro.end(); it++) {
			// A->Abc  --  A'->bcA'
			grammer.P[proLeft + FHCount].push_back((*it) + proLeft + FHCount);
		}
		//末尾添加空字
		grammer.P[proLeft + FHCount].push_back("@");
	}
}

void remove_left_recursion() {
	for (unordered_map<string, vector<string>>::iterator itI = grammer.P.begin(); itI != grammer.P.end(); itI++) {
		//对产生式itI尝试代入 其他所有itJ
		for (unordered_map<string, vector<string>>::iterator itJ = grammer.P.begin(); itJ != itI;itJ++) {
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

void remove_left_gene() {
	unordered_map<string, vector<string>>candidate;
	//遍历文法的所有产生式
	for (unordered_map<string, vector<string>>::iterator itI = grammer.P.begin();itI != grammer.P.end();itI++) {
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
				unordered_map<string, int>::iterator itProLeft = grammer.Vn.find(string(1, itI->first[0]));
				string FHCount = string(itProLeft->second, '\'');
				itProLeft->second++;
				//加了新非终结符A'  更新Vn 
				string newVN = itI->first[0] + FHCount;
				grammer.Vn[newVN] = 1;

				//给A 加上 aA' 项
				itI->second.push_back(itJ->first + itProLeft->first + FHCount);
				bool endInsertEmpty = false;
				//对所有有公因式的候选式
				for (vector<string>::iterator itJK = itJ->second.begin();itJK != itJ->second.end();itJK++) {
					//A->ab1|ab2|a  --	A'->b1|b2|@
					if (*itJK != "") {
						grammer.P[newVN].push_back(*itJK);
					}
					else { endInsertEmpty = true; }
				}
				if(endInsertEmpty){ grammer.P[newVN].push_back("@"); }
			}
		}
	}
}

string getFullChar(string candidate, int pos) {
	int length = 1;
	string fullVn;
	//下一位是' 再读一位
	for (int i = pos+1;i < candidate.length();i++) {
		if (candidate[i] == '\'') {
			length++;
		}
		else { break; }
	}
	return candidate.substr(pos, length);
}

void calculate_nullAble() {
	bool nullAbleChange = true;
	while (nullAbleChange) {
		nullAbleChange = false;
		for (unordered_map<string, vector<string>>::iterator itProduct = grammer.P.begin();itProduct != grammer.P.end();itProduct++) {
			for (string itCandidate : (*itProduct).second) {
				//跳过加入nullable 的产生式
				if (nullAble.find((*itProduct).first) != nullAble.end()) { continue; }

				if (itCandidate == "@") {
					nullAble.insert((*itProduct).first);
					nullAbleChange = true;
					break;
				}
				else {
					//候选式全含空 也属nullable集
					int allEmpty = 0;
					for (int i = 0;i < itCandidate.length();) {
						string fullVn = getFullChar(itCandidate, i);
						if (nullAble.find(fullVn) != nullAble.end()) {
							allEmpty++;
						}
						else { break; }
						i += fullVn.length();
					}
					if (allEmpty == itCandidate.length()) {
						nullAble.insert((*itProduct).first);
						nullAbleChange = true;
					}
				}
			}
		}
	}
	/*cout << "NullAble" << endl;
	for (string a : nullAble) {
		cout << a << endl;
	}*/
}

void calculate_first() {
	unordered_map<string, unordered_set<string>>firstCheck;
	do{
		//检测first是否更新
		first = firstCheck;

		for (unordered_map<string, vector<string>>::iterator itP = grammer.P.begin();itP != grammer.P.end();itP++) {
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//遍历候选式的每一个符号
				for (int i = 0; i < (*itCandidate).length();) {
					string fullChar = getFullChar(*itCandidate, i);
					//非终结符
					if(grammer.Vn.find(fullChar) != grammer.Vn.end()){
						//将这个非终结符的first集合 insert并入产生式左部符号的first集
						firstCheck[(*itP).first].insert(firstCheck[fullChar].begin(),firstCheck[fullChar].end());

						//不能推空  直接结束
						if (nullAble.find(fullChar) == nullAble.end()) {
							break;
						}
					}
					//非终结符 或 空
					else {
						firstCheck[(*itP).first].insert(fullChar);
						break;
					}
					i += fullChar.length();
				}
			}
		}
	} while (firstCheck != first);

	/*cout << endl << "First" << endl;
	for (auto a : first) {
		cout << a.first << "->";
		for (string b : a.second) {
			cout << b << " ";
		}
		cout << endl;
	}*/
}

void calculate_follow() {
	unordered_map<string, unordered_set<string>>followCheck;
	followCheck[grammer.S].insert("#");
	do {
		follow = followCheck;
		for (unordered_map<string, vector<string>>::iterator itP = grammer.P.begin();itP != grammer.P.end();itP++) {
			//temp 先拿 follow(P->first)
			unordered_set<string>temp = followCheck[(*itP).first];
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//反向遍历候选式的每一个符号
				for (int i = (*itCandidate).length()-1; i >= 0;i--) {
					//反着遍历 可能先碰到'
					if ((*itCandidate)[i] == '\'') {
						continue;
					}
					string fullChar = getFullChar(*itCandidate, i);
					//非终结符
					if (grammer.Vn.find(fullChar) != grammer.Vn.end()) {
						//follow(M) U= temp    跳过@
						for (string symbol : temp) {
							if (symbol != "@") {
								followCheck[fullChar].insert(symbol);
							}
						}
						
						if (nullAble.find(fullChar) == nullAble.end()) {
							//非空 temp = first（M)
							temp = first[fullChar];
						}
						else {
							//空 temp += first(M)
							temp.insert(first[fullChar].begin(), first[fullChar].end());
						}
					}
					//终结符 并跳过空字
					if(grammer.Vt.find(fullChar) != grammer.Vt.end()) {
						temp = {fullChar};
					}
				}
			}
		}
	} while (followCheck != follow);

	/*cout << endl << "FOLLOW" << endl;
	for (auto a : follow) {
		cout << a.first << "->";
		for (string b : a.second) {
			cout << b << " ";
		}
		cout << endl;
	}*/
}

void construct_LL1Table() {
	for (unordered_map<string, vector<string>>::iterator itP = grammer.P.begin(); itP != grammer.P.end();itP++) {
		for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
			//遍历产生式每个符号
			for (int i = 0;i < (*itCandidate).length();) {
				string fullChar = getFullChar(*itCandidate, i);

				//终结符
				if (grammer.Vn.find(fullChar) != grammer.Vn.end()) {
					//将M 的first集 加入 分析表
					for (unordered_set<string>::iterator itfullCharFirst = first[fullChar].begin(); itfullCharFirst != first[fullChar].end(); itfullCharFirst++) {
						LL1_table[itP->first].push_back({ *itfullCharFirst, *itCandidate });
					}
					//非空 不继续遍历候选式了
					if (nullAble.find(fullChar) == nullAble.end()) { break; }
				}
				//非终结符
				else if (grammer.Vt.find(fullChar) != grammer.Vt.end()) {
					LL1_table[itP->first].push_back({ fullChar, *itCandidate });
					break;
				}
				//@
				else {
					//将M 的follow集 加入 分析表
					for (unordered_set<string>::iterator itfullCharFollow = follow[itP->first].begin(); itfullCharFollow != follow[itP->first].end();itfullCharFollow++) {
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

bool LL1_predict(string inputExpression) {
	analyseStack.push("#");
	analyseStack.push(grammer.S);

	inputExpression += "#";

	while (!analyseStack.empty()) {
		string top = analyseStack.top();
		analyseStack.pop();
		string inputExpTop = inputExpression.substr(0, 1);
		//终结符
		if (grammer.Vn.find(top) != grammer.Vn.end()) {
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
			else{ return false; }

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