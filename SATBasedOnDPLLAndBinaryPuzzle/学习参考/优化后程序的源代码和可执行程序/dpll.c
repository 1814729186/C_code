#include "SuDoKu.h"


/*cnf文件处理板块*/
//标记函数，找到出现次数最多的数
void Strategy(headnode Head, int mark_1[N], int mark_2[N])
{
	int temp = 0, t = 0;
	//初始化
	for (int i = 1; i <= N; i++)
	{
		mark_1[i] = 0;
		mark_2[i] = 0;
	}
	//选择出现次数最到的
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
				//设置标记，为零表示cnf文件中出现过
				mark_1[abs(p->num)]++;
				p = p->next;
			}
		}
		q = q->next;
	}
	//排序
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
//删除单子句
status DeleteSingle(bignode G, int flag)
{
	SerchNode *p;
	if (flag > 0)
	{//传进来为正
		p = G->serchlist[flag].next;
		while (p)
		{
			//为负，删除变元
			if (p->fu_mark == 1 && p->outside_fu->mark == 1 && p->inside_fu->mark == 1 && p->outside_fu->count != 0)
			{
				p->inside_fu->mark = 0;
				p->outside_fu->count--;
			}
			if (p->zheng_mark == 1 && p->outside_zheng->mark == 1)
			{//为正，删除子句
				p->outside_zheng->mark = 0;
				p->outside_zheng->flag = flag;
			}
			p = p->next;
		}
	}
	else
	{//传进来为负
		p = G->serchlist[-flag].next;
		while (p)
		{
			//为正，删除变元
			if (p->zheng_mark == 1 && p->outside_zheng->mark == 1 && p->inside_zheng->mark == 1 && p->outside_zheng->count != 0)
			{
				p->inside_zheng->mark = 0;
				p->outside_zheng->count--;
			}
			if (p->fu_mark == 1 && p->outside_fu->mark == 1)
			{//为负，删除子句
				p->outside_fu->mark = 0;
				p->outside_fu->flag = flag;
			}
			p = p->next;
		}
	}
	return OK;
}
//恢复单子句
status Recovery(bignode G, int flag)
{
	SerchNode *p;
	if (flag > 0)
	{
		p = G->serchlist[flag].next;
		while (p)
		{
			//为负，恢复变元
			if (p->fu_mark == 1 && p->outside_fu->mark == 1 && p->inside_fu->mark == 0)
			{
				p->inside_fu->mark = 1;
				p->outside_fu->count++;
			}
			else if (p->zheng_mark == 1 && p->outside_zheng->mark == 0 && p->outside_zheng->flag == flag)
			{//为正，恢复子句
				p->outside_zheng->mark = 1;
				p->outside_zheng->flag = 0;
			}
			p = p->next;
		}
	}
	else
	{//传进来是负数
		p = G->serchlist[-flag].next;
		while (p)
		{
			//为正，恢复变元
			if (p->zheng_mark == 1 && p->outside_zheng->mark == 1 && p->inside_zheng->mark == 0)
			{
				p->inside_zheng->mark = 1;
				p->outside_zheng->count++;
			}
			else if (p->fu_mark == 1 && p->outside_fu->mark == 0 && p->outside_fu->flag == flag)
			{//为负，恢复子句
				p->outside_fu->mark = 1;
				p->outside_fu->flag = 0;
			}
			p = p->next;
		}
	}
	return OK;
}
//寻找空子句
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
//判断是否为空集
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
//寻找假设的真值
status FindCount(HeadNode *Head)
{
	int flag = 0;
	//先找单子句
	EdgeNode *p = NULL;
	HeadNode *q = NULL;
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
//Dpll算法的主函数
status Dpll_youhua(HeadNode *Head, int now_count, bignode G)
{
	int next_count;
	if (JudgeGroup(Head) == OK)  return OK;//判断是否为空
	else
	{
		if (FindEmptyClause(Head) == OK)//检查是否出现空子句
		{//出现空子句就恢复空子句并回溯
			Recovery(G, now_count);
			mark_ture[abs(now_count)] = 0;
			return FALSE;
		}
		else
		{	//找一个合适的真值,并测试
			next_count = FindCount(Head);
			if (next_count == 0)return FALSE;
			DeleteSingle(G, next_count);
			if (Dpll_youhua(Head, next_count, G) == OK) return OK;
			else
			{//将测试失败的数的负值带入检测
				Recovery(G, next_count);
				DeleteSingle(G, -next_count);
				if (Dpll_youhua(Head, -next_count, G) == OK)
				{
					if (next_count>0) mark_ture[next_count] = -1;
					else mark_ture[-next_count] = 1;
					return OK;
				}
				else
				{//负数也测试失败，则标记该假设，返回上一个假设
					Recovery(G, -next_count);
					mark_ture[abs(next_count)] = 0;
					return FALSE;
				}
			}
		}
	}
}
//匹配棋盘
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