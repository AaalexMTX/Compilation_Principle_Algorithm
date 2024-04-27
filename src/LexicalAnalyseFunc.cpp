#include"../include/LL1_info.h"
#include"../include/SLR_info.h"
#include"../include/wordAnalyse_info.h"

const char* readExpressionFile = "./src/textFile/LL1/ExpressionWord.txt";
const int EXPMAXLEN = 50;
char expLineToken[EXPMAXLEN] = {};
char strToken[EXPMAXLEN] = {};

regex delimiterPattern("[()[\\]{},.;:'\"?@#$\\\\]");		//界符正则
regex operatePattern("[=+-/*%/&|!^<>]");	//运算符正则
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

void run_ReadExp_LL1() {
	FILE* fp = fopen(readExpressionFile, "r");
	if (fp == NULL) {
		cout << "文件不存在";
	}
	else {
		while (fgets(expLineToken, EXPMAXLEN, fp) != NULL) {
			cout << expLineToken;
			if (LL1_predict(ExpChange(expLineToken))) { cout << "YES" << endl; }
			else cout << "NO" << endl;
		}
		fclose(fp);
	}
}

void run_ReadExp_SLR() {
	FILE* fp = fopen(readExpressionFile, "r");
	if (fp == NULL) {
		cout << "SLR_表达式文件不存在" << endl;
	}
	else {
		while (fgets(expLineToken, EXPMAXLEN, fp) != NULL) {
			cout << endl << string(expLineToken);
			//SLR_predict(ExpChange(expLineToken));					//SLR分析结果
			SLR_predict_AnalyseStack(ExpChange(expLineToken));	//SLR分析结果+分析栈
		}
		fclose(fp);

	}
}

string ExpChange(char line[]) {
	string elementType, elementValue;
	string ansExp = "";
	int linePf = 0;
	int wordPf = 0;
	while (linePf < strlen(line) && line[linePf] != '\n') {
		linePf = getBCExp(line, linePf);		//排除开头的空白符

		int flag = 0, wordPf = 0;
		memset(strToken, 0, EXPMAXLEN);
		//符号串
		if (isLetter(line[linePf]) || line[linePf] == '_') {
			while (isLetter(line[linePf]) || isNumber(line[linePf]) || line[linePf] == '_') {
				strToken[wordPf++] = line[linePf++];
			}
			//判断是否关键字
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
		//整形
		else if (isNumber(line[linePf])) {		//三种整数DFA不一致分开读
			if (line[linePf] == '0') {
				linePf++;							//不影响十进制读数
				regex octCheck("[1234567]");
				regex hexCheck("[0123456789abcdef]");
				if (line[linePf] == 'x') {		//十六进制 
					linePf++;						//不影响八进制读数
					while (line[linePf] == '0')linePf++;		//把先导0读掉
					while (regex_match(string(1, line[linePf]), hexCheck)) {		//第一位符合hex
						strToken[wordPf++] = line[linePf++];
					}
					elementType = "Hex";
					elementValue = "i";
				}
				else if (regex_match(string(1, line[linePf]), octCheck)) {		//八进制
					while (line[linePf] == '0')linePf++;		//读先导0
					while (regex_match(string(1, line[linePf]), octCheck)) {		//第一位符合oct
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
		//运算符 包括++ +=
		//如果第一位和 运算符正则匹配
		else if (regex_match(string(1, line[linePf]), operatePattern)) {	//regex_match正则匹配 参数1 -> char 转 string 
			 //= 或 自身 再向后读一位
			strToken[wordPf++] = line[linePf++];
			if (line[linePf] == '=' || line[linePf] == line[linePf - 1]) {
				strToken[wordPf++] = line[linePf++];
			}
			elementType = strToken;
			elementValue = strToken;
		}
		//界符
		else if (regex_match(string(1, line[linePf]), delimiterPattern)) {
			elementType = string(1, line[linePf]);
			elementValue = string(1, line[linePf]);
			linePf++;
		}
		linePf = getBCExp(line, linePf);	//排除末端空白符
		if (elementValue == ";") { continue; }
		ansExp += elementValue.c_str();
	}
	return ansExp;
}