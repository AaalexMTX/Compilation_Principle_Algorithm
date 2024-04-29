#include"../include/LL1_info.h"
#include"../include/RPN_info.h"
#include"../include/wordAnalyse_info.h"
#include"../include/Common_data.h"
using namespace std;

bool RPNClass::readSLRGrammar() {
	FILE* fp = fopen(readSLRGrammarFile, "r");
	//���ж� �ļ����쳣����ֹ����
	if (fp == NULL) {
		cout << "SLRGrammarFile Not Exist";
		return false;
	}
	else {
		while (fgets(lineToken, 50, fp) != NULL) {
			scan(lineToken);
		}
		//�ļ�������˵ù�
		fclose(fp);
	}
	return true;
}

items_Node RPNClass::itemsNodeClosure(const items_Node& oriNode) {
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

items_Node RPNClass::itemsNodeGoto(const items_Node& oriNode, string X) {
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

bool RPNClass::itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode) {
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

void RPNClass::constructVTN() {
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

void RPNClass::SLRAnaTableConstruct(items_Node& oriNode) {
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

void RPNClass::printSLRTabel() {
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

void RPNClass::SLR_predict(string inputExpression) {
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

void RPNClass::SLR_predict_AnalyseStack(string inputExpression) {
	while (!State_Stack.empty()) { State_Stack.pop(); }
	while (!Symbol_Stack.empty()) { Symbol_Stack.pop(); }
	State_Stack.push(0);
	Symbol_Stack.push("#");
	inputExpression += "#";

	printf("    ״̬ջ    |    ����ջ    |    ���봮    |\t��Ӧ����\n");
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

void RPNClass::run_ReadExp_SLR() {
	FILE* fp = fopen(readExpressionFile, "r");
	if (fp == NULL) {
		cout << "SLR_���ʽ�ļ�������" << endl;
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

void RPNClass::calculate_NullAble() {
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

void RPNClass::calculate_First() {
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

void RPNClass::calculate_Follow() {
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

void RPNClass::scan(char line[]) {
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

// 3. RPN
int RPNClass::calcReversePolistNotation(string& RPN) {
	stack<int>RPN_stack;
	while (!RPN.empty()) {
		int wordLength = 0;
		pair<string, string>rpnNode = wordChange(RPN, wordLength);
		if (rpnNode.first == "i") {
			RPN_stack.push(atoi(rpnNode.second.c_str()));
		}
		else if (rpnNode.second == "+") {
			int op1 = RPN_stack.top(); RPN_stack.pop();
			int op2 = RPN_stack.top(); RPN_stack.pop();
			RPN_stack.push(op1 + op2);
		}
		else if (rpnNode.second == "*") {
			int op1 = RPN_stack.top(); RPN_stack.pop();
			int op2 = RPN_stack.top(); RPN_stack.pop();
			RPN_stack.push(op1 * op2);
		}
		else if (rpnNode.first == ",") {
			;
		}
		RPN = RPN.substr(wordLength);
	}
	return RPN_stack.top();
}

//2. RPN
void RPNClass::semanticAction(int actionIndex, pair<string, string>& proLeftNode) {
	stack<pair<string, string>>tempSymbol_Stack_Semantic = Symbol_Stack_Semantic;
	pair<string, string>Top, Mid, End;
	if (actionIndex == 1 || actionIndex == 3 || actionIndex == 5) {
		Top = tempSymbol_Stack_Semantic.top(), tempSymbol_Stack_Semantic.pop();
		Mid = tempSymbol_Stack_Semantic.top(), tempSymbol_Stack_Semantic.pop();
		End = tempSymbol_Stack_Semantic.top();
	}
	switch (actionIndex) {
	case 0:
		printf("\n�沨��ʽ -> %s", proLeftNode.second.c_str());
		break;
	case 1:
		proLeftNode.second = (End.second + "," + Top.second + "," + "+");
		break;
	case 2:
		proLeftNode.second = tempSymbol_Stack_Semantic.top().second;
		break;
	case 3:
		proLeftNode.second = (End.second + "," + Top.second + "," + "*");
		break;
	case 4:
		proLeftNode.second = tempSymbol_Stack_Semantic.top().second;
		break;
	case 5:
		proLeftNode.second = (Mid.second);
		break;
	case 6:
		proLeftNode.second = tempSymbol_Stack_Semantic.top().second;
		break;
	default:
		break;
	}

}


//1. RPN
void RPNClass::SLR_predict_SemanticAnalyse_AnalyseStack(const char* line) {
	while (!Symbol_Stack_Semantic.empty()) { Symbol_Stack_Semantic.pop(); }
	while (!State_Stack.empty()) { State_Stack.pop(); }
	string inputExpression = string(line);
	//ɾ���ܴ��ڵ�ĩβ�ֺ� ���з�
	size_t nIndex = inputExpression.find("\n");
	size_t fIndex = inputExpression.find(";");
	if (fIndex != string::npos && nIndex != string::npos) { inputExpression = inputExpression.substr(0, fIndex); }
	else if (fIndex != string::npos) { inputExpression = inputExpression.substr(0, nIndex); }
	else if (nIndex != string::npos) { inputExpression = inputExpression.substr(0, nIndex); }

	inputExpression += "#";
	State_Stack.push(0);
	Symbol_Stack_Semantic.push(std::make_pair("#", "-"));

	printf("    ״̬ջ    |    ����ջ    |            ����ջ              |        ���봮        |\t��Ӧ����\n");
	while (true) {
		//ջ���ݵ����
		stack<int>tempState_stack = State_Stack;
		stack<pair<string, string>>tempSymbol_stack = Symbol_Stack_Semantic;
		string StateStackContent = "";
		string SymbolStackContent = "";
		string SemanticContent = "";
		while (!tempState_stack.empty()) {
			StateStackContent = to_string(tempState_stack.top()) + StateStackContent;
			tempState_stack.pop();
		}
		while (!tempSymbol_stack.empty()) {
			SymbolStackContent = tempSymbol_stack.top().first + SymbolStackContent;
			string topStr = tempSymbol_stack.top().second;
			if (topStr.find(",") != string::npos) { topStr = "(" + topStr + ")"; }
			SemanticContent = topStr + SemanticContent;
			tempSymbol_stack.pop();
		}

		printf("%-14s|%-14s%|%-32s|%-22s|", StateStackContent.c_str(), SymbolStackContent.c_str(), SemanticContent.c_str(), inputExpression.c_str());

		//��������
		int topState = State_Stack.top();
		int wordLength = 0;
		pair<string, string> topInput = wordChange(inputExpression, wordLength);
		//��״̬�任��ʽ
		string nextActionState = " ";
		for (pair<string, string> gotoTransPair : Action[topState]) {
			if (gotoTransPair.first == topInput.first) {
				nextActionState = gotoTransPair.second;
				break;
			}
		}

		if (nextActionState[0] == 'S') {
			State_Stack.push(atoi(nextActionState.substr(1).c_str()));
			Symbol_Stack_Semantic.push(topInput);
			inputExpression = inputExpression.substr(wordLength);

			printf("\t%-4s\n", nextActionState.c_str());
		}
		else if (nextActionState[0] == 'R') {
			int productionIndex = atoi(nextActionState.substr(1).c_str());
			string production = Productions[productionIndex];
			string proLeft = getFullChar(production, 0);
			string proRight = production.substr(production.find(">") + 1);

			//!!!!!!!!���������ָ��
			pair<string, string>proLeftNode = std::make_pair(proLeft, "-");
			semanticAction(productionIndex, proLeftNode);	//ִ�� ����ʽ��Ӧ����
			if (proRight != "@") {
				for (int i = 0;i < proRight.size();i++) {
					State_Stack.pop();
					Symbol_Stack_Semantic.pop();
				}
			}

			printf("\t%-4s\n", nextActionState.c_str());

			//�ñ�¶��ջ��״̬ ��ѹ�����ʽ��
			topState = State_Stack.top();
			Symbol_Stack_Semantic.push(proLeftNode);

			int nextGotoState = -1;
			for (pair<string, string>gotoTransPair : Goto[topState]) {
				if (gotoTransPair.first == proLeft) {
					nextGotoState = atoi(gotoTransPair.second.c_str());	//״̬stringתint
					break;
				}
			}
			if (nextGotoState == -1) {
				cout << "WA_Goto\n";
				break;
			}
			else {
				State_Stack.push(nextGotoState);
			}
		}
		else if (nextActionState == "acc") {
			printf("\tACC\n");
			pair<string, string>SNode = Symbol_Stack_Semantic.top();
			semanticAction(0, SNode);
			cout << "   " << calcReversePolistNotation(SNode.second) << endl;
			cout << "YES\n\n";
			break;
		}
		else {
			cout << "\tWA\n";
			cout << "WA_Action\n\n";
			break;
		}
	}
}