#include"../include/Common_data.h"
using namespace std;

std::regex N("[A-Z]");
std::regex T("[a-z0-9*+()&#]");

char lineToken[128] = {};
char expLineToken[50] = {};
char strToken[50] = {};

int getBC(int pos, char line[]) {
	while (line[pos] == ' ' || line[pos] == '|' || line[pos] == '\t' || line[pos] == '\n') {
		pos++;
	}
	return pos;
}

string getFullChar(string candidate, int pos) {
	int length = 1;
	string fullVn;
	//下一位是' 再读一位
	for (int i = pos + 1;i < candidate.length();i++) {
		if (candidate[i] == '\'') {
			length++;
		}
		else { break; }
	}
	return candidate.substr(pos, length);
}

bool Grammar_set::readGrammar(const char* GrammarFile) {
	FILE* fp = fopen(GrammarFile, "r");
	//无判断 文件打开异常会中止程序
	if (fp == NULL) {
		cout << string(GrammarFile)+" Not Exist";
		return false;
	}
	else {
		memset(lineToken, '\0', sizeof(lineToken));
		while (fgets(lineToken, 128, fp) != NULL) {
			scanCandidateIntoGrammar(lineToken);
			memset(lineToken, '\0', sizeof(lineToken));
		}
		//文件句柄开了得关
		fclose(fp);
	}
	return true;
}

void Grammar_set::scanCandidateIntoGrammar(char line[]) {
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
			grammar.P[product_left].push_back(product_right);
			//更新下一次读取的起始位置 及 长度
			startPos = getBC(startPos + length + 1, line);
			moveCheck = line[startPos];
			length = 0;

			//实验五中加入产生式数组 便于编号和归约
			Productions.push_back(string(product_left + "->" + product_right));
		}
	}
	//读入Vn Vt -- 无法读入开始时文法 以A' 等两位做 非终结符的
	for (int i = 0;i < product.length(); i++) {
		if (product[i] == '\n' || product[i] == '\0') { break; }

		string checkString = string(1, product[i]);
		if (regex_match(checkString, N)) {
			grammar.Vn[checkString] = 1;
			// Vn集合中的一个元素 是开始符号S
			if (grammar.S == "") { grammar.S = checkString; }
		}
		else if (regex_match(checkString, T)) {
			grammar.Vt.insert(checkString);
		}
	}
}


void Grammar_set::formatPrint() {
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
	//CH
	/*cout << endl << "开始符号  S:  " << grammar.S;
	cout << endl << "  终结符 Vn:  ";
	for (unordered_map<string, int>::iterator it = grammar.Vn.begin(); it != grammar.Vn.end();it++) {
		cout << it->first << " ";
	}
	cout << endl << "非终结符 Vt:  ";
	for (set<string>::iterator it = grammar.Vt.begin(); it != grammar.Vt.end();it++) {
		cout << *it << " ";
	}*/

	cout << endl << "start symbol  S:  " << grammar.S;
	cout << endl << "  Terminator Vn:  ";
	for (unordered_map<string, int>::iterator it = grammar.Vn.begin(); it != grammar.Vn.end();it++) {
		cout << it->first << " ";
	}
	cout << endl << "non-terminal Vt:  ";
	for (set<string>::iterator it = grammar.Vt.begin(); it != grammar.Vt.end();it++) {
		cout << *it << " ";
	}
}

void Grammar_set::formatPrintLL1IntoFile(const char* writeFile) {
	if (freopen(writeFile, "w", stdout) != NULL) {
		//格式化输出文法
		formatPrint();
		//把标准输出改回控制台
		auto _ = freopen("CON", "w", stdout);
	}
	else {
		cout << string(writeFile) + " Not Exist" << endl;;
	}
}

void Grammar_set::calculate_NullAble() {
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

void Grammar_set::calculate_First() {
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

void Grammar_set::calculate_Follow() {
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