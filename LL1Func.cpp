#include"LL1_info.h"


//全局变量
const char* readFileName = "anaTestWords01.txt";
const char* writeFileName = "LL1resultFile.txt";
int VNnumber = 0;
regex N("[A-Z]");
regex T("[a-z0-9*+()&#]");
grammerStruct grammer;

int getBC(int pos, char line[]) {
	while (line[pos] == ' ' || line[pos] == '|' || line[pos] == '\t') {
		pos++;
	}
	return pos;
}

//文件扫描录入文法
void scan(char lineToken[]) {
	string product = lineToken;
	string product_left, product_right;
	int startPos = 0, length = 0;
	if (product.find('-', startPos) != string::npos) {
		//取产生式左部
		startPos = getBC(startPos, lineToken);
		while (regex_match(string(1, lineToken[startPos + length]), N)) {
			length++;
		}
		product_left = product.substr(startPos, length);
		//取产生式右部
		startPos = product.find('-', startPos) + 2;
		startPos = getBC(startPos, lineToken);
		char moveCheck = lineToken[startPos];
		length = 0;
		while (moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0' && startPos <= product.length() - 1) {
			while (moveCheck != ' ' && moveCheck != '|' && moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0') {
				length++;
				moveCheck = lineToken[startPos + length];
			}
			//起始位置+长度	取候选式 
			product_right = product.substr(startPos, length);
			grammer.P[product_left].push_back(product_right);
			//更新下一次读取的起始位置 及 长度
			startPos = getBC(startPos + length + 1, lineToken);
			moveCheck = lineToken[startPos];
			length = 0;
		}
	}
	//读入Vn Vt
	for (int i = 0;i < sizeof(lineToken) / sizeof(lineToken[0]); i++) {
		if (lineToken[i] == '\n' || lineToken[i] == '\0') { break; }
		if (regex_match(string(1, lineToken[i]), N)) {
			pair<string, int>VNCount(string(1, lineToken[i]), 1);
			grammer.Vn.insert(VNCount);
			// Vn集合中的一个元素 是开始符号S
			if (grammer.S == "") { grammer.S = string(1, lineToken[i]); }
		}
		else if (regex_match(string(1, lineToken[i]), T)) {
			grammer.Vt.insert(string(1, lineToken[i]));
		}
	}
}

//格式化输出文法
void print() {
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
	for (set<pair<string, int>>::iterator it = grammer.Vn.begin(); it != grammer.Vn.end();it++) {
		cout << (*it).first << " ";
	}
	cout << endl << "非终结符 Vt:  ";
	for (set<string>::iterator it = grammer.Vt.begin(); it != grammer.Vt.end();it++) {
		cout << *it << " ";
	}
}

//消除 产生式的直接左递归
void directLeftRecursion(string proLeft, vector<string>proRight) {
	vector<string>originPro, newPro;
	for (vector<string>::iterator it = proRight.begin(); it < proRight.end(); it++) {
		//检测是否发生左递归
		if ((*it).find(proLeft) == 0) {
			//取 候选式非左侧子串的部分
			string backSub = (*it).substr(proLeft.length());
			newPro.push_back(backSub);
		}
		else {
			originPro.push_back(*it);
		}
	}
	//消除左递归后更新文法
	if (!newPro.empty()) {
		//清空旧产生式
		grammer.P[proLeft].clear();
		//Vn中加入新的(是否需要遍历查重 再插)
		pair<string, int>newVn(proLeft + "'", 1);
		grammer.Vn.insert(newVn);
		//进了if 就表示多了一个分号 遍历找对应的Vn<pair>对
		set<pair<string, int>>::iterator it;
		for (it = grammer.Vn.begin();it != grammer.Vn.end();it++) {
			if ((*it).first == proLeft) {
				break;
			}
		}
		string FHCount = string((*it).second, '\'');	//分号数量
		for (vector<string>::iterator it = originPro.begin(); it < originPro.end(); it++) {
			//不能插入形如 A-> @A'
			if ((*it) == "@") { grammer.P[proLeft].push_back("@"); }
			else {
				// A->e  -- A->eA'
				grammer.P[proLeft].push_back((*it) + proLeft + FHCount);
			}
		}
		for (vector<string>::iterator it = newPro.begin();it < newPro.end(); it++) {
			// A->Abc  --  A'->bcA'
			grammer.P[proLeft + FHCount].push_back((*it) + proLeft + FHCount);
		}
		//末尾添加空字
		grammer.P[proLeft + FHCount].push_back("@");
		//插新的 删旧的 
		int count = (*it).second;
		grammer.Vn.insert(pair<string, int>((*it).first, count + 1));
		grammer.Vn.erase(*it);
	}
}

//消除 间接左递归
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

//提取左因子
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
				//进了else 就表示多了一个分号 
				set<pair<string, int>>::iterator it;
				for (it = grammer.Vn.begin();it != grammer.Vn.end();it++) {
					if ((*it).first == (*itI).first) {
						break;
					}
				}
				string FHCount = string((*it).second, '\'');	//分号数量
				//给A 加上 aA' 项
				itI->second.push_back(itJ->first + itI->first + FHCount);
				//对所有有公因式的候选式
				for (vector<string>::iterator itJK = itJ->second.begin();itJK != itJ->second.end();itJK++) {
					//A->ab1|ab2|a  --	A'->b1|b2|@
					if (*itJK != "") {
						grammer.P[itI->first + FHCount].push_back(*itJK);
					}
					else {
						grammer.P[itI->first + FHCount].push_back("@");
					}
				}
				//Vn中插加分号的 新生成的产生式左部 -- S'
				grammer.Vn.insert(pair<string, int>(itI->first + FHCount, 1));
				//加新的 -- <S ,2>
				int count = (*it).second;
				pair<string, int>newVn((*it).first, count + 1);
				grammer.Vn.insert(newVn);
				//删旧的 -- <S,1>
				grammer.Vn.erase(*it);
			}
		}
	}
}