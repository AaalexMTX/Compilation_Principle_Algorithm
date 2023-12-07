#include"LL1_info.h"
#include"wordAnalyse_info.h"

int main() {
	readGrammar();

	remove_left_recursion();
	remove_left_gene();	

	calculate_nullAble();
	calculate_first();
	calculate_follow();
	construct_LL1Table();

	ReadExpression();

	//formatPrint();
	
	//formatPrintIntoFile();
	return 0;
}
