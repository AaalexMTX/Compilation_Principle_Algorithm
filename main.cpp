#include"LL1_info.h"
#include"SLR_info.h"
#include"wordAnalyse_info.h"
using namespace std;

extern grammerStruct grammer;
void menu();

int main() {
	int choice = 0;

	do {
		menu();
		cin >> choice;
		switch (choice)
		{
		case 1: {
			readLL1Grammar();

			remove_left_recursion();
			remove_left_gene();

			// 估计是改了 stdout的输出位置没改回 / 文件没关闭，导致后面输出报错
			//formatPrintLL1IntoFile();

			calculate_NullAble();
			calculate_First();
			calculate_Follow();

		
			construct_LL1Table();
			run_ReadExp_LL1();

			break;
		}
		case 2: {
			cout << "SLR " << endl;

			readGrammar();

			calculate_NullAble();
			calculate_First();
			calculate_Follow();

			/*item originItem(grammer.S, grammer.P[grammer.S][0], -1);
			vector<item>originItemVector(1, { grammer.S, grammer.P[grammer.S][0], -1 });*/

			//匿名定义
			items_Node originNode(vector<item>(1, { grammer.S, grammer.P[grammer.S][0], -1 }));
			SLRAnaTableConstruct(originNode);

			printSLRTabel();

			run_ReadExp_SLR();
		}
		case 3:

		default:
			break;
		}
		cout << "\n\n";
	} while (choice != 0);
	return 0;
}

void menu() {
	cout << "|------学生管理系统-------|" << endl;
	cout << "0. exit " << endl;
	cout << "1. LL(1)分析" << endl;
	cout << "2. SLR分析" << endl;
	cout << "3. 逆波兰式分析 " << endl;
	cout << "4. -- " << endl;
	cout << "5. -- " << endl;
	cout << "6. -- " << endl;
	cout << "|------------------------|" << endl;
	cout << "请输入选项:";
}