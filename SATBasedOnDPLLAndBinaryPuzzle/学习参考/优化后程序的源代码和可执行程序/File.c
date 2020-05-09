#include "SuDoKu.h"

/*cnf�ļ���ȡ���*/
//��ȡcnf�ļ�
status ReadFile(headnode *Head, bignode G,char filename[60])
{
	int mid,countt=0;
	char ch;
	FILE  *fp;//�ļ�ָ��
	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("File open error��\n ");
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
		for (int i = 0; i < 5; i++)
			fread(&ch, sizeof(char), 1, fp);
		fscanf(fp, "%d", &variable);
		fscanf(fp, "%d", &sentences);
		countt = sentences;
		printf("cnf�ļ��е���������%d     ��ʽ���� %d\n", variable, sentences);

		//��ȡ��ʽ����
		EdgeNode *p = NULL, *t = NULL;
		HeadNode *r = NULL, *q = NULL;
		SerchNode *n = NULL, *x = NULL;

		//��ʼ��������
		for (int i = 0; i <= N; i++)
		{
			G->mark = 0;
			G->serchlist[i].fu_mark = 0;
			G->serchlist[i].zheng_mark = 0;
			G->serchlist[i].inside_fu = NULL;
			G->serchlist[i].inside_zheng = NULL;
			G->serchlist[i].next = NULL;
			G->serchlist[i].outside_fu = NULL;
			G->serchlist[i].outside_zheng = NULL;
		}

		r = (HeadNode*)malloc(sizeof(HeadNode));
		q = r;
		(*Head) = r;
		r->next = NULL;
		r->inside = NULL;
		r->count = 0;
		r->mark = 0;
		r->flag = 0;
		while (fscanf(fp, "%d", &mid) != EOF&& countt )
		{
			if (mid != 0)
			{
				//�����Ԫ
				p = (EdgeNode*)malloc(sizeof(EdgeNode));
				p->next = NULL;
				p->num = mid;
				p->mark = 1;
				if (q->inside == NULL)  q->inside = p;
				else
				{
					t = q->inside;
					while (t->next)
						t = t->next;
					t->next = p;
				}
				q->count++;

				//������Ľ���
				n = (SerchNode*)malloc(sizeof(SerchNode));
				if (mid > 0)
				{
					n->zheng_mark = 1;
					n->inside_zheng = p;
					n->outside_zheng = q;
					n->inside_fu = NULL;
					n->outside_fu = NULL;
					n->fu_mark = 0;
					n->next = NULL;
					x = G->serchlist[mid].next;
					if (!x) G->serchlist[mid].next = n;
					else
					{
						while (x->next)
							x = x->next;
						x->next = n;
					}
				}
				else
				{
					n->zheng_mark = 0;
					n->next = NULL;
					n->inside_zheng = NULL;
					n->outside_zheng = NULL;
					n->inside_fu = p;
					n->outside_fu = q;
					n->fu_mark = 1;
					x = G->serchlist[-mid].next;
					if (!x) G->serchlist[-mid].next = n;
					else
					{
						while (x->next)
							x = x->next;
						x->next = n;
					}
				}
			}
			else
			{
				//������һ����ʽ
				r = (HeadNode*)malloc(sizeof(HeadNode));
				r->next = NULL;
				r->inside = NULL;
				r->count = 0;
				r->mark = 0;
				r->flag = 0;
				if (q->inside != NULL) q->mark = 1;
				q->next = r;
				q = r;
				countt--;
			}
		}
	}
	return OK;
}
//���cnf�ļ�
void Traverse(headnode Head)
{
	EdgeNode *p = NULL;
	HeadNode *q = NULL;
	q = Head;
	printf("___________________________\n");
	while (q)
	{
		p = q->inside;
		while (p)
		{
			printf("%6d", p->num);
			p = p->next;
		}
		printf("\n");
		q = q->next;
	}
}
//��cnf�ļ�Сǰ��
void check(headnode Head)
{
	EdgeNode *p = NULL;
	HeadNode *q = NULL;
	q = Head;
	while (q)
	{
		sentences++;
		q = q->next;
	}
}
//����cnf�ļ�
status Save_File(headnode Head)
{
	EdgeNode *p = NULL;
	HeadNode *q = NULL;
	FILE  *fp;//�ļ�111ָ��
	char filename[60];
	printf("input file name: ");
	scanf("%s", filename);
	if ((fp = fopen(filename, "wb")) == NULL)
	{
		printf("File open error��\n ");
		return FALSE;
	}
	else
	{
		fprintf(fp, "%s", "p cnf ");
		fprintf(fp, "%d ", variable);
		fprintf(fp, "%d \n", sentences);
		q = Head;
		while (q)
		{
			p = q->inside;
			while (p)
			{
				fprintf(fp, "%d  ", p->num);
				p = p->next;
			}
			fprintf(fp, "%d \n", 0);
			q = q->next;
		}
		fclose(fp);
	}
	return OK;
}
//��������
void Realse_Link(headnode * Head)
{
	EdgeNode *p = NULL, *r = NULL;
	HeadNode *q = NULL, *t = NULL;
	SerchNode *n = NULL, *x = NULL;

	q = *Head;
	while (q)
	{
		p = q->inside;
		while (p)
		{
			r = p->next;
			free(p);
			p = r;
		}
		q->inside = NULL;
		q = q->next;
	}
	q = *Head;
	while (q)
	{
		t = q->next;
		free(q);
		q = t;
	}
	*Head = NULL;
}