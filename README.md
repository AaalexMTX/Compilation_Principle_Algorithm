# LL(1)_TopToDown
*完成对于初始文法的LL(1)文法转换 包括左递归消除 ＋ 提公因式*</br>
---
例如**左递归的消除**
>A->ab|ac|f      A->aA'|eA''|f
>A->ef|eh;  ---> A'->b|c
>                A"->f|h;

