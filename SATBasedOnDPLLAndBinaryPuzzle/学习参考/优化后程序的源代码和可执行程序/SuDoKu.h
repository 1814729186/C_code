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
int variable , sentences ;//��¼��Ԫ���͹�ʽ��
int FLAG;//ѡ�����
int mark_ture[N];//������ȷ�Ľ�
int mark_1[N];//�洢��Ԫ���ֵĴ���
int mark_2[N];//�洢��Ԫ���±�
int a[10][10][10];//��ά����
int counnt ;//��¼�ڶ���
int hole[9][9];//��¼�ڹ��Ķ�
int shuzu[9][9];//��¼����

//�ڽӱ�һ֮�洢
typedef struct EdgeNode {//С�߼���
	int num;
	int mark;
	struct EdgeNode *next;
}EdgeNode, *edgenode;
typedef struct HeadNode {//��߼���
	int count;
	int mark;
	int flag;
	struct HeadNode *next;
	struct EdgeNode *inside;
}HeadNode, *headnode;
//�ڽӱ��֮����
typedef struct SerchNode {//����ֱ��ɾ��������Ҫ����
	int zheng_mark;//�������Ԫ
	int fu_mark;//��Ǹ���Ԫ
	struct EdgeNode *inside_zheng;//ָ������Ԫ
	struct HeadNode *outside_zheng;
	struct EdgeNode *inside_fu;//ָ�򸺱�Ԫ
	struct HeadNode *outside_fu;
	struct  SerchNode*next;
}SerchNode, SerchList[N+1];
typedef struct {//����ɾ�����ʵļ�����ֵ
	SerchList serchlist;
	int mark;
} BigNode, *bignode;

//                    cnf�ļ���ȡ����Ż���                       

//��ȡcnf�ļ�
status ReadFile(headnode *Head, bignode G,char filename[60]);
//����cnf�ļ�
status Save_File(headnode Head);
//��cnf�ļ�Сǰ��
void check(headnode Head);
//���cnf�ļ�
void Traverse(headnode Head);
//�������
void Realse_Link(headnode * Head);

//                      cnf�ļ��������Ż���                    

//��Ǻ�����ȷ���������ֵ���
void Strategy(headnode Head, int mark_1[N], int mark_2[N]);
//ɾ�����Ӿ�
status DeleteSingle(bignode G, int flag);
//�ָ����Ӿ�
status Recovery(bignode G, int flag);
//�ж��Ƿ�Ϊ�ռ�
status JudgeGroup(HeadNode *Head);
//Ѱ�ҿ��Ӿ�
status FindEmptyClause(HeadNode *Head);
//Ѱ�Ҽ������ֵ
status FindCount(HeadNode *Head);
//Dpll�㷨��������
status Dpll_youhua(HeadNode *Head, int now_count, bignode G);

//                    ������������                        

//�����������
void Print_SuDoKu(int arr[][9]);
//ƥ������
void ShuChu_match(int shuzu[][9]);
//���ö�ȡ����
status Read_SuDoKu(headnode *Head, bignode  G, int mid);
//�����ڽӱ�֮��ʼ���ڽӱ����
status Isinit_SuDoKu(headnode *Head, bignode G);
//����֮�ж�����������Ƿ���ȷ
status SuDoKu_Judge(int arr[][9], int row, int col, int n);
//ƥ�亯��
void Match(int arr[][9], int a[][10][10]);
//ÿ��ȡֵ��� 
void  Create_Cnf_Position(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿ��ȡֵ���
void  Create_Cnf_Row(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿ��ȡֵ���
void Create_Cnf_Col(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿС����ȡֵ���
void Create_Cnf_Block(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿ��ȡֵ��֤
void Insure_Cnf_Position(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿ��ȡֵ��֤
void  Insure_Cnf_Row(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿ��ȡֵ��֤
void  Insure_Cnf_Col(int arr[][9], int a[][10][10], headnode *H, bignode G);
//ÿС����ȡֵ��֤
void  Insure_Cnf_Block(int arr[][9], int a[][10][10], headnode *H, bignode G);
//����ת���ɺ���
void Tranlate(int arr[][9], headnode *H, bignode  F);
//���������ĺ���
status  Create_sudoku(headnode *H, bignode  F, int arr[9][9]);


//                                �ڶ���                           

//����dpll�㷨    
status resolve_sudoku(int arr[][9], headnode H, bignode  F);
//�ڶ�������
status Dig_Hole(int arr[][9], int n, int m, int k, headnode H, bignode  F);
//�ж��Ƿ�ΪΨһ��
status Juge_Hole(int arr[][9], int n, int m, int k, headnode H, bignode  F);
//�ҵ���һ����
int Delete_Num(int arr[][9]);