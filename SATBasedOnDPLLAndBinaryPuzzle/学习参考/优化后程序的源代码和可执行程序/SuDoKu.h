#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#pragma warning(disable:4996)

#define FALSE -1
#define OK 1
#define OVERFLOW -2
#define N 600

typedef int status;
int variable , sentences ;//记录变元数和公式数
int FLAG;//选择策略
int mark_ture[N];//保存正确的解
int mark_1[N];//存储变元出现的次数
int mark_2[N];//存储变元的下标
int a[10][10][10];//三维数组
int counnt ;//记录挖洞数
int hole[9][9];//记录挖过的洞
int shuzu[9][9];//记录棋盘

//邻接表一之存储
typedef struct EdgeNode {//小边集合
	int num;
	int mark;
	struct EdgeNode *next;
}EdgeNode, *edgenode;
typedef struct HeadNode {//大边集合
	int count;
	int mark;
	int flag;
	struct HeadNode *next;
	struct EdgeNode *inside;
}HeadNode, *headnode;
//邻接表二之索引
typedef struct SerchNode {//用于直接删除，不需要遍历
	int zheng_mark;//标记正变元
	int fu_mark;//标记负变元
	struct EdgeNode *inside_zheng;//指向正变元
	struct HeadNode *outside_zheng;
	struct EdgeNode *inside_fu;//指向负变元
	struct HeadNode *outside_fu;
	struct  SerchNode*next;
}SerchNode, SerchList[N+1];
typedef struct {//用于删除合适的假设真值
	SerchList serchlist;
	int mark;
} BigNode, *bignode;

//                    cnf文件读取板块优化后                       

//读取cnf文件
status ReadFile(headnode *Head, bignode G,char filename[60]);
//保存cnf文件
status Save_File(headnode Head);
//存cnf文件小前提
void check(headnode Head);
//输出cnf文件
void Traverse(headnode Head);
//清空链表
void Realse_Link(headnode * Head);

//                      cnf文件处理板块优化后                    

//标记函数，确定假设的真值情况
void Strategy(headnode Head, int mark_1[N], int mark_2[N]);
//删除单子句
status DeleteSingle(bignode G, int flag);
//恢复单子句
status Recovery(bignode G, int flag);
//判断是否为空集
status JudgeGroup(HeadNode *Head);
//寻找空子句
status FindEmptyClause(HeadNode *Head);
//寻找假设的真值
status FindCount(HeadNode *Head);
//Dpll算法的主函数
status Dpll_youhua(HeadNode *Head, int now_count, bignode G);

//                    数独函数部分                        

//输出数独棋盘
void Print_SuDoKu(int arr[][9]);
//匹配棋盘
void ShuChu_match(int shuzu[][9]);
//调用读取函数
status Read_SuDoKu(headnode *Head, bignode  G, int mid);
//读入邻接表之初始化邻接表操作
status Isinit_SuDoKu(headnode *Head, bignode G);
//数独之判断填入的数字是否正确
status SuDoKu_Judge(int arr[][9], int row, int col, int n);
//匹配函数
void Match(int arr[][9], int a[][10][10]);
//每空取值组合 
void  Create_Cnf_Position(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每行取值组合
void  Create_Cnf_Row(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每列取值组合
void Create_Cnf_Col(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每小方块取值组合
void Create_Cnf_Block(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每空取值保证
void Insure_Cnf_Position(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每行取值保证
void  Insure_Cnf_Row(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每列取值保证
void  Insure_Cnf_Col(int arr[][9], int a[][10][10], headnode *H, bignode G);
//每小方块取值保证
void  Insure_Cnf_Block(int arr[][9], int a[][10][10], headnode *H, bignode G);
//数独转换成函数
void Tranlate(int arr[][9], headnode *H, bignode  F);
//调用数独的函数
status  Create_sudoku(headnode *H, bignode  F, int arr[9][9]);


//                                挖洞法                           

//调用dpll算法    
status resolve_sudoku(int arr[][9], headnode H, bignode  F);
//挖洞主函数
status Dig_Hole(int arr[][9], int n, int m, int k, headnode H, bignode  F);
//判断是否为唯一解
status Juge_Hole(int arr[][9], int n, int m, int k, headnode H, bignode  F);
//找到下一个洞
int Delete_Num(int arr[][9]);