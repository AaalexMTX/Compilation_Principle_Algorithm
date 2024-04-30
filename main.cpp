#include"./include/LL1_info.h"
#include"./include/SLR_info.h"
#include"./include/RPN_info.h"
#include"./include/Common_data.h"
#include"./include/wordAnalyse_info.h"
using namespace std;

void menu();
//避免执行额外的初始化操作
int initRecord[10] = {};
void initLL1Class(const int choice, LL1Class* LL1) {
	//上次执行过初始化化  跳过
	if (initRecord[choice]) {
		return;
	}
	//更新 初始化记录数组
	memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	//LL1文件中文法读入
	if (!LL1->readLL1Grammar()) {
		return;
	};

	//消除左递归 和 前缀
	LL1->remove_left_recursion();
	LL1->remove_left_gene();

	// 将标准文法写入文件
	// 估计是改了 stdout的输出位置没改回 / 文件没关闭，导致后面输出报错
	LL1->formatPrintLL1IntoFile();

	// 计算构造LL1的 空集、first集、follow集
	LL1->calculate_NullAble();
	LL1->calculate_First();
	LL1->calculate_Follow();

	// 构造LL1 分析表
	LL1->construct_LL1Table();
}

void initSLRClass(int choice, SLRClass* SLR) {
	if (initRecord[choice]) {
		return;
	}
	memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	// SLR文件中读入文法
	SLR->readSLRGrammar();

	// 构造相应的SLR的 空集、first集、follow集
	SLR->calculate_NullAble();
	SLR->calculate_First();
	SLR->calculate_Follow();

	/*item originItem(grammar.S, grammar.P[grammar.S][0], -1);
	vector<item>originItemVector(1, { grammar.S, grammar.P[grammar.S][0], -1 });*/

	//匿名定义
	items_Node originNode(vector<item>(1, { SLR->grammar.S, SLR->grammar.P[SLR->grammar.S][0], -1 }));
	SLR->SLRAnaTableConstruct(originNode);

	SLR->printSLRTabel();
};


void initRPNClass(int choice, RPNClass* RPN) {
	if (initRecord[choice]) {
		return;
	}
	memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	// 文件中读入文法
	RPN->readRPNGrammar();

	// 构造相应的的 空集、first集、follow集
	RPN->calculate_NullAble();
	RPN->calculate_First();
	RPN->calculate_Follow();

	/*item originItem(grammar.S, grammar.P[grammar.S][0], -1);
	vector<item>originItemVector(1, { grammar.S, grammar.P[grammar.S][0], -1 });*/

	//匿名定义
	items_Node originNode(vector<item>(1, { RPN->grammar.S, RPN->grammar.P[RPN->grammar.S][0], -1 }));
	RPN->SLRAnaTableConstruct(originNode);

	RPN->printSLRTabel();
};

int main() {
	int choice = 0;
	LL1Class LL1 = LL1Class();
	SLRClass SLR = SLRClass();
	RPNClass RPN = RPNClass();
	do {
		menu();
		cin >> choice;
		switch (choice)
		{
		case 1: {
			//initLL1(choice);
			//根据内存中数据结构分析记录的文法和分析表，对文件中的表达式进行LL1分析
			//run_ReadExp_LL1();
			break;
		}
		case 2: {
			//initSLR(choice);
			// 进行SLR分析
			//run_ReadExp_SLR();
			break;
		}
		case 3: {
			printf("RPL\n");
			break;
		}
		case 4: {
			initLL1Class(choice, &LL1);
			LL1.run_ReadExp_LL1();
			break;
		}
		case 5: {
			initSLRClass(choice, &SLR);
			SLR.run_ReadExp_SLR();
			break;
		}
		case 6: {
			initRPNClass(choice, &RPN);
			RPN.runn_ReadExp_RPN();
			break;
		}
		default:
			break;
		}
		cout << "\n\n";
	} while (choice != 0);
	return 0;
}

void menu() {
	cout << "|------编译原理分析系统-------|" << endl;
	cout << "0. exit " << endl;
	cout << "1. 词法分析" << endl;
	cout << "2. 文法读入" << endl;
	cout << "3. LL1文法改造 " << endl;
	cout << "4. LL1分析类版 " << endl;
	cout << "5. SLR类版 " << endl;
	cout << "6. 逆波兰分析" << endl;
	cout << "|------------------------|" << endl;
	cout << "请输入选项:";
}