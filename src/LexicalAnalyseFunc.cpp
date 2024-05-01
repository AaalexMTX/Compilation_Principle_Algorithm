#include"../include/LL1_info.h"
#include"../include/SLR_info.h"
#include"../include/wordAnalyse_info.h"
using namespace std;

regex delimiterPattern("[()[\\]{},.;:'\"?@#$\\\\]");		//界符正则
regex operatePattern("[=+-/*%/&|!^<>]");					//运算符正则
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
		pos = getBC(pos,lineToken);		//排除开头的空白符
		memset(strToken,  '\0', sizeof(strToken));
		//符号串
		if (isLetter(lineToken[pos]) || lineToken[pos] == '_') {
			while (isLetter(lineToken[pos]) || isNumber(lineToken[pos]) || lineToken[pos] == '_') {
				strToken[j++] = lineToken[pos++];
			}
			//判断是否关键字
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
		//整形
		else if (isNumber(lineToken[pos])) {		//三种整数DFA不一致分开读
			if (lineToken[pos] == '0') {
				pos++;							//不影响十进制读数
				regex octCheck("[1234567]");
				regex hexCheck("[0123456789abcdef]");
				if (lineToken[pos] == 'x') {		//十六进制 
					pos++;						//不影响八进制读数
					while (lineToken[pos] == '0')pos++;		//把先导0读掉
					while (regex_match(string(1, lineToken[pos]), hexCheck)) {		//第一位符合hex
						strToken[j++] = lineToken[pos++];
					}
					elementType = "Hex";
					elementValue = strToken;
				}
				else if (regex_match(string(1, lineToken[pos]), octCheck)) {		//八进制
					while (lineToken[pos] == '0')pos++;		//读先导0
					while (regex_match(string(1, lineToken[pos]), octCheck)) {		//第一位符合oct
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
		//运算符 包括++ +=
		//如果第一位和 运算符正则匹配
		else if (regex_match(string(1, lineToken[pos]), operatePattern)) {	//regex_match正则匹配 参数1 -> char 转 string 
			 //= 或 自身 再向后读一位
			strToken[j++] = lineToken[pos++];
			if (lineToken[pos] == '=' || lineToken[pos] == lineToken[pos - 1]) {
				strToken[j++] = lineToken[pos++];
			}
			elementType = strToken;
			elementValue = "-";
		}
		//界符
		else if (regex_match(string(1, lineToken[pos]), delimiterPattern)) {
			elementType = string(1, lineToken[pos++]);
			elementValue = "-";
		}
		//错误字符

		printf("<%s , %s>\n", &elementType[0], elementValue.c_str());
		pos = getBC(pos,lineToken);	//排除末端空白符
	}
}

bool readWordFile(const char* wordFile) {
	FILE* fp = fopen(wordFile, "r");
	//无判断 文件打开异常会中止程序
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
		//文件句柄开了得关
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
		linePf = getBCExp(line, linePf);		//排除开头的空白符

		int flag = 0, wordPf = 0;
		memset(strToken, 0, 50);
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

pair<string, string> wordChange(string& line, int& linePf) {
	string elementType, elementValue;	//类型 和 语义值
	int wordPf = 0;						//遍历完整单词用
	//一次分析
	linePf = getBCExp(line, linePf);							//排除开头的空白符
	memset(strToken, 0, sizeof(strToken));			//清空单词队列
	//符号串
	if (isLetter(line[linePf]) || line[linePf] == '_') {
		while (isLetter(line[linePf]) || isNumber(line[linePf]) || line[linePf] == '_') {
			strToken[wordPf++] = line[linePf++];
		}
		//无需 判断是否关键字
		elementType = "str";
		elementValue = "-1";
	}
	//整形
	else if (isNumber(line[linePf])) {		//三种整数DFA不一致分开读
		if (line[linePf] == '0') {
			linePf++;							//不影响十进制读数
			regex octCheck("[01234567]");
			regex hexCheck("[0123456789abcdef]");
			if (line[linePf] == 'x') {		//十六进制 
				linePf++;						//不影响八进制读数
				while (line[linePf] == '0')linePf++;		//把先导0读掉
				while (regex_match(string(1, line[linePf]), hexCheck)) {
					strToken[wordPf++] = line[linePf++];
				}
				elementType = "i";
				elementValue = to_string(stoi(string(strToken), nullptr, 16));	//十六进 转 十进int 转 十进string
			}
			else if (regex_match(string(1, line[linePf]), octCheck)) {		//八进制
				while (line[linePf] == '0')linePf++;		//读先导0
				while (regex_match(string(1, line[linePf]), octCheck)) {		//第一位符合oct
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
		else {		//非0开头的十进制
			while (isNumber(line[linePf])) {
				strToken[wordPf++] = line[linePf++];
			}
			elementType = "i";
			elementValue = string(strToken);
		}
	}
	//运算符 包括++ += 不考虑
	else if (regex_match(string(1, line[linePf]), operatePattern)) {	//regex_match正则匹配 参数1 -> char 转 string 
		 //= 或 自身 再向后读一位
		strToken[wordPf++] = line[linePf++];
		elementType = string(strToken);
		elementValue = string(strToken);
	}
	//界符
	else if (regex_match(string(1, line[linePf]), delimiterPattern)) {
		elementType = string(1, line[linePf]);
		elementValue = "-";
		linePf++;
	}
	linePf = getBCExp(line, linePf);	//排除末端空白符
	return std::make_pair(elementType, elementValue);
}