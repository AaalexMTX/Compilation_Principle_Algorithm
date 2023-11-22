#include"LL1_info.h"


//ȫ�ֱ���
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

//�ļ�ɨ��¼���ķ�
void scan(char lineToken[]) {
	string product = lineToken;
	//��¼����ʽ�� �� �� �Ҳ���ѡʽ
	string product_left, product_right;
	int startPos = 0, length = 0;
	if (product.find('-', startPos) != string::npos) {
		//ȡ����ʽ��
		startPos = getBC(startPos, lineToken);
		while (regex_match(string(1, lineToken[startPos + length]), N)) {
			length++;
		}
		product_left = product.substr(startPos, length);
		//ȡ����ʽ�Ҳ�
		startPos = product.find('-', startPos) + 2;
		startPos = getBC(startPos, lineToken);
		char moveCheck = lineToken[startPos];
		length = 0;
		while (moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0' && startPos <= product.length() - 1) {
			while (moveCheck != ' ' && moveCheck != '|' && moveCheck != ';' && moveCheck != '\n' && moveCheck != '\0') {
				length++;
				moveCheck = lineToken[startPos + length];
			}
			//��ʼλ��+����	ȡ��ѡʽ 
			product_right = product.substr(startPos, length);
			grammer.P[product_left].push_back(product_right);
			//������һ�ζ�ȡ����ʼλ�� �� ����
			startPos = getBC(startPos + length + 1, lineToken);
			moveCheck = lineToken[startPos];
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

//��ʽ������ķ�
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

//���� ����ʽ��ֱ����ݹ�
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

//���� �����ݹ�
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

//��ȡ������
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