
### v1.0
LL(1)
1. 文法 和 表达式文件还是不能加staic
    - 用类的本质目的是模版生成，对相似的数据对象（不同文法）有相同的分析处理方式...同理也别加const(定义的时候就得赋值还不能改)
    - static就固定了文件和数据，没有用类的必要了

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
        getFullChar
```