
### v1.0
## LL(1)
1. 文法 和 表达式文件还是不能加staic
    - 用类的本质目的是模版生成，对相似的数据对象（不同文法）有相同的分析处理方式...同理也别加const(定义的时候就得赋值还不能改)
    - static就固定了文件和数据，没有用类的必要了
2. 公用数据结构复写一遍
    - 提取放在一个公用文件中（tools）

```cpp
// 全局数据结构都是基本直接暴露的全局变量
// 记录使用的（方法+全局变量）--指导类的定义
//读入LL1文法
void readLL1Grammar();           
    string readLL1GrammarFile;      //文法文件路径
    char lineToken[]                //记录文法结构(记录一行)
    void scan[lineToken]            //分析该产生式
        regex N                     //正则匹配 终结符
        regex T                     //用于正则匹配 非终结符
        grammerStruct grammar       //LL1文法记录
        vector<string>Productions;  //顺序放产生式（用map是乱序存的 输出分析表会很乱）
        void getBC()                //获取下一个非空位置
//消除 产生式的直接左递归
void directLeftRecursion(string proLeft, vector<string>proRight);
    grammerStruct grammar
//消除左递归
remove_left_recursion();
    grammerStruct grammar
    void directLeftRecursion(string proLeft, vector<string>proRight)        //消除直接左递归
        grammerStruct grammar       //直接操作文法结构
//提公因式
void remove_left_gene()
    grammerStruct grammar           //直接操作文法结构
//格式化 输出文法到文件
void formatPrintLL1IntoFile()
    string writeLL1TransResultFile  //写入文件名（可以设成static）
    void formatPrint()
        grammerStruct grammar
//计算空集
void calculate_NullAble()
    grammerStruct grammar
    unordered_set<string>NullAble   //读文法 计算写入空集数据结构
    string getFullChar(string candidate, int pos)       //计算完整候选式用
//计算first集合
void calculate_First()
    grammerStruct grammar
    unordered_map<string, unordered_set<string>>Follow  //读文法 计算写入   First集数据结构
    string getFullChar(string candidate, int pos)
//计算Follow集合
void calculate_Follow()
    grammerStruct grammar
    NullAble
    unordered_map<string, unordered_set<string>>Follow  //读文法 计算写入   Follow集数据结构
//构建LL1分析表
void construct_LL1Table()
    grammerStruct grammar
    NullAble
    First
    Follow
    unordered_map<string, vector<pair<string, string>>>LL1_table; //文法来构建LL1分析表
    string getFullChar(string candidate, int pos)
//开启LL1分析
void run_ReadExp_LL1()
    string readExpressionFile           //要分析的表达式文件（static预定）
    char expLineToken[EXPMAXLEN] = {};  //记录表达式（类似lineToken[] （公用）
    string ExpChange(char line[])       //预处理文件里的表达式（char[] -> string,去空白符等）
    bool LL1_predict(string inputExpression)    //LL1预测
        stack<string>analyseStack;      //分析栈（私有）
        grammerStruct grammar
        ... LL1_table                   //综合了First Follow Nullable信息
        getFullChar()
```

## SLR
```cpp
// SLR文件中读入文法
readSLRGrammar();
    string readSLRGrammarFile
    lineToken
    void scan[lineToken]            //分析该产生式
        regex N                     //正则匹配 终结符
        regex T                     //用于正则匹配 非终结符
        grammerStruct grammar       //文法记录
        vector<string>Productions;  //顺序放产生式（用map是乱序存的 输出分析表会很乱）
        void getBC()                //获取下一个非空位置
// 构造相应的SLR的 空集、first集、follow集
calculate_NullAble();
calculate_First();
calculate_Follow();

//匿名定义初始化节点(对定义的items_Node节点 构造分析表)
//构造分析表 （核心）
SLRAnaTableConstruct(originNode);
    vector<string>VNT;				//取出文法中的Vn和Vt便于遍历
    vector<items_Node>itemsNodeSet(0);  //所有项目集
    items_Node                      //项目集节点类型
    vector<string> Productions;     //顺序记录的产生式

    Follow!!!!!                    //用到了follow集合
    unordered_map<int, vector<pair<string, string>>>Action;		//Action表
unordered_map<int, vector<pair<string, string>>>Goto;		//Goto表
    constructVTN();
        regex N                     //正则匹配 终结符
        regex T                     //用于正则匹配 非终结符
        Productions; 
    items_Node itemsNodeClosure(const items_Node& oriNode); //对初始Node 求闭包
        grammar;                    //访问自身文法结构
        items_Node;                 //项目集节点信息
        getFullChar()               //获取完整候选式
    items_Node itemsNodeGoto(const items_Node& oriNode, string X)   //对初始项目集 接收x的goto项目集
        items_Node;                 //项目集节点信息   
    bool itemsNodeCheckSame(const items_Node& oriNode, const items_Node& nextNode)  //检测两个字符集是否相同 
//输出分析表  (预测的数据 就存在Action和Goto里了)
// items_Node和grammar只是工具
printSLRTabel();
    grammar;
    VNT[];
    Action();
    Goto()
// 进行SLR分析
run_ReadExp_SLR();
    string readExpressionFile;      //放公用文件中
    ExpChange(expLineToken);        //公用 处理文件用表达式用
    SLR_predict_AnalyseStack();	    //SLR分析结果+分析栈
        stack<int>State_Stack;		//状态栈
        stack<string>Symbol_Stack;	//符号栈
        Action();
        Goto();
```