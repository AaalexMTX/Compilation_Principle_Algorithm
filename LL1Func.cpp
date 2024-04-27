#include"LL1_info.h"
#include"SLR_info.h"
#include"wordAnalyse_info.h"

/*  1.分析栈状态   2.其他文法的适配性检测
*
 L->E
E->E+T|T;
T->T*F|F;
F->(E)|i;

test data
10;
1+2;
(1+2)*3+(5+6*7);
((1+2)*3+4;
1+2+3+(*4+5);
(a+b)*(c+d);
((ab3+de4)**5)+1;

1 + 1 + ab * 2
12 + 18
12 * 21
12 * abc + 1
(12+1)
(1)
((1*2)
(1)*(3)
(1++float)
(1*1+int)

test grammar
i(+i)*n (*i|@)

L->S
S->AB;
A->i|A+i
B->*i|@

9
12+1
13+ a+b
21 + 33 * abc
12 * a
*12

*/

extern grammerStruct grammer;
//int main() {
//	readGrammar();
//
//	calculate_NullAble();
//	calculate_First();
//	calculate_Follow();
//
//	/*item originItem(grammer.S, grammer.P[grammer.S][0], -1);
//	vector<item>originItemVector(1, { grammer.S, grammer.P[grammer.S][0], -1 });*/
//
//	//匿名定义
//	items_Node originNode(vector<item>(1, { grammer.S, grammer.P[grammer.S][0], -1 }));
//	SLRAnaTableConstruct(originNode);
//
//	printSLRTabel();
//
//	ReadExp_SLR();
//	return 0;
//}
