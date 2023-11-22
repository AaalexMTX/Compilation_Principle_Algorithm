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
	//记录产生式的 左部 和 右部候选式
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
	for (unordered_map<string, int>::iterator it = grammer.Vn.begin(); it != grammer.Vn.end();it++) {
		cout << it->first << " ";
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