#include"LL1_info.h"

//ȫ�ֱ���
const int MAXLEN = 20;
char lineToken[MAXLEN] = {};
extern const char* readFileName;
extern const char* writeFileName;
extern int VNnumber;

//test
int main() {
	FILE* fp = fopen(readFileName, "r");
	//���ж� �ļ����쳣����ֹ����
	if (fp == NULL) {
		cout << "NO file";
	}
	else {
		while (fgets(lineToken, 20, fp) != NULL) {
			scan(lineToken);
		}
	}
	remove_left_recursion();
	remove_left_gene();

	print();
	if (freopen(writeFileName, "w", stdout) != NULL) {
		print();
	}
	fclose(stdout);
	return 0;
}
