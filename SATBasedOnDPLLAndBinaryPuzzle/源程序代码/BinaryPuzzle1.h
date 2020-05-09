#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
//宏定义区域
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define OVERFLOW -2
#define OK 2
#define ADDR ".\\testFile\\"

typedef int Status;
typedef struct word;
typedef struct childSentence;
typedef struct cnfFile;

//用于统计cnf中文字总数的结构
typedef struct count {
	int posi;		//储存当前结构中存在的正的文字的个数
	int nega;		//当前文字负的个数
}count,*pCount;
//count的头结点
typedef struct headCount {
	int max_variable;		//当前结构中个数最多的文字变量
	int sum;					//文字最多的文字变量的总数
	bool containPo;			//标记当前数量最多的文字是否存在正数
	bool cintainNe;			//标记当前结构中是否存在负数
	pCount count;			//指向统计数组
}headCount,*pHeadCount;
//总 结点cnf文件，基本信息
typedef struct cnfFile {
	int numOfVariable;//涉及到的文字的个数
	int numOfchildSentence;
	struct childSentence* sentence;
}cnfFile, *pCnfFile;

//子句结点
typedef struct childSentence {
	struct childSentence * nextSen;//指向另一个子句
	struct word* words;				//指向文字，当此项为空时，说明为空子句
	int numOfword;					//该子句中文字的个数
	int mark;							//标记该节点是否被删除，当mark为0时，表示该节点存在，否则该节点的值等于分裂规则或者单子句规则中导致该字句被删除的文字
	bool isCunzai;						//仅用于生成随机数独地图时，标记当前空格是否被删除掉
}childSentence, *pChildSentence;

//文字结点
typedef struct word {
	int varibale;					//文字变元
	bool mark;					//标记，用于标记该节点是被删除
	struct word *nextword;	//单链表指向下一个文字
}word, *pWord;

//链栈，用于非递归实现DPLL回溯
typedef struct stack {
	struct stackNode *top;
	struct stackNode *tail;
}*pstack;

typedef struct stackNode {
	int setTrueWord;			//在单子句规则选中的单子句文字，或者在分裂规则中选中的假定为真的文字
	bool methodMark;					//标记该次化简使用所使用的规则，false--单子句规则，使用回溯时直接恢复即可，true--分裂规则使用回溯时往下深入
	stackNode* nextNode;
}stackNode, *pStackNode;
//单子句规则查找字句的结构,数组下标作为变量
//初始化栈
void initStack(pstack &sta);
//入栈
void addStack(pstack sta, int word, bool methodMark);
//出栈
Status outStack(pstack sta, int &word, bool &methodMark);
//检测空栈
bool cheakStack(pstack sta);
//根据栈元素进行回溯
Status recovery(pstack sta, pCnfFile pcnf);
//化简规则，单子句中的化简/分裂规则中的化简
Status simplyS(pCnfFile pcnf, int word);
//检测子句中是否含有目标文字
bool checkWord(pChildSentence pcs, int word);
//选择分裂规则中使用的文字，选择结构中同时出现原文字、非文字的文字
int seclect(pCnfFile pcnf, int *count, bool &mark);
//寻找单子句，成功找到则返回单子句的文字，否则返回0
int findSingal(pCnfFile pcnf);
//检测S中是否存在空子句
bool checkNull(pCnfFile pcnf);
//DPLL算法主体
Status DPLL(pCnfFile pcnf, pstack &stack,int beginTime,bool change);
//文件读取函数
Status readFile(pCnfFile pcnf,char *pFilename);
//读文件创建结构
Status readAndCreate(pCnfFile pcnf, FILE*file);
//销毁栈并递归输出路径
void destroyStack(pstack sta, bool isSuccess, int * answer, int &index);

void destroy(pStackNode psn, pstack sta, int &i, bool isSuccess, int * answer, int &index);
//销毁当前的cnf结构
void destroyCNF(pCnfFile pcnf);
//销毁子句
void destroyChildScen(pChildSentence pcs);
//C（n,2）组合数
bool* printCn2(int n, int &len);
//生成1~n中的n/2+1的所有组合数，用数组保存
bool* generateCombination(int n, int &len);

int countNumOfTrue(bool bol[], int n);
//生成00000000~11111111中的所有数按位形成的bool数组
bool* generateZuhe(int n, int &len);
//(1)限制1，同行、同列不能连续三个相同
void printCondition1(FILE * file, int n);
//(2)限制2，同一行或同一列0，1个数相同，任意n/2 +1个不相同
void printCondition2(FILE*file, int n);
//（3）限制3，任意两行不相同
void printCondition3(FILE*file, int n);

void printConfition(void);

void addNodeToPcnf(pCnfFile pcnf, int word);
//完成求解后，重置pcnf的mask
void resetPcnf(pCnfFile pcnf);
//生成数读地图
void generateSoDuKuMap(pCnfFile pcnf, int n);
//写入文件
Status writeSoDoKufile(pCnfFile pcnf);

void printMap(int *digHole, int *answer, int n);
//挖洞生成数独地图,传入生成的空的个数,n表示阶数
void digMap(pCnfFile pcnf, int *digHole, int numOfHole, int n);
//将编号为num的位置设为其相反数，查看是否有解,n为阶数
Status changeAndDPLL(pCnfFile pcnf, int num, int n);

pChildSentence findNum(pCnfFile pcnf, int num, bool &bol);
//在count结构中添加变量variable
void addToCount(pHeadCount phc, int variable);
//从count结构中删除variable
void delFormCount(pHeadCount phc, int variable, pCnfFile pcnf);
//文件读取函数改进--添加统计count的操作入口
Status readFile_1(pCnfFile pcnf, pHeadCount &phc,char * pFilename);
//读文件创建结构
Status readAndCreate_1(pCnfFile pcnf, FILE*file, pHeadCount &phc);
//选取变元改写
int select_1(pCnfFile pcnf, pHeadCount phc, bool &mark);
//化简单子句改写
Status simplyS_1(pCnfFile pcnf, int word, pHeadCount phc);
//DPLL改写
Status DPLL_1(pCnfFile pcnf, pstack &stack, pHeadCount phc);
//recovery函数改写
//根据栈元素进行回溯
Status recovery_1(pstack sta, pCnfFile pcnf, pHeadCount phc);

void resetCount(pCnfFile pcnf, pHeadCount phc);

Status writeRes(char* fileName, bool isSuccess, int sumTime, int* answer, int n);