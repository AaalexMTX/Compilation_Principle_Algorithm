#include"../include/LL1_info.h"
#include"../include/SLR_info.h"
#include"../include/wordAnalyse_info.h"
#include"../include/Common_data.h"
using namespace std;

items_Node SLRClass::itemsNodeClosure(const items_Node& oriNode) {
	//解闭包后的项目集
	items_Node WholeNode(oriNode);
	//避免对Vn重复拓展
	unordered_map<string, bool>VnExtendCheck;

	//由于vector尾插 不用检测
	bool setChangeFlag = true;
	while (setChangeFlag) {
		setChangeFlag = false;
		//遍历项目集中所有项目vector
		for (int i = 0;i < WholeNode.items_Set.size();i++) {
			//如果点 的后字符是Vn 且未访问 则拓展
			string nextStr = getFullChar(WholeNode.items_Set[i].ProCandidate, WholeNode.items_Set[i].point + 1);
			if (grammar.Vn.find(nextStr) != grammar.Vn.end() &&
				VnExtendCheck[nextStr] == false) {
				//将非终结符的所有候选式 转为item加入 项目集的vector
				for (string nextCandidate : grammar.P[nextStr]) {
					item nextCanitem(nextStr, nextCandidate, -1);
					WholeNode.items_Set.push_back(nextCanitem);
					setChangeFlag = true;
				}
				//将该Vn的状态置为已拓展
				VnExtendCheck[nextStr] = true;
			}
		}
	}
	return WholeNode;
}

items_Node SLRClass::itemsNodeGoto(const items_Node& oriNode, string X) {
	items_Node gotoNode;		//无参构造一个空的项目集合
	gotoNode.state_id = itemsNodeSet.size();

	//遍历所有候选式检测 点后是否以 X开头
	for (item p : oriNode.items_Set) {
		//该项目 点在末尾结束
		if (p.point + 1 > p.ProCandidate.length()) { continue; };
		string nextStr = getFullChar(p.ProCandidate, p.point + 1);
		if (nextStr == X) {
			//构造 点后移的新项目 
			item newItem(p.Proleft, p.ProCandidate, p.point + 1);
			gotoNode.items_Set.push_back(newItem);
			gotoNode.originNum += 1;
		}
	}
	return gotoNode;
}

bool SLRClass::itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode) {
	//由于后续item都是由初始item扩展来的
	//因此若两itemNode若初始项目集一致 认为相同
	if (oriNode.originNum == nextNode.originNum) {
		//顺序一定相同吗？？？？
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
	oriNode = itemsNodeClosure(oriNode);	//将初始Node 求闭包
	itemsNodeSet.push_back(oriNode);	//加入总item集合
	queue<items_Node>Q;					//加入循环队列
	Q.push(oriNode);
	while (!Q.empty()) {
		items_Node frontNode = Q.front();
		Q.pop();
		for (string X : VNT) {
			items_Node nextNode = itemsNodeGoto(frontNode, X);
			//存在转移
			if (!nextNode.items_Set.empty()) {
				nextNode = itemsNodeClosure(nextNode);		//求闭包
				//检测set中是否已经有该Node的记录
				bool checkNodeInSet = false;
				for (items_Node itNode : itemsNodeSet) {
					if (itemsNodeCheckSame(nextNode, itNode)) {
						nextNode = itNode;		//在 直接用已经存在的(id不同)
						checkNodeInSet = true;
					}
				}
				//不在set中 加入队列与set
				if (!checkNodeInSet) {
					itemsNodeSet.push_back(nextNode);
					Q.push(nextNode);
					//检测可能出现的规约 更新Action 加入Ri
					for (int i = 0;i < nextNode.originNum;i++) {
						//如果点出现在 末尾可以归约
						if (nextNode.items_Set[i].point + 1 >= nextNode.items_Set[i].ProCandidate.length()) {
							//拼出完整产生式 并查找其编号
							string production = nextNode.items_Set[i].Proleft + "->" + nextNode.items_Set[i].ProCandidate;
							auto it = std::find(Productions.begin(), Productions.end(), production);
							int proIndex = std::distance(Productions.begin(), it);

							if (proIndex == 0) {
								Action[nextNode.state_id].push_back(std::make_pair("#", "acc"));
							}
							else {
								//将 follow集合中所有vt 加入action
								for (string vt : Follow[nextNode.items_Set[i].Proleft]) {
									Action[nextNode.state_id].push_back(make_pair(vt, "R" + to_string(proIndex)));
								}
							}
						}
					}
					//空字只有可能是拓展来的	更新Action 加入Ri
					for (int i = nextNode.originNum;i < nextNode.items_Set.size();i++) {
						if (nextNode.items_Set[i].ProCandidate == "@") {
							item traversalItem = nextNode.items_Set[i];
							//拼出完整产生式 并查找其编号
							string production = traversalItem.Proleft + "->" + traversalItem.ProCandidate;
							auto it = std::find(Productions.begin(), Productions.end(), production);
							int proIndex = std::distance(Productions.begin(), it);

							for (string vt : Follow[traversalItem.Proleft]) {
								Action[nextNode.state_id].push_back(std::make_pair(vt, "R" + to_string(proIndex)));
							}
						}
					}
				}

				//若X是Vn 更新Goto
				if (grammar.Vn.find(X) != grammar.Vn.end()) {
					// 0 -> <E,1>
					pair<string, string>gotoPair(X, to_string(nextNode.state_id));
					Goto[frontNode.state_id].push_back(gotoPair);
				}
				//是Vt 更新Action 加入Si
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
	//printf("  状态|  +    *   (     )    i    # | E   T   F\n");
	printf("  状态|");
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
			//跳过非终结符 的输出
			if (grammar.Vn.find(VNT[i]) != grammar.Vn.end()) { continue; }
			bool checkChar = false;
			for (pair<string, string>& ACPair : Action[coutPos]) {
				//如果存在 (i,R2) && 未被访问
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
				//如果存在 (i,R2) && 未被访问
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
		//遍历对应状态
		for (pair<string, string> gotoTransPair : Action[topState]) {
			if (gotoTransPair.first == topInput) {
				nextActionState = gotoTransPair.second;
				break;
			}
		}
		if (nextActionState[0] == 'S') {
			//移进
			Symbol_Stack.push(topInput);
			State_Stack.push(atoi(nextActionState.substr(1).c_str()));		//子字符串转int 压入状态栈
			inputExpression = inputExpression.substr(1);
		}
		else if (nextActionState[0] == 'R') {
			//规约
			int productIndex = atoi(nextActionState.substr(1).c_str());
			string production = Productions[productIndex];
			string proLeft = getFullChar(production, 0);
			string proRight = production.substr(production.find(">") + 1);

			// 只需弹出对应数量即可 无序纠结次序与匹配
			// 空字不弹 跳过即可
			if (proRight != "@") {
				for (int i = 0;i < proRight.length();i++) {
					State_Stack.pop();
					Symbol_Stack.pop();
				}
			}

			//将产生式左部 压入符号栈，并拿到暴露出的状态
			Symbol_Stack.push(proLeft);
			topState = State_Stack.top();
			int nextGotoState = -1;
			for (pair<string, string>gotoTransPair : Goto[topState]) {
				if (gotoTransPair.first == proLeft) {
					nextGotoState = atoi(gotoTransPair.second.c_str());	//状态string转int
					break;
				}
			}
			if (nextGotoState == -1) {
				//不存在状态转移 错误 ?可能存在吗
				cout << "NO_GOTOTRANS";
				break;
			}
			else {
				State_Stack.push(nextGotoState);
			}
		}
		else if (nextActionState == "acc") {
			//结束
			cout << "YES" << endl;
			break;
		}
		else {
			//不存在状态转移 错误
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
	//printf("    状态栈    |    符号栈    |    输入串    |\t对应操作\n");
	while (true) {
		//栈内容的输出
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

		//分析部分
		int topState = State_Stack.top();
		string topInput = string(1, inputExpression[0]);

		string nextActionState = "";
		//遍历对应状态
		for (pair<string, string> gotoTransPair : Action[topState]) {
			if (gotoTransPair.first == topInput) {
				nextActionState = gotoTransPair.second;
				break;
			}
		}
		if (nextActionState[0] == 'S') {
			//移进
			Symbol_Stack.push(topInput);
			State_Stack.push(atoi(nextActionState.substr(1).c_str()));		//子字符串转int 压入状态栈
			inputExpression = inputExpression.substr(1);

			printf("移进 -> %-4s\n", nextActionState.c_str());
		}
		else if (nextActionState[0] == 'R') {
			//规约
			int productIndex = atoi(nextActionState.substr(1).c_str());
			//拿到用于归约的产生式 左部和右部
			string production = Productions[productIndex];
			string proLeft = getFullChar(production, 0);
			string proRight = production.substr(production.find(">") + 1);

			// 只需弹出对应数量即可 无序纠结次序与匹配
			// 空字不弹 跳过即可
			if (proRight != "@") {
				for (int i = 0;i < proRight.length();i++) {
					State_Stack.pop();
					Symbol_Stack.pop();
				}
			}

			printf("归约 -> %-4s :%-7s\n", nextActionState.c_str(), production.c_str());

			//将产生式左部 压入符号栈，并拿到暴露出的状态
			Symbol_Stack.push(proLeft);
			topState = State_Stack.top();
			int nextGotoState = -1;
			for (pair<string, string>gotoTransPair : Goto[topState]) {
				if (gotoTransPair.first == proLeft) {
					nextGotoState = atoi(gotoTransPair.second.c_str());	//状态string转int
					break;
				}
			}
			if (nextGotoState == -1) {
				//不存在状态转移 错误 ?可能存在吗
				cout << "WA" << endl;
				cout << "NO_GOTOTRANS";
				break;
			}
			else {
				State_Stack.push(nextGotoState);
			}
		}
		else if (nextActionState == "acc") {
			//结束
			cout << "ACC" << endl;
			cout << "YES" << endl;
			break;
		}
		else {
			//不存在状态转移 错误
			cout << "WA" << endl;
			cout << "NO_ACTIONTRANS" << endl;
			break;
		}
	}
}

void SLRClass::run_ReadExp_SLR() {
	FILE* fp = fopen(readSLRExpressionFile, "r");
	if (fp == NULL) {
		//cout << "SLR_表达式文件不存在" << endl;
		cout << "read SLR Expression File Not Exist \n" << endl;
	}
	else {
		while (fgets(expLineToken, 50, fp) != NULL) {
			cout << endl << string(expLineToken);
			//SLR_predict(ExpChange(expLineToken));					//SLR分析结果
			SLR_predict_AnalyseStack(ExpChange(expLineToken));	//SLR分析结果+分析栈
		}
		fclose(fp);
	}
}
