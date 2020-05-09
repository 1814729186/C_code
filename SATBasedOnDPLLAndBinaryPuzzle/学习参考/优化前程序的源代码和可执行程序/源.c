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

/*С�߼���*/
typedef struct Edgenode {
	int num;
	int mark;
	struct Edgenode *next;
}Edgenode, *edgenode;
/*��߼���*/
typedef struct Headnode {
	int count;
	int mark;
	int flag;
	struct Headnode *next;
	struct Edgenode *inside;
}Headnode, *headnode;

/*cnf�ļ�������*/
/*��ȡcnf�ļ�*/
status ReadFile(headnode *Head)
{
	int mid,counnt=0;
	char ch;
	FILE  *fp;//�ļ�ָ��
	char filename[70];
	printf("input file name: ");
	scanf("%s", filename);
	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("file open error��\n ");
		return FALSE;
	}
	else
	{
		printf("\n����ȡ��cnf�ļ��е�ע�����ݣ�\n");
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
		printf("cnf�ļ��е���������%d     ��ʽ���� %d\n", variable, sentences);

		//��ȡ��ʽ����
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
				//�����Ԫ
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
				//������һ����ʽ
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
/*���cnf�ļ�*/
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
/*��Ǻ�����ȷ���������ֵ���*/
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
				//���ñ�ǣ�Ϊ���ʾcnf�ļ��г��ֹ�
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

/*���ֵ��Ӿ�*/
status FindSingle(Headnode *Head, int * flag)
{
	Edgenode *p = NULL;
	Headnode *q = NULL;
	q = Head;
	while (q)
	{
		p = q->inside;
		if (q->count == 1 && q->mark == 1)//��¼��Ԫ����
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
/*ɾ�����Ӿ�*/
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
			//ɾ�����Ӿ�
			if (q->mark == 1 && p->num == flag)
			{
				q->mark = 0;
				q->flag = flag;
				break;
			}
			//ɾ����Ԫ
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
/*�ָ����Ӿ�*/
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
			//�ָ����Ӿ�
			if (p->num == flag && q->mark == 0 && q->flag == flag)
			{
				q->mark = 1;
				q->flag = 0;
				break;
			}
			//�ָ���Ԫ
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
/*Ѱ�ҿ��Ӿ�*/
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
/*�ж�s�Ƿ�Ϊ�ռ�*/
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
/*Ѱ�Ҽ������ֵ*/
int FindCount(Headnode *Head)
{
	int flag;
	//���ҵ��Ӿ�
	if (FindSingle(Head, &flag) == OK)
	{
		if (flag > 0 && mark[flag] == 0)
		{
			//printf("11111��ǰ�������ֵΪ:%d\n", flag);
			mark[flag] = 1;
			return flag;
		}
		if (flag < 0 && mark[-flag] == 0)
		{
			//printf("2222��ǰ�������ֵΪ:%d\n", flag);
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
				//printf("333��ǰ�������ֵΪ:%d\n", i);
				mark[i] = 1;
				return i;
			}
		}
	}
	return 0;
}
/*dpll�������*/
status Dpll(Headnode *Head, int now_count)
{
	int next_count;
	//�ж��Ƿ�Ϊ��
	if (JudgeGroup(Head) == OK)  return OK;
	else
	{
		//����Ƿ���ֿ��Ӿ䣬���־��޸�Ȼ��return false
		if (FindemptyClause(Head) == OK)
		{
			//printf("��Ҫ���ݵ���ֵ��%2d\n", now_count);
			Recovery(Head, now_count);
			//traverse(head);
			if (now_count>0)
				mark[now_count] = 0;
			else mark[-now_count] = 0;
			return FALSE;
		}
		else
		{
			//�����ּ���һ�����ʵ���ֵ
			//���Ե�ǰ������ֵ�Ƿ����
			next_count = FindCount(Head);
			if (next_count == 0)return FALSE;
			DeleteSingle(Head, next_count);
			if (Dpll(Head, next_count) == OK) return OK;
			else
			{
				Recovery(Head, next_count);
				//printf("��ǰ�����Ӧ��ֵ��%2d\n", -next_count);
				DeleteSingle(Head, -next_count);
				//Traverse(Head);
				if (Dpll(Head, -next_count) == OK)
				{
					//printf("��ǰ�����Ӧ��ֵ��%2d\n", -next_count);
					//Traverse(Head);
					if (next_count>0)
						mark[next_count] = -1;
					else mark[-next_count] = 1;
					return OK;
				}
				else
				{//����Ҳ����ʧ�ܣ����Ǹü��裬������һ������
					Recovery(Head, -next_count);
					//printf("��������ʧ�ܣ��˳���ֵ����ȥ��ֵ\n");
					//traverse(head);
					if (next_count>0)
						mark[next_count] = 0;
					else mark[-next_count] = 0;
					return FALSE;
				}
				//��������������ı����գ��˻ص���һ������
			}
		}
	}
}
/*������*/
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
					printf("cnf�ļ��ɹ���ȡ��\n");
				else printf("cnf�ļ���ȡʧ�ܣ�\n");
				getchar(); getchar();
				break;
			case 2:
				if (Traverse(T) == OK)
					printf("cnf�ļ�����ɹ���\n");
				else printf("cnf�ļ����ʧ�ܣ�\n");
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
					printf("dpll�㷨����ɹ����ļ��н⣡\n");
				}
				else  printf("dpll�㷨����ʧ�ܣ��ļ��޽⣡\n");
				t2 = clock();
				printf("\nʱ�䣺%dms\n", (t2 - t1) );
				if (flag == 1)
				{
					printf("cnf�ļ��Ľ�Ϊ��\n");
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
				printf("�������\n");
				getchar(); getchar();
				break;
			}//end of switch
		}
		else {
			printf("�������\n");
			getchar(); getchar(); getchar();
		}
	}//end of while
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
	system("pause");
	return 0;
}//end of main()

