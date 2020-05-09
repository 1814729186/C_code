#include "SuDoKu.h"


/*cnf�ļ�������*/
//��Ǻ������ҵ����ִ���������
void Strategy(headnode Head, int mark_1[N], int mark_2[N])
{
	int temp = 0, t = 0;
	//��ʼ��
	for (int i = 1; i <= N; i++)
	{
		mark_1[i] = 0;
		mark_2[i] = 0;
	}
	//ѡ����ִ������
	EdgeNode *p;
	HeadNode *q;
	q = Head;
	while (q)
	{
		if (q->mark == 1)
		{
			p = q->inside;
			while (p)
			{
				//���ñ�ǣ�Ϊ���ʾcnf�ļ��г��ֹ�
				mark_1[abs(p->num)]++;
				p = p->next;
			}
		}
		q = q->next;
	}
	//����
	for (int i = 1; i <= variable; i++)
	{
		temp = 0;
		t = 0;
		for (int j = 1; j <= variable; j++)
		{
			if (mark_1[j] > temp)
			{
				t = j;
				temp = mark_1[j];
			}
		}
		mark_2[i] = t;
		mark_1[t] = 0;
	}
}
//ɾ�����Ӿ�
status DeleteSingle(bignode G, int flag)
{
	SerchNode *p;
	if (flag > 0)
	{//������Ϊ��
		p = G->serchlist[flag].next;
		while (p)
		{
			//Ϊ����ɾ����Ԫ
			if (p->fu_mark == 1 && p->outside_fu->mark == 1 && p->inside_fu->mark == 1 && p->outside_fu->count != 0)
			{
				p->inside_fu->mark = 0;
				p->outside_fu->count--;
			}
			if (p->zheng_mark == 1 && p->outside_zheng->mark == 1)
			{//Ϊ����ɾ���Ӿ�
				p->outside_zheng->mark = 0;
				p->outside_zheng->flag = flag;
			}
			p = p->next;
		}
	}
	else
	{//������Ϊ��
		p = G->serchlist[-flag].next;
		while (p)
		{
			//Ϊ����ɾ����Ԫ
			if (p->zheng_mark == 1 && p->outside_zheng->mark == 1 && p->inside_zheng->mark == 1 && p->outside_zheng->count != 0)
			{
				p->inside_zheng->mark = 0;
				p->outside_zheng->count--;
			}
			if (p->fu_mark == 1 && p->outside_fu->mark == 1)
			{//Ϊ����ɾ���Ӿ�
				p->outside_fu->mark = 0;
				p->outside_fu->flag = flag;
			}
			p = p->next;
		}
	}
	return OK;
}
//�ָ����Ӿ�
status Recovery(bignode G, int flag)
{
	SerchNode *p;
	if (flag > 0)
	{
		p = G->serchlist[flag].next;
		while (p)
		{
			//Ϊ�����ָ���Ԫ
			if (p->fu_mark == 1 && p->outside_fu->mark == 1 && p->inside_fu->mark == 0)
			{
				p->inside_fu->mark = 1;
				p->outside_fu->count++;
			}
			else if (p->zheng_mark == 1 && p->outside_zheng->mark == 0 && p->outside_zheng->flag == flag)
			{//Ϊ�����ָ��Ӿ�
				p->outside_zheng->mark = 1;
				p->outside_zheng->flag = 0;
			}
			p = p->next;
		}
	}
	else
	{//�������Ǹ���
		p = G->serchlist[-flag].next;
		while (p)
		{
			//Ϊ�����ָ���Ԫ
			if (p->zheng_mark == 1 && p->outside_zheng->mark == 1 && p->inside_zheng->mark == 0)
			{
				p->inside_zheng->mark = 1;
				p->outside_zheng->count++;
			}
			else if (p->fu_mark == 1 && p->outside_fu->mark == 0 && p->outside_fu->flag == flag)
			{//Ϊ�����ָ��Ӿ�
				p->outside_fu->mark = 1;
				p->outside_fu->flag = 0;
			}
			p = p->next;
		}
	}
	return OK;
}
//Ѱ�ҿ��Ӿ�
status FindEmptyClause(HeadNode *Head)
{
	HeadNode *q = NULL;
	q = Head;
	while (q)
	{
		if (q->count == 0 && q->mark == 1)
			return OK;
		q = q->next;
	}
	return FALSE;
}
//�ж��Ƿ�Ϊ�ռ�
status JudgeGroup(HeadNode *Head)
{
	HeadNode *q = NULL;
	q = Head;
	while (q)
	{
		if (q->mark != 0) return FALSE;
		q = q->next;
	}
	return OK;
}
//Ѱ�Ҽ������ֵ
status FindCount(HeadNode *Head)
{
	int flag = 0;
	//���ҵ��Ӿ�
	EdgeNode *p = NULL;
	HeadNode *q = NULL;
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
					flag = p->num;
					goto next_num;
				}
				p = p->next;
			}
		}
		q = q->next;
	}
next_num:
	{
		if (flag > 0 && mark_ture[flag] == 0)
		{
			mark_ture[flag] = 1;
			return flag;
		}
		if (flag < 0 && mark_ture[-flag] == 0)
		{
			mark_ture[-flag] = -1;
			return flag;
		}
	}
	if (FLAG == 1)
	{
		for (int i = 1; i <= variable; i++)
		{
			flag = mark_2[i];
			if (flag != 0 && mark_ture[flag] == 0)
			{
				mark_ture[flag] = 1;
				return flag;
			}
		}
	}
	else if (FLAG == 2)
	{
		for (int i = 1; i <= variable; i++)
		{
			if (mark_ture[i] == 0)
			{
				mark_ture[i] = 1;
				return i;
			}
		}
	}
	return 0;
}
//Dpll�㷨��������
status Dpll_youhua(HeadNode *Head, int now_count, bignode G)
{
	int next_count;
	if (JudgeGroup(Head) == OK)  return OK;//�ж��Ƿ�Ϊ��
	else
	{
		if (FindEmptyClause(Head) == OK)//����Ƿ���ֿ��Ӿ�
		{//���ֿ��Ӿ�ͻָ����Ӿ䲢����
			Recovery(G, now_count);
			mark_ture[abs(now_count)] = 0;
			return FALSE;
		}
		else
		{	//��һ�����ʵ���ֵ,������
			next_count = FindCount(Head);
			if (next_count == 0)return FALSE;
			DeleteSingle(G, next_count);
			if (Dpll_youhua(Head, next_count, G) == OK) return OK;
			else
			{//������ʧ�ܵ����ĸ�ֵ������
				Recovery(G, next_count);
				DeleteSingle(G, -next_count);
				if (Dpll_youhua(Head, -next_count, G) == OK)
				{
					if (next_count>0) mark_ture[next_count] = -1;
					else mark_ture[-next_count] = 1;
					return OK;
				}
				else
				{//����Ҳ����ʧ�ܣ����Ǹü��裬������һ������
					Recovery(G, -next_count);
					mark_ture[abs(next_count)] = 0;
					return FALSE;
				}
			}
		}
	}
}
//ƥ������
void ShuChu_match(int shuzu[][9])
{
	for (int t = 1; t < variable + 1; t++)
	{
		if (mark_ture[t] == 1)
		{
			for (int i = 1; i <= 9; i++)
				for (int j = 1; j <= 9; j++)
					for (int k = 1; k <= 9; k++)
						if (a[i][j][k] == t)
							shuzu[i - 1][j - 1] = k;
		}
	}

}/**/