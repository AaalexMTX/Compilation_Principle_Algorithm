#include"../include/Common_data.h"
using namespace std;

std::regex N("[A-Z]");
std::regex T("[a-z0-9*+()&#]");

char lineToken[50] = {};
char expLineToken[50] = {};

int getBC(int pos, char line[]) {
	while (line[pos] == ' ' || line[pos] == '|' || line[pos] == '\t') {
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

bool readGrammar(const char* GrammarFile,grammarStruct& grammar, std::vector<std::string>& Productions) {
	FILE* fp = fopen(GrammarFile, "r");
	//无判断 文件打开异常会中止程序
	if (fp == NULL) {
		cout << "SLRGrammarFile Not Exist";
		return false;
	}
	else {
		//memset(lineToken, '/0', sizeof(lineToken));
		while (fgets(lineToken, 50, fp) != NULL) {
			scanCandidateIntoGrammar(lineToken,grammar,Productions);
		}
		//文件句柄开了得关
		fclose(fp);
	}
	return true;
}

void scanCandidateIntoGrammar(char line[],grammarStruct& grammar, std::vector<std::string>& Productions) {
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