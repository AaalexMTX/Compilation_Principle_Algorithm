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
	//��һλ��' �ٶ�һλ
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
	//���ж� �ļ����쳣����ֹ����
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
		//�ļ�������˵ù�
		fclose(fp);
	}
	return true;
}

void Grammar_set::scanCandidateIntoGrammar(char line[]) {
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
	/*cout << endl << "��ʼ����  S:  " << grammar.S;
	cout << endl << "  �ս�� Vn:  ";
	for (unordered_map<string, int>::iterator it = grammar.Vn.begin(); it != grammar.Vn.end();it++) {
		cout << it->first << " ";
	}
	cout << endl << "���ս�� Vt:  ";
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
		//��ʽ������ķ�
		formatPrint();
		//�ѱ�׼����Ļؿ���̨
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
				//��������NullAble �Ĳ���ʽ
				if (NullAble.find((*itProduct).first) != NullAble.end()) { continue; }

				if (itCandidate == "@") {
					NullAble.insert((*itProduct).first);
					NullAbleChange = true;
					break;
				}
				else {
					//��ѡʽȫ���� Ҳ��NullAble��
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
		//���First�Ƿ����
		First = FirstCheck;

		for (unordered_map<string, vector<string>>::iterator itP = grammar.P.begin();itP != grammar.P.end();itP++) {
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//������ѡʽ��ÿһ������
				for (int i = 0; i < (*itCandidate).length();) {
					string fullChar = getFullChar(*itCandidate, i);
					//���ս��
					if (grammar.Vn.find(fullChar) != grammar.Vn.end()) {
						//��������ս����First���� insert�������ʽ�󲿷��ŵ�First��
						FirstCheck[(*itP).first].insert(FirstCheck[fullChar].begin(), FirstCheck[fullChar].end());

						//�����ƿ�  ֱ�ӽ���
						if (NullAble.find(fullChar) == NullAble.end()) {
							break;
						}
					}
					//���ս�� �� ��
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
				//temp ���� Follow(P->first)
				unordered_set<string>temp = FollowCheck[(*itP).first];

				//���������ѡʽ��ÿһ������
				for (int i = (*itCandidate).length() - 1; i >= 0;i--) {
					//���ű��� ����������'
					if ((*itCandidate)[i] == '\'') {
						continue;
					}
					string fullChar = getFullChar(*itCandidate, i);
					//���ս��
					if (grammar.Vn.find(fullChar) != grammar.Vn.end()) {
						//Follow(M) U= temp    ����@
						for (string symbol : temp) {
							if (symbol != "@") {
								FollowCheck[fullChar].insert(symbol);
							}
						}

						if (NullAble.find(fullChar) == NullAble.end()) {
							//�ǿ� temp = First��M)
							temp = First[fullChar];
						}
						else {
							//�� temp += First(M)
							temp.insert(First[fullChar].begin(), First[fullChar].end());
						}
					}
					//�ս�� ����������
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