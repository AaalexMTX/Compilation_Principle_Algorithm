#include"../include/LL1_info.h"
#include"../include/SLR_info.h"
#include"../include/wordAnalyse_info.h"
using namespace std;

const char* readExpressionFile = "./src/textFile/ExpressionWord.txt";
char strToken[50] = {};

regex delimiterPattern("[()[\\]{},.;:'\"?@#$\\\\]");		//�������
regex operatePattern("[=+-/*%/&|!^<>]");	//���������
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