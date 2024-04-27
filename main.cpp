#include"./include/LL1_info.h"
#include"./include/SLR_info.h"
#include"./include/wordAnalyse_info.h"
using namespace std;

extern grammerStruct grammer;
void menu();
void initLL1();
void initSLR();

int main() {
	int choice = 0;
	//����ִ�ж���ĳ�ʼ������
	int initRecord[10] = {};
	do {
		menu();
		cin >> choice;
		switch (choice)
		{
		case 1: {
			if (!initRecord[choice]) {
				initLL1();
				//���� ��ʼ����¼����
				memset(initRecord, 0, sizeof(initRecord));
				initRecord[choice] = 1;
			}
			//�����ڴ������ݽṹ������¼���ķ��ͷ��������ļ��еı��ʽ����LL1����
			run_ReadExp_LL1();
			break;
		}
		case 2: {
			if (!initRecord[choice]) {
				initSLR();
				memset(initRecord, 0, sizeof(initRecord));
				initRecord[choice] = 1;
			}
			// ����SLR����
			run_ReadExp_SLR();
		}
		case 3: {
			printf("RPL\n");
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
	cout << "|------����ԭ�����ϵͳ-------|" << endl;
	cout << "0. exit " << endl;
	cout << "1. LL(1)����" << endl;
	cout << "2. SLR����" << endl;
	cout << "3. �沨��ʽ���� " << endl;
	cout << "4. -- " << endl;
	cout << "5. -- " << endl;
	cout << "6. -- " << endl;
	cout << "|------------------------|" << endl;
	cout << "������ѡ��:";
}

void initLL1() {
	//LL1�ļ����ķ�����
	readLL1Grammar();

	//������ݹ� �� ǰ׺
	remove_left_recursion();
	remove_left_gene();

	// ����׼�ķ�д���ļ�
	// �����Ǹ��� stdout�����λ��û�Ļ� / �ļ�û�رգ����º����������
	formatPrintLL1IntoFile();

	// ���㹹��LL1�� �ռ���first����follow��
	calculate_NullAble();
	calculate_First();
	calculate_Follow();

	// ����LL1 ������
	construct_LL1Table();
}

void initSLR() {
	// SLR�ļ��ж����ķ�
	readGrammar();

	// ������Ӧ��SLR�� �ռ���first����follow��
	calculate_NullAble();
	calculate_First();
	calculate_Follow();

	/*item originItem(grammer.S, grammer.P[grammer.S][0], -1);
	vector<item>originItemVector(1, { grammer.S, grammer.P[grammer.S][0], -1 });*/

	//��������
	items_Node originNode(vector<item>(1, { grammer.S, grammer.P[grammer.S][0], -1 }));
	SLRAnaTableConstruct(originNode);

	printSLRTabel();
};