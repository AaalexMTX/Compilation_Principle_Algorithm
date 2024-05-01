#include"../include/LL1_info.h"
#include"../include/SLR_info.h"
#include"../include/wordAnalyse_info.h"
using namespace std;

regex delimiterPattern("[()[\\]{},.;:'\"?@#$\\\\]");		//�������
regex operatePattern("[=+-/*%/&|!^<>]");					//���������
const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";
char keyWords[][10] = { "int", "float","bool", "true", "false", "main", "for",
	"while", "if", "else","return", "void", "scanf", "cin","include","string",
	"printf", "cout", "switch", "case", "default" ,"then" };


bool isNumber(char& c) {
	return (c >= '0' && c <= '9') ? true : false;
}

bool isLetter(char& c) {
	return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') ? true : false;
}

int getBCExp(char line[], int pos) {
	int nextStart = pos;
	char c = line[nextStart];
	while (c == '\n' || c == ' ' || c == '\t') {
		nextStart++;
		c = line[nextStart];
	}
	return nextStart;
}

int getBCExp(string line, int pos) {
	while (line[pos] == '\n' || line[pos] == ' ') {
		pos++;
	}
	return pos;
}

void wordChange1() {
	int pos = 0,j = 0;
	string elementType, elementValue;
	while (pos < strlen(lineToken)) {
		int flag = 0, j = 0;
		pos = getBC(pos,lineToken);		//�ų���ͷ�Ŀհ׷�
		memset(strToken,  '\0', sizeof(strToken));
		//���Ŵ�
		if (isLetter(lineToken[pos]) || lineToken[pos] == '_') {
			while (isLetter(lineToken[pos]) || isNumber(lineToken[pos]) || lineToken[pos] == '_') {
				strToken[j++] = lineToken[pos++];
			}
			//�ж��Ƿ�ؼ���
			for (int i = 0; i < sizeof(keyWords) / sizeof(keyWords[0]); i++) {
				if (!strcmp(strToken, keyWords[i])) {
					elementType = keyWords[i];
					elementValue = to_string(i);
					flag = 1;
					break;
				}
			}
			if (!flag) {
				elementType = "ID";
				elementValue = strToken;
			}

		}
		//����
		else if (isNumber(lineToken[pos])) {		//��������DFA��һ�·ֿ���
			if (lineToken[pos] == '0') {
				pos++;							//��Ӱ��ʮ���ƶ���
				regex octCheck("[1234567]");
				regex hexCheck("[0123456789abcdef]");
				if (lineToken[pos] == 'x') {		//ʮ������ 
					pos++;						//��Ӱ��˽��ƶ���
					while (lineToken[pos] == '0')pos++;		//���ȵ�0����
					while (regex_match(string(1, lineToken[pos]), hexCheck)) {		//��һλ����hex
						strToken[j++] = lineToken[pos++];
					}
					elementType = "Hex";
					elementValue = strToken;
				}
				else if (regex_match(string(1, lineToken[pos]), octCheck)) {		//�˽���
					while (lineToken[pos] == '0')pos++;		//���ȵ�0
					while (regex_match(string(1, lineToken[pos]), octCheck)) {		//��һλ����oct
						strToken[j++] = lineToken[pos++];
						elementType = "Oct";
						elementValue = strToken;
					}
				}
				else {
					elementType = "Dec";
					elementValue = "0";
				}
			}
			else {
				while (isNumber(lineToken[pos])) {
					strToken[j++] = lineToken[pos++];
				}
				elementType = "Dec";
				elementValue = strToken;
			}
		}
		//����� ����++ +=
		//�����һλ�� ���������ƥ��
		else if (regex_match(string(1, lineToken[pos]), operatePattern)) {	//regex_match����ƥ�� ����1 -> char ת string 
			 //= �� ���� ������һλ
			strToken[j++] = lineToken[pos++];
			if (lineToken[pos] == '=' || lineToken[pos] == lineToken[pos - 1]) {
				strToken[j++] = lineToken[pos++];
			}
			elementType = strToken;
			elementValue = "-";
		}
		//���
		else if (regex_match(string(1, lineToken[pos]), delimiterPattern)) {
			elementType = string(1, lineToken[pos++]);
			elementValue = "-";
		}
		//�����ַ�

		printf("<%s , %s>\n", &elementType[0], elementValue.c_str());
		pos = getBC(pos,lineToken);	//�ų�ĩ�˿հ׷�
	}
}

bool readWordFile(const char* wordFile) {
	FILE* fp = fopen(wordFile, "r");
	//���ж� �ļ����쳣����ֹ����
	if (fp == NULL) {
		cout << string(wordFile) + " Not Exist";
		return false;
	}
	else {
		memset(lineToken, '\0', sizeof(lineToken));
		while (fgets(lineToken, 128, fp) != NULL) {
			wordChange1();
			memset(lineToken, '\0', sizeof(lineToken));
		}
		//�ļ�������˵ù�
		fclose(fp);
	}
	return true;
}

string ExpChange(char line[]) {
	string elementType, elementValue;
	string ansExp = "";
	int linePf = 0;
	int wordPf = 0;
	while (linePf < strlen(line) && line[linePf] != '\n') {
		linePf = getBCExp(line, linePf);		//�ų���ͷ�Ŀհ׷�

		int flag = 0, wordPf = 0;
		memset(strToken, 0, 50);
		//���Ŵ�
		if (isLetter(line[linePf]) || line[linePf] == '_') {
			while (isLetter(line[linePf]) || isNumber(line[linePf]) || line[linePf] == '_') {
				strToken[wordPf++] = line[linePf++];
			}
			//�ж��Ƿ�ؼ���
			for (int i = 0; i < sizeof(keyWords) / sizeof(keyWords[0]); i++) {
				if (!strcmp(strToken, keyWords[i])) {
					elementType = keyWords[i];
					elementValue = "i";
					flag = 1;
					break;
				}
			}
			if (!flag) {
				elementType = "ID";
				elementValue = "i";
			}

		}
		//����
		else if (isNumber(line[linePf])) {		//��������DFA��һ�·ֿ���
			if (line[linePf] == '0') {
				linePf++;							//��Ӱ��ʮ���ƶ���
				regex octCheck("[1234567]");
				regex hexCheck("[0123456789abcdef]");
				if (line[linePf] == 'x') {		//ʮ������ 
					linePf++;						//��Ӱ��˽��ƶ���
					while (line[linePf] == '0')linePf++;		//���ȵ�0����
					while (regex_match(string(1, line[linePf]), hexCheck)) {		//��һλ����hex
						strToken[wordPf++] = line[linePf++];
					}
					elementType = "Hex";
					elementValue = "i";
				}
				else if (regex_match(string(1, line[linePf]), octCheck)) {		//�˽���
					while (line[linePf] == '0')linePf++;		//���ȵ�0
					while (regex_match(string(1, line[linePf]), octCheck)) {		//��һλ����oct
						strToken[wordPf++] = line[linePf++];
						elementType = "Oct";
						elementValue = "i";
					}
				}
				else {
					elementType = "Dec";
					elementValue = "i";
				}
			}
			else {
				while (isNumber(line[linePf])) {
					strToken[wordPf++] = line[linePf++];
				}
				elementType = "Dec";
				elementValue = "i";
			}
		}
		//����� ����++ +=
		//�����һλ�� ���������ƥ��
		else if (regex_match(string(1, line[linePf]), operatePattern)) {	//regex_match����ƥ�� ����1 -> char ת string 
			 //= �� ���� ������һλ
			strToken[wordPf++] = line[linePf++];
			if (line[linePf] == '=' || line[linePf] == line[linePf - 1]) {
				strToken[wordPf++] = line[linePf++];
			}
			elementType = strToken;
			elementValue = strToken;
		}
		//���
		else if (regex_match(string(1, line[linePf]), delimiterPattern)) {
			elementType = string(1, line[linePf]);
			elementValue = string(1, line[linePf]);
			linePf++;
		}
		linePf = getBCExp(line, linePf);	//�ų�ĩ�˿հ׷�
		if (elementValue == ";") { continue; }
		ansExp += elementValue.c_str();
	}
	return ansExp;
}

pair<string, string> wordChange(string& line, int& linePf) {
	string elementType, elementValue;	//���� �� ����ֵ
	int wordPf = 0;						//��������������
	//һ�η���
	linePf = getBCExp(line, linePf);							//�ų���ͷ�Ŀհ׷�
	memset(strToken, 0, sizeof(strToken));			//��յ��ʶ���
	//���Ŵ�
	if (isLetter(line[linePf]) || line[linePf] == '_') {
		while (isLetter(line[linePf]) || isNumber(line[linePf]) || line[linePf] == '_') {
			strToken[wordPf++] = line[linePf++];
		}
		//���� �ж��Ƿ�ؼ���
		elementType = "str";
		elementValue = "-1";
	}
	//����
	else if (isNumber(line[linePf])) {		//��������DFA��һ�·ֿ���
		if (line[linePf] == '0') {
			linePf++;							//��Ӱ��ʮ���ƶ���
			regex octCheck("[01234567]");
			regex hexCheck("[0123456789abcdef]");
			if (line[linePf] == 'x') {		//ʮ������ 
				linePf++;						//��Ӱ��˽��ƶ���
				while (line[linePf] == '0')linePf++;		//���ȵ�0����
				while (regex_match(string(1, line[linePf]), hexCheck)) {
					strToken[wordPf++] = line[linePf++];
				}
				elementType = "i";
				elementValue = to_string(stoi(string(strToken), nullptr, 16));	//ʮ���� ת ʮ��int ת ʮ��string
			}
			else if (regex_match(string(1, line[linePf]), octCheck)) {		//�˽���
				while (line[linePf] == '0')linePf++;		//���ȵ�0
				while (regex_match(string(1, line[linePf]), octCheck)) {		//��һλ����oct
					strToken[wordPf++] = line[linePf++];
					elementType = "i";
					elementValue = to_string(stoi(string(strToken), nullptr, 8));
				}
			}
			else {
				elementType = "i";
				elementValue = "0";
			}
		}
		else {		//��0��ͷ��ʮ����
			while (isNumber(line[linePf])) {
				strToken[wordPf++] = line[linePf++];
			}
			elementType = "i";
			elementValue = string(strToken);
		}
	}
	//����� ����++ += ������
	else if (regex_match(string(1, line[linePf]), operatePattern)) {	//regex_match����ƥ�� ����1 -> char ת string 
		 //= �� ���� ������һλ
		strToken[wordPf++] = line[linePf++];
		elementType = string(strToken);
		elementValue = string(strToken);
	}
	//���
	else if (regex_match(string(1, line[linePf]), delimiterPattern)) {
		elementType = string(1, line[linePf]);
		elementValue = "-";
		linePf++;
	}
	linePf = getBCExp(line, linePf);	//�ų�ĩ�˿հ׷�
	return std::make_pair(elementType, elementValue);
}