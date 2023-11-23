#include"LL1_info.h"

int main() {
	readGrammar();

	remove_left_recursion();
	remove_left_gene();	

	calculate_nullAble();
	calculate_first();
	calculate_follow();
	construct_LL1Table();

	cout << endl;
	if (LL1_predict("(i+i*)")) {
		cout << "YES";
	}
	else { cout << "NO"; }
	cout << endl;

	formatPrint();
	
	return 0;
}
