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
	//��һλ��' �ٶ�һλ
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
	//���ж� �ļ����쳣����ֹ����
	if (fp == NULL) {
		cout << "SLRGrammarFile Not Exist";
		return false;
	}
	else {
		//memset(lineToken, '/0', sizeof(lineToken));
		while (fgets(lineToken, 50, fp) != NULL) {
			scanCandidateIntoGrammar(lineToken,grammar,Productions);
		}
		//�ļ�������˵ù�
		fclose(fp);
	}
	return true;
}

void scanCandidateIntoGrammar(char line[],grammarStruct& grammar, std::vector<std::string>& Productions) {
	string product = line;
	//��¼����ʽ�� �� �� �Ҳ���ѡʽ
	string product_left, product_right;
	int startPos = 0, length = 0;
	if (product.find('-', startPos) != string::npos) {
		//ȡ����ʽ��
		startPos = getBC(startPos, line);
		while (regex_match(string(1, line[startPos + length]), N)) {
			length++;
		}
		product_left = product.substr(startPos, length);
		//ȡ����ʽ�Ҳ�
		startPos = product.find('-', startPos) + 2;
		startPos = getBC(startPos, line);
		char moveCheck = line[startPos];
		length = 0;
		while (moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0' && startPos <= product.length() - 1) {
			while (moveCheck != ' ' && moveCheck != '|' && moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0') {
				length++;
				moveCheck = line[startPos + length];
			}
			//��ʼλ��+����	ȡ��ѡʽ 
			product_right = product.substr(startPos, length);
			grammar.P[product_left].push_back(product_right);
			//������һ�ζ�ȡ����ʼλ�� �� ����
			startPos = getBC(startPos + length + 1, line);
			moveCheck = line[startPos];
			length = 0;

			//ʵ�����м������ʽ���� ���ڱ�ź͹�Լ
			Productions.push_back(string(product_left + "->" + product_right));
		}
	}
	//����Vn Vt -- �޷����뿪ʼʱ�ķ� ��A' ����λ�� ���ս����
	for (int i = 0;i < product.length(); i++) {
		if (product[i] == '\n' || product[i] == '\0') { break; }

		string checkString = string(1, product[i]);
		if (regex_match(checkString, N)) {
			grammar.Vn[checkString] = 1;
			// Vn�����е�һ��Ԫ�� �ǿ�ʼ����S
			if (grammar.S == "") { grammar.S = checkString; }
		}
		else if (regex_match(checkString, T)) {
			grammar.Vt.insert(checkString);
		}
	}
}