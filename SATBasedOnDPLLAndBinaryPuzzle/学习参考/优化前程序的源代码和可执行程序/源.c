#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#pragma warning(disable:4996)

#define FALSE -1
#define OK 1
#define OVERFLOW -2
#define N 350

typedef int datatype;
typedef int status;
int variable, sentences;
int mark[350];

/*小边集合*/
typedef struct Edgenode {
	int num;
	int mark;
	struct Edgenode *next;
}Edgenode, *edgenode;
/*大边集合*/
typedef struct Headnode {
	int count;
	int mark;
	int flag;
	struct Headnode *next;
	struct Edgenode *inside;
}Headnode, *headnode;

/*cnf文件处理板块*/
/*读取cnf文件*/
status ReadFile(headnode *Head)
{
	int mid,counnt=0;
	char ch;
	FILE  *fp;//文件指针
	char filename[70];
	printf("input file name: ");
	scanf("%s", filename);
	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("file open error！\n ");
		return FALSE;
	}
	else
	{
		printf("\n所读取的cnf文件中的注释内容：\n");
		while (fread(&ch, sizeof(char), 1, fp))
		{
			if (ch != 'p') printf("%c", ch);
			else break;
		}
		for (int i = 0; i<5; i++)
			fread(&ch, sizeof(char), 1, fp);
		fscanf(fp, "%d", &variable);
		fscanf(fp, "%d", &sentences);
		counnt = sentences;
		printf("cnf文件中的文字数：%d     公式数： %d\n", variable, sentences);

		//读取公式部分
		Edgenode *p = NULL, *t = NULL;
		Headnode *r = NULL, *q = NULL;
		r = (Headnode*)malloc(sizeof(Headnode));
		q = r;
		(*Head) = r;
		r->next = NULL;
		r->inside = NULL;
		r->count = 0;
		r->mark = 0;
		r->flag = 0;
		while (fscanf(fp, "%d", &mid) != EOF && counnt)
		{
			if (mid != 0)
			{
				//读入变元
				p = (Edgenode*)malloc(sizeof(Edgenode));
				p->next = NULL;
				p->num = mid;
				p->mark = 1;
				if (q->inside == NULL)
				{
					q->inside = p;
				}
				else
				{
					t = q->inside;
					while (t->next)
						t = t->next;
					t->next = p;
				}
				q->count++;
			}
			else
			{
				//读入下一个公式
				r = (Headnode*)malloc(sizeof(Headnode));
				r->next = NULL;
				r->inside = NULL;
				r->count = 0;
				r->mark = 0;
				r->flag = 0;
				if (q->inside != NULL) q->mark = 1;
				q->next = r;
				q = r;
				counnt--;
			}
		}
	}
	return OK;
}
/*输出cnf文件*/
status Traverse(headnode Head)
{
	int mark = 0;
	Edgenode *p;
	Headnode *q;
	q = Head;
	printf("___________________________\n");
	while (q)
	{
		if (q->mark == 1)
		{
			printf("%2d :", q->count);
			p = q->inside;
			while (p)
			{
				if (p->mark == 1)
					printf("%4d", p->num);
				p = p->next;
			}
		}
		if (q->mark == 1) printf("\n");
		q = q->next;
	}
	printf("___________________________\n");
	return OK;
}
/*标记函数，确定假设的真值情况*/
void Judgement(headnode Head, int mark[N])
{
	int temp;
	Edgenode *p;
	Headnode *q;
	q = Head;
	while (q)
	{
		if (q->mark == 1)
		{
			p = q->inside;
			while (p)
			{
				//设置标记，为零表示cnf文件中出现过
				if (p->num > 0)  mark[p->num] = 0;
				else
				{
					temp = p->num;
					mark[-temp] = 0;
				};
				p = p->next;
			}
		}
		q = q->next;
	}
}

/*发现单子句*/
status FindSingle(Headnode *Head, int * flag)
{
	Edgenode *p = NULL;
	Headnode *q = NULL;
	q = Head;
	while (q)
	{
		p = q->inside;
		if (q->count == 1 && q->mark == 1)//记录变元数量
		{
			while (p)
			{
				if (p->mark == 1)
				{
					*flag = p->num;
					return OK;
				}
				p = p->next;
			}
		}
		q = q->next;
	}
	return FALSE;
}
/*删除单子句*/
status DeleteSingle(Headnode *Head, int flag)
{
	Edgenode *p = NULL;
	Headnode *q = NULL;
	q = Head;
	while (q)
	{
		p = q->inside;
		while (p)
		{
			//删除单子句
			if (q->mark == 1 && p->num == flag)
			{
				q->mark = 0;
				q->flag = flag;
				break;
			}
			//删除变元
			if (q->mark == 1 && p->num == -flag&&p->mark != 0 && q->count != 0)
			{
				p->mark = 0;
				q->count--;
			}
			p = p->next;
		}
		q = q->next;
	}
	return OK;
}
/*恢复单子句*/
status Recovery(Headnode *Head, int flag)
{
	Edgenode *p = NULL;
	Headnode *q = NULL;
	q = Head;
	while (q)
	{
		p = q->inside;
		while (p)
		{
			//恢复单子句
			if (p->num == flag && q->mark == 0 && q->flag == flag)
			{
				q->mark = 1;
				q->flag = 0;
				break;
			}
			//恢复变元
			else if (p->num == -flag && p->mark == 0 && q->mark == 1)
			{
				p->mark = 1;
				q->count++;
			}
			p = p->next;
		}
		q = q->next;
	}
	return OK;
}
/*寻找空子句*/
status FindemptyClause(Headnode *Head)
{
	Headnode *q = NULL;
	q = Head;
	while (q)
	{
		//p = q->inside;
		if (q->count == 0 && q->mark == 1)
			return OK;
		q = q->next;
	}
	return FALSE;
}
/*判断s是否为空集*/
status JudgeGroup(Headnode *Head)
{
	Headnode *q = NULL;
	q = Head;
	while (q)
	{
		if (q->mark != 0) return FALSE;
		q = q->next;
	}
	return OK;
}
/*寻找假设的真值*/
int FindCount(Headnode *Head)
{
	int flag;
	//先找单子句
	if (FindSingle(Head, &flag) == OK)
	{
		if (flag > 0 && mark[flag] == 0)
		{
			//printf("11111当前假设的真值为:%d\n", flag);
			mark[flag] = 1;
			return flag;
		}
		if (flag < 0 && mark[-flag] == 0)
		{
			//printf("2222当前假设的真值为:%d\n", flag);
			mark[-flag] = -1;
			return flag;
		}
	}
	else
	{
		for (int i = 1; i <= variable; i++)
		{
			if (mark[i] == 0)
			{
				//printf("333当前假设的真值为:%d\n", i);
				mark[i] = 1;
				return i;
			}
		}
	}
	return 0;
}
/*dpll函数版块*/
status Dpll(Headnode *Head, int now_count)
{
	int next_count;
	//判断是否为空
	if (JudgeGroup(Head) == OK)  return OK;
	else
	{
		//检查是否出现空子句，出现就修复然后return false
		if (FindemptyClause(Head) == OK)
		{
			//printf("需要回溯的数值：%2d\n", now_count);
			Recovery(Head, now_count);
			//traverse(head);
			if (now_count>0)
				mark[now_count] = 0;
			else mark[-now_count] = 0;
			return FALSE;
		}
		else
		{
			//不出现即找一个合适的真值
			//测试当前假设真值是否成立
			next_count = FindCount(Head);
			if (next_count == 0)return FALSE;
			DeleteSingle(Head, next_count);
			if (Dpll(Head, next_count) == OK) return OK;
			else
			{
				Recovery(Head, next_count);
				//printf("当前检验对应负值：%2d\n", -next_count);
				DeleteSingle(Head, -next_count);
				//Traverse(Head);
				if (Dpll(Head, -next_count) == OK)
				{
					//printf("当前检验对应负值：%2d\n", -next_count);
					//Traverse(Head);
					if (next_count>0)
						mark[next_count] = -1;
					else mark[-next_count] = 1;
					return OK;
				}
				else
				{//负数也测试失败，则标记该假设，返回上一个假设
					Recovery(Head, -next_count);
					//printf("正负检验失败，退出该值并消去该值\n");
					//traverse(head);
					if (next_count>0)
						mark[next_count] = 0;
					else mark[-next_count] = 0;
					return FALSE;
				}
				//还不成立这个数的标记清空，退回到上一个数；
			}
		}
	}
}
/*主函数*/
int main(void)
{
	headnode T;
	Headnode Arr;
	T = &Arr;
	int op = 1;
	int t1, t2, flag = 0;
	while (op) {
		system("cls");
		printf("\n                menu for suduku or sat           \n");
		printf("---------------------------------------------------\n");
		printf("         1.readfile              3.dpll \n");
		printf("    	 2.printfile             0.exit\n");
		printf("---------------------------------------------------\n");
		if (scanf("%d", &op))
		{
			switch (op) {
			case 1:
				if (ReadFile(&T) == OK)
					printf("cnf文件成功读取！\n");
				else printf("cnf文件读取失败！\n");
				getchar(); getchar();
				break;
			case 2:
				if (Traverse(T) == OK)
					printf("cnf文件输出成功！\n");
				else printf("cnf文件输出失败！\n");
				getchar(); getchar();
				break;
			case 3:
				for (int i = 1; i <= variable; i++)
					mark[i] = 2;
				Judgement(T, mark);
				t1 = clock();
				if (Dpll(T, 1) == OK)
				{
					flag = 1;
					printf("dpll算法处理成功，文件有解！\n");
				}
				else  printf("dpll算法处理失败，文件无解！\n");
				t2 = clock();
				printf("\n时间：%dms\n", (t2 - t1) );
				if (flag == 1)
				{
					printf("cnf文件的解为：\n");
					for (int i = 1; i <= variable; i++)
					{
						if (mark[i] == 1)printf("%4d  ", i);
						else if (mark[i] == -1)printf("%4d   ", -i);
						if (i % 10 == 0) printf("\n");
					}
				}
				getchar(); getchar();
				break;
			default:
				printf("输入错误！\n");
				getchar(); getchar();
				break;
			}//end of switch
		}
		else {
			printf("输入错误！\n");
			getchar(); getchar(); getchar();
		}
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	system("pause");
	return 0;
}//end of main()

