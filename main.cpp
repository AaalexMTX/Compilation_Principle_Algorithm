#include"./include/LL1_info.h"
#include"./include/SLR_info.h"
#include"./include/RPN_info.h"
#include"./include/Common_data.h"
#include"./include/wordAnalyse_info.h"
using namespace std;

//����ִ�ж���ĳ�ʼ������
int initRecord[10] = {};
void menu();
void initLL1Class(const int&, LL1Class*);
void initSLRClass(const int&, SLRClass*);
void initRPNClass(const int&, RPNClass*);
void pretreatLL1Class(const int&, LL1Class*);

int main() {
	int choice = 0;
	Grammar_set grammar = Grammar_set();
	LL1Class LL1 = LL1Class();
	LL1Class LL1pre = LL1Class();
	SLRClass SLR = SLRClass();
	RPNClass RPN = RPNClass();
	do {
		menu();
		cin >> choice;
		switch (choice)
		{
		case 1: {
			auto _ = readWordFile(readExpressionFile);
			break;
		}
		case 2: {
			// ��һ���ķ�
			if (!initRecord[choice]) {
				grammar.readGrammar(grammar.readGrammarFile);
				//���� ��ʼ����¼����
				initRecord[choice] = 1;
			}
			grammar.formatPrint();
			break;
		}
		case 3: {
			pretreatLL1Class(choice, &LL1pre);
			LL1pre.formatPrint();
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
			SLR.printSLRTabel();

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
	cout << "|------����ԭ�����ϵͳ-------|" << endl;
	cout << "0. exit " << endl;
	cout << "1. �ʷ�����" << endl;
	cout << "2. �ķ�����" << endl;
	cout << "3. LL1�ķ����� " << endl;
	cout << "4. LL1������� " << endl;
	cout << "5. SLR��� " << endl;
	cout << "6. �沨������" << endl;
	cout << "|------------------------|" << endl;
	cout << "������ѡ��:";
}

//��ʼ��LL1Ԥ�������������ݹ�+�ṫ��ʽ��
void pretreatLL1Class(const int& choice, LL1Class* LL1pre) {
	//�ϴ�ִ�й���ʼ����  ����
	if (initRecord[choice]) {
		return;
	}
	//���� ��ʼ����¼����
	initRecord[choice] = 1;

	//LL1�ļ����ķ�����
	if (!LL1pre->readGrammar(LL1pre->readtransLL1GrammarFile)) {
		return;
	}

	//������ݹ� �� ǰ׺
	LL1pre->remove_left_recursion();
	LL1pre->remove_left_gene();

	// ����׼�ķ�д���ļ�
	LL1pre->formatPrintLL1IntoFile(LL1pre->writeLL1TransResultFile);
}

//��ʼ��LL1����
void initLL1Class(const int& choice, LL1Class* LL1) {
	//�ϴ�ִ�й���ʼ����  ����
	if (initRecord[choice]) {
		return;
	}
	//���� ��ʼ����¼����
	//memset(initRecord, 0, sizeof(initRecord));
	initRecord[choice] = 1;

	//LL1�ļ����ķ�����
	if (!LL1->readGrammar(LL1->readLL1GrammarFile)) {
		return;
	}

	//������ݹ� �� ǰ׺
	LL1->remove_left_recursion();
	LL1->remove_left_gene();

	// ����׼�ķ�д���ļ�
	//LL1->formatPrintLL1IntoFile();

	// ���㹹��LL1�� �ռ���first����follow��
	LL1->calculate_NullAble();
	LL1->calculate_First();
	LL1->calculate_Follow();

	// ����LL1 ������
	LL1->construct_LL1Table();
}

//��ʼ��SLR����
void initSLRClass(const int& choice, SLRClass* SLR) {
	if (initRecord[choice]) {
		return;
	}

	initRecord[choice] = 1;

	// SLR�ļ��ж����ķ�
	if (!SLR->readGrammar(SLR->readSLRGrammarFile)) {
		return;
	}

	// ������Ӧ��SLR�� �ռ���first����follow��
	SLR->calculate_NullAble();
	SLR->calculate_First();
	SLR->calculate_Follow();

	/*item originItem(grammar.S, grammar.P[grammar.S][0], -1);
	vector<item>originItemVector(1, { grammar.S, grammar.P[grammar.S][0], -1 });*/

	//��������
	items_Node originNode(vector<item>(1, { SLR->grammar.S, SLR->grammar.P[SLR->grammar.S][0], -1 }));
	SLR->SLRAnaTableConstruct(originNode);

};

//��ʼ��RPN����
void initRPNClass(const int& choice, RPNClass* RPN) {
	if (initRecord[choice]) {
		return;
	}

	initRecord[choice] = 1;

	// �ļ��ж����ķ�
	if (!RPN->readGrammar(RPN->readRPNGrammarFile)) {
		return;
	}

	// ������Ӧ�ĵ� �ռ���first����follow��
	RPN->calculate_NullAble();
	RPN->calculate_First();
	RPN->calculate_Follow();

	/*item originItem(grammar.S, grammar.P[grammar.S][0], -1);
	vector<item>originItemVector(1, { grammar.S, grammar.P[grammar.S][0], -1 });*/

	//��������
	items_Node originNode(vector<item>(1, { RPN->grammar.S, RPN->grammar.P[RPN->grammar.S][0], -1 }));
	RPN->SLRAnaTableConstruct(originNode);

	RPN->printSLRTabel();
};