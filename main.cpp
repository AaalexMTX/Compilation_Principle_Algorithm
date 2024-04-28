#include"./include/LL1_info.h"
#include"./include/SLR_info.h"
#include"./include/wordAnalyse_info.h"
using namespace std;

extern grammerStruct grammar;
void menu();
void initLL1(int);
void initSLR(int);
void initLL1Class(const int choice, LL1Class* LL1);
//����ִ�ж���ĳ�ʼ������
int initRecord[10] = {};

int main() {
	int choice = 0;
	do {
		menu();
		cin >> choice;
		switch (choice)
		{
		case 1: {
			initLL1(choice);
			//�����ڴ������ݽṹ������¼���ķ��ͷ��������ļ��еı��ʽ����LL1����
			run_ReadExp_LL1();
			break;
		}
		case 2: {
			initSLR(choice);
			// ����SLR����
			run_ReadExp_SLR();
			break;
		}
		case 3: {
			printf("RPL\n");
			break;
		}
		case 4: {
			LL1Class LL1 = LL1Class();
			initLL1Class(choice, &LL1);
			LL1.run_ReadExp_LL1();
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
	cout << "4. LL1������� " << endl;
	cout << "5. SLR��� " << endl;
	cout << "6. -- " << endl;
	cout << "|------------------------|" << endl;
	cout << "������ѡ��:";
}

void initLL1(int choice) {
	//�ϴ�ִ�й���ʼ����  ����
	if (initRecord[choice]) {
		return;
	}
	//���� ��ʼ����¼����
	memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	//LL1�ļ����ķ�����
	if (!readLL1Grammar()) {
		return;
	};

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

void initSLR(int choice) {
	if (initRecord[choice]) {
		return;
	}
	memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	// SLR�ļ��ж����ķ�
	readSLRGrammar();

	// ������Ӧ��SLR�� �ռ���first����follow��
	calculate_NullAble();
	calculate_First();
	calculate_Follow();

	/*item originItem(grammar.S, grammar.P[grammar.S][0], -1);
	vector<item>originItemVector(1, { grammar.S, grammar.P[grammar.S][0], -1 });*/

	//��������
	items_Node originNode(vector<item>(1, { grammar.S, grammar.P[grammar.S][0], -1 }));
	SLRAnaTableConstruct(originNode);

	printSLRTabel();
};

void initLL1Class(const int choice, LL1Class *LL1) {
	//�ϴ�ִ�й���ʼ����  ����
	if (initRecord[choice]) {
		return;
	}
	//���� ��ʼ����¼����
	memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	//LL1�ļ����ķ�����
	if (!LL1->readLL1Grammar()) {
		return;
	};

	//������ݹ� �� ǰ׺
	LL1->remove_left_recursion();
	LL1->remove_left_gene();

	// ����׼�ķ�д���ļ�
	// �����Ǹ��� stdout�����λ��û�Ļ� / �ļ�û�رգ����º����������
	LL1->formatPrintLL1IntoFile();

	// ���㹹��LL1�� �ռ���first����follow��
	LL1->calculate_NullAble();
	LL1->calculate_First();
	LL1->calculate_Follow();

	// ����LL1 ������
	LL1->construct_LL1Table();
}