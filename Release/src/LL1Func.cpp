#include"../include/LL1_info.h"
#include"../include/wordAnalyse_info.h"
#include"../include/Common_data.h"
using namespace std;

void LL1Class::directLeftRecursion(string proLeft, vector<string>proRight) {
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
		grammar.P[proLeft].clear();

		//���·ֺŵ�����ͳһ���� A��  �� A' A''  -> A�м�¼�´����'������
		unordered_map<string, int>::iterator itProLeft = grammar.Vn.find(string(1, proLeft[0]));
		string FHCount = string(itProLeft->second, '\'');
		itProLeft->second++;
		//�����·��ս��A'  ����Vn 
		grammar.Vn[proLeft + FHCount] = 1;

		bool endInsertEmpty = false;
		for (vector<string>::iterator it = originPro.begin(); it < originPro.end(); it++) {
			//���ܲ������� A-> @A'
			if ((*it) == "@") { endInsertEmpty = true; }
			else {
				// A->e  -- A->eA'
				grammar.P[proLeft].push_back((*it) + proLeft + FHCount);
			}
		}
		//���⽫ @������ĩβ
		if (endInsertEmpty) { grammar.P[proLeft].push_back("@"); }
		for (vector<string>::iterator it = newPro.begin();it < newPro.end(); it++) {
			// A->Abc  --  A'->bcA'
			grammar.P[proLeft + FHCount].push_back((*it) + proLeft + FHCount);
		}
		//ĩβ��ӿ���
		grammar.P[proLeft + FHCount].push_back("@");
	}
}

void LL1Class::remove_left_recursion() {
	for (unordered_map<string, vector<string>>::iterator itI = grammar.P.begin(); itI != grammar.P.end(); itI++) {
		//�Բ���ʽitI���Դ��� ��������itJ
		for (unordered_map<string, vector<string>>::iterator itJ = grammar.P.begin(); itJ != itI;itJ++) {
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

void LL1Class::remove_left_gene() {
	unordered_map<string, vector<string>>candidate;
	//�����ķ������в���ʽ
	for (unordered_map<string, vector<string>>::iterator itI = grammar.P.begin();itI != grammar.P.end();itI++) {
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
				unordered_map<string, int>::iterator itProLeft = grammar.Vn.find(string(1, itI->first[0]));
				string FHCount = string(itProLeft->second, '\'');
				itProLeft->second++;
				//�����·��ս��A'  ����Vn 
				string newVN = itI->first[0] + FHCount;
				grammar.Vn[newVN] = 1;

				//��A ���� aA' ��
				itI->second.push_back(itJ->first + itProLeft->first + FHCount);
				bool endInsertEmpty = false;
				//�������й���ʽ�ĺ�ѡʽ
				for (vector<string>::iterator itJK = itJ->second.begin();itJK != itJ->second.end();itJK++) {
					//A->ab1|ab2|a  --	A'->b1|b2|@
					if (*itJK != "") {
						grammar.P[newVN].push_back(*itJK);
					}
					else { endInsertEmpty = true; }
				}
				if (endInsertEmpty) { grammar.P[newVN].push_back("@"); }
			}
		}
	}
}

void LL1Class::construct_LL1Table() {
	for (unordered_map<string, vector<string>>::iterator itP = grammar.P.begin(); itP != grammar.P.end();itP++) {
		for (vector<string>::iterator itCandidate = itP->second.begin();itCandidate != itP->second.end();itCandidate++) {
			//��������ʽÿ������
			for (int i = 0;i < (*itCandidate).length();) {
				string fullChar = getFullChar(*itCandidate, i);

				//�ս��
				if (grammar.Vn.find(fullChar) != grammar.Vn.end()) {
					//��M ��First�� ���� ������
					for (unordered_set<string>::iterator itfullCharFirst = First[fullChar].begin(); itfullCharFirst != First[fullChar].end(); itfullCharFirst++) {
						LL1_table[itP->first].push_back({ *itfullCharFirst, *itCandidate });
					}
					//�ǿ� ������������ѡʽ��
					if (NullAble.find(fullChar) == NullAble.end()) { break; }
				}
				//���ս��
				else if (grammar.Vt.find(fullChar) != grammar.Vt.end()) {
					LL1_table[itP->first].push_back({ fullChar, *itCandidate });
					break;
				}
				//@
				else {
					//��M ��Follow�� ���� ������
					for (unordered_set<string>::iterator itfullCharFollow = Follow[itP->first].begin(); itfullCharFollow != Follow[itP->first].end();itfullCharFollow++) {
						LL1_table[itP->first].push_back({ *itfullCharFollow, *itCandidate });
					}
				}
				i += fullChar.length();
			}
		}
	}

	cout << endl << " ---- LL1_table ----  " << endl;
	for (unordered_map<string, vector<pair<string, string>>>::iterator a = LL1_table.begin(); a != LL1_table.end(); a++) {
		cout << a->first << " -- ";
		for (vector<pair<string, string>>::iterator b = a->second.begin(); b != a->second.end(); b++) {
			cout << b->first << "," << b->second << "  ";
		}
		cout << endl;
	}
}

bool LL1Class::LL1_predict(string inputExpression) {
	while (!analyseStack.empty()) { analyseStack.pop(); }
	analyseStack.push("#");
	analyseStack.push(grammar.S);

	inputExpression += "#";

	while (!analyseStack.empty()) {
		string top = analyseStack.top();
		analyseStack.pop();
		string inputExpTop = inputExpression.substr(0, 1);
		//�ս��
		if (grammar.Vn.find(top) != grammar.Vn.end()) {
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
			else { return false; }

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

void LL1Class::run_ReadExp_LL1() {
	FILE* fp = fopen(readLL1ExpressionFile, "r");
	if (fp == NULL) {
		cout << "Expression File Not Exist";
	}
	else {
		while (fgets(expLineToken, 50, fp) != NULL) {
			cout << expLineToken;
			if (LL1_predict(ExpChange(expLineToken))) { cout << "YES" << endl; }
			else cout << "NO" << endl;
		}
		fclose(fp);
	}
}
