#include"LL1_info.h"
	
//ȫ�ֱ���
const char* READGRAMMARFileName = "grammarText.txt";
const char* WRITEGRAMMARFileName = "LL1resultFile.txt";
const int MAXLEN = 50;
char lineToken[MAXLEN] = {};
regex N("[A-Z]");
regex T("[a-z0-9*+()&#]");
grammerStruct grammer;
unordered_set<string>nullAble;
unordered_map<string, unordered_set<string>>first, follow;
unordered_map<string, vector<pair<string,string>>>LL1_table;
stack<string>analyseStack;

void readGrammar() {
	FILE* fp = fopen(READGRAMMARFileName, "r");
	//���ж� �ļ����쳣����ֹ����
	if (fp == NULL) {
		cout << "NO file";
	}
	else {
		while (fgets(lineToken, MAXLEN, fp) != NULL) {
			scan(lineToken);
		}
	}
}

void formatPrintIntoFile() {
	if (freopen(WRITEGRAMMARFileName, "w", stdout) != NULL) {
		formatPrint();
	}
	fclose(stdout);
}

int getBC(int pos, char line[]) {
	while (line[pos] == ' ' || line[pos] == '|' || line[pos] == '\t') {
		pos++;
	}
	return pos;
}

void scan(char line[]) {
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
			grammer.P[product_left].push_back(product_right);
			//������һ�ζ�ȡ����ʼλ�� �� ����
			startPos = getBC(startPos + length + 1, line);
			moveCheck = line[startPos];
			length = 0;
		}
	}
	//����Vn Vt -- �޷����뿪ʼʱ�ķ� ��A' ����λ�� ���ս����
	for (int i = 0;i < product.length(); i++) {
		if (product[i] == '\n' || product[i] == '\0') { break; }

		string checkString = string(1, product[i]);
		if (regex_match(checkString, N)) {
			grammer.Vn[checkString] = 1;
			// Vn�����е�һ��Ԫ�� �ǿ�ʼ����S
			if (grammer.S == "") { grammer.S = checkString; }
		}
		else if (regex_match(checkString, T)) {
			grammer.Vt.insert(checkString);
		}
	}
}

void formatPrint() {
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
	cout << endl << "��ʼ����  S:  " << grammer.S;
	cout << endl << "  �ս�� Vn:  ";
	for (unordered_map<string, int>::iterator it = grammer.Vn.begin(); it != grammer.Vn.end();it++) {
		cout << it->first << " ";
	}
	cout << endl << "���ս�� Vt:  ";
	for (set<string>::iterator it = grammer.Vt.begin(); it != grammer.Vt.end();it++) {
		cout << *it << " ";
	}
}

void directLeftRecursion(string proLeft, vector<string>proRight) {
	vector<string>originPro, newPro;
	for (vector<string>::iterator it = proRight.begin(); it < proRight.end(); it++) {
		//����ݹ�
		if ((*it).find(proLeft) == 0) {
			//ȡ ��ѡʽ������Ӵ��Ĳ���
			string backSub = (*it).substr(proLeft.length());
			newPro.push_back(backSub);
		}
		else {
			originPro.push_back(*it);
		}
	}
	//�ǿ�˵��������ݹ� �����ķ�
	if (!newPro.empty()) {
		//��վɲ���ʽ
		grammer.P[proLeft].clear();

		//���·ֺŵ�����ͳһ���� A��  �� A' A''  -> A�м�¼�´����'������
		unordered_map<string, int>::iterator itProLeft = grammer.Vn.find(string(1,proLeft[0]));
		string FHCount = string(itProLeft->second, '\'');
		itProLeft->second++;
		//�����·��ս��A'  ����Vn 
		grammer.Vn[proLeft + FHCount] = 1;

		bool endInsertEmpty = false;
		for (vector<string>::iterator it = originPro.begin(); it < originPro.end(); it++) {
			//���ܲ������� A-> @A'
			if ((*it) == "@") { endInsertEmpty = true; }
			else {
				// A->e  -- A->eA'
				grammer.P[proLeft].push_back((*it) + proLeft + FHCount);
			}
		}
		//���⽫ @������ĩβ
		if (endInsertEmpty) { grammer.P[proLeft].push_back("@"); }
		for (vector<string>::iterator it = newPro.begin();it < newPro.end(); it++) {
			// A->Abc  --  A'->bcA'
			grammer.P[proLeft + FHCount].push_back((*it) + proLeft + FHCount);
		}
		//ĩβ��ӿ���
		grammer.P[proLeft + FHCount].push_back("@");
	}
}

void remove_left_recursion() {
	for (unordered_map<string, vector<string>>::iterator itI = grammer.P.begin(); itI != grammer.P.end(); itI++) {
		//�Բ���ʽitI���Դ��� ��������itJ
		for (unordered_map<string, vector<string>>::iterator itJ = grammer.P.begin(); itJ != itI;itJ++) {
			if (itJ == itI) { continue; }
			//���տ��ܷ����������ɵ��²���ʽ
			vector<string> newPro;

			//itIK����itI��vector��ѡʽ���� ��ÿһ����ѡʽ���Դ���ItJ��
			for (vector<string>::iterator itIK = itI->second.begin();itIK < itI->second.end(); itIK++) {
				//�����������ݹ�
				if ((*itIK).find(itJ->first) == 0) {
					//��itJ���к�ѡʽ�����*itIK
					for (vector<string>::iterator itJK = itJ->second.begin(); itJK != itJ->second.end();itJK++) {
						newPro.push_back(*itJK + (*itIK).substr(itJ->first.length()));
					}
				}
				//��itI�в��� itJ��ͷ��ֱ�ӷ���newPro
				else {
					newPro.push_back(*itIK);
				}
			}
			itI->second = newPro;
		}
		//����ֱ����ݹ�
		directLeftRecursion(itI->first, itI->second);
	}
}

void remove_left_gene() {
	unordered_map<string, vector<string>>candidate;
	//�����ķ������в���ʽ
	for (unordered_map<string, vector<string>>::iterator itI = grammer.P.begin();itI != grammer.P.end();itI++) {
		candidate.clear();
		//�������к�ѡʽ
		for (vector<string>::iterator itIK = itI->second.begin();itIK < itI->second.end();itIK++) {
			//�Կ�ͷ�ַ�Ϊ��
			string startStr = string(1, (*itIK)[0]);
			//�����ַ���Ϊֵ ¼��candidate
			string subVal = (*itIK).substr(1);
			candidate[startStr].push_back(subVal);
		}

		itI->second.clear();
		//��ô��ѡʽ��map ���Ƿ���Ҫ�ṫ��ʽ
		for (unordered_map<string, vector<string>>::iterator itJ = candidate.begin();itJ != candidate.end();itJ++) {
			if (itJ->second.size() == 1) {
				// A-> r1|r2  -- A->r1|r2 
				itI->second.push_back(itJ->first + itJ->second[0]);
			}
			else {
				//����else �ͱ�ʾ ��Ҫ�ṫ��ʽ
				unordered_map<string, int>::iterator itProLeft = grammer.Vn.find(string(1, itI->first[0]));
				string FHCount = string(itProLeft->second, '\'');
				itProLeft->second++;
				//�����·��ս��A'  ����Vn 
				string newVN = itI->first[0] + FHCount;
				grammer.Vn[newVN] = 1;

				//��A ���� aA' ��
				itI->second.push_back(itJ->first + itProLeft->first + FHCount);
				bool endInsertEmpty = false;
				//�������й���ʽ�ĺ�ѡʽ
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

string getFullChar(string candidate, int pos) {
	int length = 1;
	string fullVn;
	//��һλ��' �ٶ�һλ
	for (int i = pos+1;i < candidate.length();i++) {
		if (candidate[i] == '\'') {
			length++;
		}
		else { break; }
	}
	return candidate.substr(pos, length);
}

void calculate_nullAble() {
	bool nullAbleChange = true;
	while (nullAbleChange) {
		nullAbleChange = false;
		for (unordered_map<string, vector<string>>::iterator itProduct = grammer.P.begin();itProduct != grammer.P.end();itProduct++) {
			for (string itCandidate : (*itProduct).second) {
				//��������nullable �Ĳ���ʽ
				if (nullAble.find((*itProduct).first) != nullAble.end()) { continue; }

				if (itCandidate == "@") {
					nullAble.insert((*itProduct).first);
					nullAbleChange = true;
					break;
				}
				else {
					//��ѡʽȫ���� Ҳ��nullable��
					int allEmpty = 0;
					for (int i = 0;i < itCandidate.length();) {
						string fullVn = getFullChar(itCandidate, i);
						if (nullAble.find(fullVn) != nullAble.end()) {
							allEmpty++;
						}
						else { break; }
						i += fullVn.length();
					}
					if (allEmpty == itCandidate.length()) {
						nullAble.insert((*itProduct).first);
						nullAbleChange = true;
					}
				}
			}
		}
	}
	/*cout << "NullAble" << endl;
	for (string a : nullAble) {
		cout << a << endl;
	}*/
}

void calculate_first() {
	unordered_map<string, unordered_set<string>>firstCheck;
	do{
		//���first�Ƿ����
		first = firstCheck;

		for (unordered_map<string, vector<string>>::iterator itP = grammer.P.begin();itP != grammer.P.end();itP++) {
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//������ѡʽ��ÿһ������
				for (int i = 0; i < (*itCandidate).length();) {
					string fullChar = getFullChar(*itCandidate, i);
					//���ս��
					if(grammer.Vn.find(fullChar) != grammer.Vn.end()){
						//��������ս����first���� insert�������ʽ�󲿷��ŵ�first��
						firstCheck[(*itP).first].insert(firstCheck[fullChar].begin(),firstCheck[fullChar].end());

						//�����ƿ�  ֱ�ӽ���
						if (nullAble.find(fullChar) == nullAble.end()) {
							break;
						}
					}
					//���ս�� �� ��
					else {
						firstCheck[(*itP).first].insert(fullChar);
						break;
					}
					i += fullChar.length();
				}
			}
		}
	} while (firstCheck != first);

	/*cout << endl << "First" << endl;
	for (auto a : first) {
		cout << a.first << "->";
		for (string b : a.second) {
			cout << b << " ";
		}
		cout << endl;
	}*/
}

void calculate_follow() {
	unordered_map<string, unordered_set<string>>followCheck;
	followCheck[grammer.S].insert("#");
	do {
		follow = followCheck;
		for (unordered_map<string, vector<string>>::iterator itP = grammer.P.begin();itP != grammer.P.end();itP++) {
			//temp ���� follow(P->first)
			unordered_set<string>temp = followCheck[(*itP).first];
			for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
				//���������ѡʽ��ÿһ������
				for (int i = (*itCandidate).length()-1; i >= 0;i--) {
					//���ű��� ����������'
					if ((*itCandidate)[i] == '\'') {
						continue;
					}
					string fullChar = getFullChar(*itCandidate, i);
					//���ս��
					if (grammer.Vn.find(fullChar) != grammer.Vn.end()) {
						//follow(M) U= temp    ����@
						for (string symbol : temp) {
							if (symbol != "@") {
								followCheck[fullChar].insert(symbol);
							}
						}
						
						if (nullAble.find(fullChar) == nullAble.end()) {
							//�ǿ� temp = first��M)
							temp = first[fullChar];
						}
						else {
							//�� temp += first(M)
							temp.insert(first[fullChar].begin(), first[fullChar].end());
						}
					}
					//�ս�� ����������
					if(grammer.Vt.find(fullChar) != grammer.Vt.end()) {
						temp = {fullChar};
					}
				}
			}
		}
	} while (followCheck != follow);

	/*cout << endl << "FOLLOW" << endl;
	for (auto a : follow) {
		cout << a.first << "->";
		for (string b : a.second) {
			cout << b << " ";
		}
		cout << endl;
	}*/
}

void construct_LL1Table() {
	for (unordered_map<string, vector<string>>::iterator itP = grammer.P.begin(); itP != grammer.P.end();itP++) {
		for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
			//��������ʽÿ������
			for (int i = 0;i < (*itCandidate).length();) {
				string fullChar = getFullChar(*itCandidate, i);

				//�ս��
				if (grammer.Vn.find(fullChar) != grammer.Vn.end()) {
					//��M ��first�� ���� ������
					for (unordered_set<string>::iterator itfullCharFirst = first[fullChar].begin(); itfullCharFirst != first[fullChar].end(); itfullCharFirst++) {
						LL1_table[itP->first].push_back({ *itfullCharFirst, *itCandidate });
					}
					//�ǿ� ������������ѡʽ��
					if (nullAble.find(fullChar) == nullAble.end()) { break; }
				}
				//���ս��
				else if (grammer.Vt.find(fullChar) != grammer.Vt.end()) {
					LL1_table[itP->first].push_back({ fullChar, *itCandidate });
					break;
				}
				//@
				else {
					//��M ��follow�� ���� ������
					for (unordered_set<string>::iterator itfullCharFollow = follow[itP->first].begin(); itfullCharFollow != follow[itP->first].end();itfullCharFollow++) {
						LL1_table[itP->first].push_back({ *itfullCharFollow, *itCandidate });
					}
				}
				i += fullChar.length();
			}
		}
	}

	/*cout << endl << "LL1_Table" << endl;
	for (unordered_map<string, vector<pair<string, string>>>::iterator A = LL1_table.begin(); A != LL1_table.end(); A++) {
		cout << A->first << " -- ";
		for (vector<pair<string, string>>::iterator B = A->second.begin(); B != A->second.end(); B++) {
			cout << B->first << "," << B->second << "  ";
		}
		cout << endl;
	}*/
}

bool LL1_predict(string inputExpression) {
	analyseStack.push("#");
	analyseStack.push(grammer.S);

	inputExpression += "#";

	while (!analyseStack.empty()) {
		string top = analyseStack.top();
		analyseStack.pop();
		string inputExpTop = inputExpression.substr(0, 1);
		//�ս��
		if (grammer.Vn.find(top) != grammer.Vn.end()) {
			string predictString = "";
			for (vector<pair<string, string>>::iterator itLL1Table = LL1_table[top].begin();
				itLL1Table != LL1_table[top].end(); itLL1Table++) {
				if ((*itLL1Table).first == inputExpTop) {
					predictString = (*itLL1Table).second;
					break;
				}
			}
			if (predictString == "@") { continue; }

			//LL(1)�д��ڶ�Ӧ��ϵ����ջ��ѡʽ �������
			if (predictString != "") {
				//������ջ
				string fullChar;
				for (int i = predictString.length() - 1; i >= 0;i--) {
					if (predictString[i] == '\'') { continue; }
					fullChar = getFullChar(predictString, i);
					analyseStack.push(fullChar);
				}
			}
			else{ return false; }

		}
		else {
			//��ͬ���ռ���ͬʱ��ջ ������� 
			if (top == inputExpTop) {
				inputExpression = inputExpression.substr(1);
			}
			else { return false; }
		}
	}
	return inputExpression.empty();
}