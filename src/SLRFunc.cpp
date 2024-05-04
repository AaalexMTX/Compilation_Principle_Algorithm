#include"../include/LL1_info.h"
#include"../include/SLR_info.h"
#include"../include/wordAnalyse_info.h"
#include"../include/Common_data.h"
using namespace std;

items_Node SLRClass::itemsNodeClosure(const items_Node& oriNode) {
	//��հ������Ŀ��
	items_Node WholeNode(oriNode);
	//�����Vn�ظ���չ
	unordered_map<string, bool>VnExtendCheck;

	//����vectorβ�� ���ü��
	bool setChangeFlag = true;
	while (setChangeFlag) {
		setChangeFlag = false;
		//������Ŀ����������Ŀvector
		for (int i = 0;i < WholeNode.items_Set.size();i++) {
			//����� �ĺ��ַ���Vn ��δ���� ����չ
			string nextStr = getFullChar(WholeNode.items_Set[i].ProCandidate, WholeNode.items_Set[i].point + 1);
			if (grammar.Vn.find(nextStr) != grammar.Vn.end() &&
				VnExtendCheck[nextStr] == false) {
				//�����ս�������к�ѡʽ תΪitem���� ��Ŀ����vector
				for (string nextCandidate : grammar.P[nextStr]) {
					item nextCanitem(nextStr, nextCandidate, -1);
					WholeNode.items_Set.push_back(nextCanitem);
					setChangeFlag = true;
				}
				//����Vn��״̬��Ϊ����չ
				VnExtendCheck[nextStr] = true;
			}
		}
	}
	return WholeNode;
}

items_Node SLRClass::itemsNodeGoto(const items_Node& oriNode, string X) {
	items_Node gotoNode;		//�޲ι���һ���յ���Ŀ����
	gotoNode.state_id = itemsNodeSet.size();

	//�������к�ѡʽ��� ����Ƿ��� X��ͷ
	for (item p : oriNode.items_Set) {
		//����Ŀ ����ĩβ����
		if (p.point + 1 > p.ProCandidate.length()) { continue; };
		string nextStr = getFullChar(p.ProCandidate, p.point + 1);
		if (nextStr == X) {
			//���� ����Ƶ�����Ŀ 
			item newItem(p.Proleft, p.ProCandidate, p.point + 1);
			gotoNode.items_Set.push_back(newItem);
			gotoNode.originNum += 1;
		}
	}
	return gotoNode;
}

bool SLRClass::itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode) {
	//���ں���item�����ɳ�ʼitem��չ����
	//�������itemNode����ʼ��Ŀ��һ�� ��Ϊ��ͬ
	if (oriNode.originNum == nextNode.originNum) {
		//˳��һ����ͬ�𣿣�����
		for (int i = 0;i < oriNode.originNum;i++) {
			if (oriNode.items_Set[i] != nextNode.items_Set[i]) {
				return false;
			}
		}
	}
	else { return false; }
	return true;
}

void SLRClass::constructVTN() {
	for (string itVN : Productions) {
		for (char itcharVn : itVN) {
			string Vn = string(1, itcharVn);
			auto it = std::find(VNT.begin(), VNT.end(), Vn);
			if (regex_match(Vn, N) && it == VNT.end()) {
				VNT.push_back(Vn);
			}
		}
	}
	for (string itVT : Productions) {
		for (char itcharVn : itVT) {
			string Vt = string(1, itcharVn);
			auto it = std::find(VNT.begin(), VNT.end(), Vt);
			if (regex_match(Vt, T) && it == VNT.end()) {
				VNT.push_back(Vt);
			}
		}
	}
	VNT.push_back("#");
}

void SLRClass::SLRAnaTableConstruct(items_Node& oriNode) {
	constructVTN();
	oriNode = itemsNodeClosure(oriNode);	//����ʼNode ��հ�
	itemsNodeSet.push_back(oriNode);	//������item����
	queue<items_Node>Q;					//����ѭ������
	Q.push(oriNode);
	while (!Q.empty()) {
		items_Node frontNode = Q.front();
		Q.pop();
		for (string X : VNT) {
			items_Node nextNode = itemsNodeGoto(frontNode, X);
			//����ת��
			if (!nextNode.items_Set.empty()) {
				nextNode = itemsNodeClosure(nextNode);		//��հ�
				//���set���Ƿ��Ѿ��и�Node�ļ�¼
				bool checkNodeInSet = false;
				for (items_Node itNode : itemsNodeSet) {
					if (itemsNodeCheckSame(nextNode, itNode)) {
						nextNode = itNode;		//�� ֱ�����Ѿ����ڵ�(id��ͬ)
						checkNodeInSet = true;
					}
				}
				//����set�� ���������set
				if (!checkNodeInSet) {
					itemsNodeSet.push_back(nextNode);
					Q.push(nextNode);
					//�����ܳ��ֵĹ�Լ ����Action ����Ri
					for (int i = 0;i < nextNode.originNum;i++) {
						//���������� ĩβ���Թ�Լ
						if (nextNode.items_Set[i].point + 1 >= nextNode.items_Set[i].ProCandidate.length()) {
							//ƴ����������ʽ ����������
							string production = nextNode.items_Set[i].Proleft + "->" + nextNode.items_Set[i].ProCandidate;
							auto it = std::find(Productions.begin(), Productions.end(), production);
							int proIndex = std::distance(Productions.begin(), it);

							if (proIndex == 0) {
								Action[nextNode.state_id].push_back(std::make_pair("#", "acc"));
							}
							else {
								//�� follow����������vt ����action
								for (string vt : Follow[nextNode.items_Set[i].Proleft]) {
									Action[nextNode.state_id].push_back(make_pair(vt, "R" + to_string(proIndex)));
								}
							}
						}
					}
					//����ֻ�п�������չ����	����Action ����Ri
					for (int i = nextNode.originNum;i < nextNode.items_Set.size();i++) {
						if (nextNode.items_Set[i].ProCandidate == "@") {
							item traversalItem = nextNode.items_Set[i];
							//ƴ����������ʽ ����������
							string production = traversalItem.Proleft + "->" + traversalItem.ProCandidate;
							auto it = std::find(Productions.begin(), Productions.end(), production);
							int proIndex = std::distance(Productions.begin(), it);

							for (string vt : Follow[traversalItem.Proleft]) {
								Action[nextNode.state_id].push_back(std::make_pair(vt, "R" + to_string(proIndex)));
							}
						}
					}
				}

				//��X��Vn ����Goto
				if (grammar.Vn.find(X) != grammar.Vn.end()) {
					// 0 -> <E,1>
					pair<string, string>gotoPair(X, to_string(nextNode.state_id));
					Goto[frontNode.state_id].push_back(gotoPair);
				}
				//��Vt ����Action ����Si
				else {
					pair<string, string>actionPair(X, "S" + to_string(nextNode.state_id));
					Action[frontNode.state_id].push_back(actionPair);
				}
			}
		}
	}
}

void SLRClass::printSLRTabel() {
	printf("      |\t\tACTION\t\t    |    GOTO\t\n");
	//printf("  ״̬|  +    *   (     )    i    # | E   T   F\n");
	printf("  ״̬|");
	for (int i = grammar.Vn.size();i < VNT.size();i++) {
		printf(" %-3s ", VNT[i].c_str());
	}
	cout << "|";
	for (int i = 1;i < grammar.Vn.size();i++) {
		printf(" %-3s", VNT[i].c_str());
	}
	cout << endl;

	int coutPos = 0;
	int tableLength = Action.size();
	while (tableLength--) {
		printf(" %4d |", coutPos);
		vector<int>coutOnceCheck(VNT.size(), 0);
		for (int i = 0; i < VNT.size(); i++) {
			//�������ս�� �����
			if (grammar.Vn.find(VNT[i]) != grammar.Vn.end()) { continue; }
			bool checkChar = false;
			for (pair<string, string>& ACPair : Action[coutPos]) {
				//������� (i,R2) && δ������
				if (ACPair.first == VNT[i] && coutOnceCheck[i] == 0) {
					//cout<<" "<<ACPair.second << " ";
					printf(" %-3s", ACPair.second.c_str());
					checkChar = true;
					break;
				}
			}
			if (!checkChar) { cout << "    "; }
			cout << "|";
		}
		for (int i = 0; i < VNT.size(); i++) {
			if (grammar.Vt.count(VNT[i]) != 0 || VNT[i] == grammar.S) { continue; }
			bool checkChar = false;
			for (pair<string, string>ACPair : Goto[coutPos]) {
				//������� (i,R2) && δ������
				if (ACPair.first == VNT[i] && coutOnceCheck[i] == 0) {
					//cout <<"  " << ACPair.second << " ";
					printf(" %-3s", ACPair.second.c_str());
					checkChar = true;
					break;
				}
			}
			if (!checkChar) { cout << "    "; }
			//cout << "|";
		}
		cout << endl;
		coutPos++;
	}
}

void SLRClass::SLR_predict(string inputExpression) {
	while (!State_Stack.empty()) { State_Stack.pop(); }
	while (!Symbol_Stack.empty()) { Symbol_Stack.pop(); }
	State_Stack.push(0);
	Symbol_Stack.push("#");
	inputExpression += "#";

	while (true) {
		int topState = State_Stack.top();
		string topInput = string(1, inputExpression[0]);

		string nextActionState = "";
		//������Ӧ״̬
		for (pair<string, string> gotoTransPair : Action[topState]) {
			if (gotoTransPair.first == topInput) {
				nextActionState = gotoTransPair.second;
				break;
			}
		}
		if (nextActionState[0] == 'S') {
			//�ƽ�
			Symbol_Stack.push(topInput);
			State_Stack.push(atoi(nextActionState.substr(1).c_str()));		//���ַ���תint ѹ��״̬ջ
			inputExpression = inputExpression.substr(1);
		}
		else if (nextActionState[0] == 'R') {
			//��Լ
			int productIndex = atoi(nextActionState.substr(1).c_str());
			string production = Productions[productIndex];
			string proLeft = getFullChar(production, 0);
			string proRight = production.substr(production.find(">") + 1);

			// ֻ�赯����Ӧ�������� ������������ƥ��
			// ���ֲ��� ��������
			if (proRight != "@") {
				for (int i = 0;i < proRight.length();i++) {
					State_Stack.pop();
					Symbol_Stack.pop();
				}
			}

			//������ʽ�� ѹ�����ջ�����õ���¶����״̬
			Symbol_Stack.push(proLeft);
			topState = State_Stack.top();
			int nextGotoState = -1;
			for (pair<string, string>gotoTransPair : Goto[topState]) {
				if (gotoTransPair.first == proLeft) {
					nextGotoState = atoi(gotoTransPair.second.c_str());	//״̬stringתint
					break;
				}
			}
			if (nextGotoState == -1) {
				//������״̬ת�� ���� ?���ܴ�����
				cout << "NO_GOTOTRANS";
				break;
			}
			else {
				State_Stack.push(nextGotoState);
			}
		}
		else if (nextActionState == "acc") {
			//����
			cout << "YES" << endl;
			break;
		}
		else {
			//������״̬ת�� ����
			cout << "NO_ACTIONTRANS" << endl;
			break;
		}
	}
}

void SLRClass::SLR_predict_AnalyseStack(string inputExpression) {
	while (!State_Stack.empty()) { State_Stack.pop(); }
	while (!Symbol_Stack.empty()) { Symbol_Stack.pop(); }
	State_Stack.push(0);
	Symbol_Stack.push("#");
	inputExpression += "#";
	printf("  state stack | symbol stack |        semantic stack          |      input string    |\toperation\n");
	//printf("    ״̬ջ    |    ����ջ    |    ���봮    |\t��Ӧ����\n");
	while (true) {
		//ջ���ݵ����
		stack<int>tempState_stack = State_Stack;
		stack<string>tempSymbol_stack = Symbol_Stack;
		string StateStackContent = "";
		string SymbolStackContent = "";
		while (!tempState_stack.empty()) {
			StateStackContent = to_string(tempState_stack.top()) + StateStackContent;
			tempState_stack.pop();
		}
		while (!tempSymbol_stack.empty()) {
			SymbolStackContent = tempSymbol_stack.top() + SymbolStackContent;
			tempSymbol_stack.pop();
		}
		printf("%-14s|%-14s%|%-14s|", StateStackContent.c_str(), SymbolStackContent.c_str(), inputExpression.c_str());

		//��������
		int topState = State_Stack.top();
		string topInput = string(1, inputExpression[0]);

		string nextActionState = "";
		//������Ӧ״̬
		for (pair<string, string> gotoTransPair : Action[topState]) {
			if (gotoTransPair.first == topInput) {
				nextActionState = gotoTransPair.second;
				break;
			}
		}
		if (nextActionState[0] == 'S') {
			//�ƽ�
			Symbol_Stack.push(topInput);
			State_Stack.push(atoi(nextActionState.substr(1).c_str()));		//���ַ���תint ѹ��״̬ջ
			inputExpression = inputExpression.substr(1);

			printf("�ƽ� -> %-4s\n", nextActionState.c_str());
		}
		else if (nextActionState[0] == 'R') {
			//��Լ
			int productIndex = atoi(nextActionState.substr(1).c_str());
			//�õ����ڹ�Լ�Ĳ���ʽ �󲿺��Ҳ�
			string production = Productions[productIndex];
			string proLeft = getFullChar(production, 0);
			string proRight = production.substr(production.find(">") + 1);

			// ֻ�赯����Ӧ�������� ������������ƥ��
			// ���ֲ��� ��������
			if (proRight != "@") {
				for (int i = 0;i < proRight.length();i++) {
					State_Stack.pop();
					Symbol_Stack.pop();
				}
			}

			printf("��Լ -> %-4s :%-7s\n", nextActionState.c_str(), production.c_str());

			//������ʽ�� ѹ�����ջ�����õ���¶����״̬
			Symbol_Stack.push(proLeft);
			topState = State_Stack.top();
			int nextGotoState = -1;
			for (pair<string, string>gotoTransPair : Goto[topState]) {
				if (gotoTransPair.first == proLeft) {
					nextGotoState = atoi(gotoTransPair.second.c_str());	//״̬stringתint
					break;
				}
			}
			if (nextGotoState == -1) {
				//������״̬ת�� ���� ?���ܴ�����
				cout << "WA" << endl;
				cout << "NO_GOTOTRANS";
				break;
			}
			else {
				State_Stack.push(nextGotoState);
			}
		}
		else if (nextActionState == "acc") {
			//����
			cout << "ACC" << endl;
			cout << "YES" << endl;
			break;
		}
		else {
			//������״̬ת�� ����
			cout << "WA" << endl;
			cout << "NO_ACTIONTRANS" << endl;
			break;
		}
	}
}

void SLRClass::run_ReadExp_SLR() {
	FILE* fp = fopen(readSLRExpressionFile, "r");
	if (fp == NULL) {
		//cout << "SLR_���ʽ�ļ�������" << endl;
		cout << "read SLR Expression File Not Exist \n" << endl;
	}
	else {
		while (fgets(expLineToken, 50, fp) != NULL) {
			cout << endl << string(expLineToken);
			//SLR_predict(ExpChange(expLineToken));					//SLR�������
			SLR_predict_AnalyseStack(ExpChange(expLineToken));	//SLR�������+����ջ
		}
		fclose(fp);
	}
}
