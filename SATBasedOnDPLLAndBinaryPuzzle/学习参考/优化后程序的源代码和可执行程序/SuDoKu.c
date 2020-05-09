#include "SuDoKu.h"

//������������

//�����������
void Print_SuDoKu(int arr[][9])
{
	int i, j;
	printf("-------------------------------\n");
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (j % 3 == 0) printf("|");
			if (arr[i][j] != 0) printf("%2d ", arr[i][j]);
			else printf(" _ ");
			if (j == 8) printf("|\n");
		}
		if ((i + 1) % 3 == 0) printf("-------------------------------\n");
	}
}
//���ö�ȡ����
status Read_SuDoKu(headnode *Head, bignode  G, int mid)
{
	//��ȡ��ʽ����
	EdgeNode *p = NULL, *t = NULL;
	HeadNode *r = NULL, *q = NULL;
	SerchNode *n = NULL, *x = NULL;
	q = *Head;
	while (q->next)
	{
		q = q->next;
	}
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
	}
	return OK;
}
//�����ڽӱ�֮��ʼ���ڽӱ����
status Isinit_SuDoKu(headnode *Head, bignode G)
{
	HeadNode *r = NULL, *q = NULL;
	//��ʼ��������
	for (int i = 1; i < N; i++)
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
	//��ʼ���洢��
	r = (HeadNode*)malloc(sizeof(HeadNode));
	q = r;
	(*Head) = r;
	r->next = NULL;
	r->inside = NULL;
	r->count = 0;
	r->mark = 0;
	r->flag = 0;
	return OK;
}
//����֮�ж�����������Ƿ���ȷ
status SuDoKu_Judge(int arr[][9], int row, int col, int n)
{
	int i, j;

	for (i = 0; i < 9; i++) {// �ж����ظ�
		if (arr[row][i] == n) {
			return FALSE;
		}
	}
	for (j = 0; j < 9; j++) {// �ж����ظ�
		if (arr[j][col] == n) {
			return FALSE;
		}
	}
	// �ж�����С�Ź����ظ�
	int x = (row / 3) * 3;
	int y = (col / 3) * 3;
	for (i = x; i < x + 3; i++)
		for (j = y; j < y + 3; j++)
			if (arr[i][j] == n) return FALSE;

	return OK;
}
//ƥ�亯��
void Match(int arr[][9], int a[][10][10])
{
	int order = 1;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (arr[i][j] == 0)
			{
				for (int k = 1; k <= 9; k++)
				{
					if (SuDoKu_Judge(arr, i, j, k) == OK)
					{
						a[i + 1][j + 1][k] = order;
						order++;
					}
				}
			}
		}
	}
	variable = order - 1;
}
//ÿ��ȡֵ��� 
void  Create_Cnf_Position(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t;
	int part_1[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
		part_1[i] = 0;

	for (i = 0; i < 9; i++) //���������б�
	{
		for (j = 0; j < 9; j++)
		{
			if (arr[i][j] == 0)//�ҵ��ո�
			{
				for (k = 1; k <= 9; k++)//�ҳ����ܵ�ȡֵ
					if (arr[i][j] == 0 && SuDoKu_Judge(arr, i, j, k) == OK)
						part_1[k] = 1;
				//������ܵ�����ȡֵ
				for (k = 1; k <= 9; k++)
					if (part_1[k] == 1)
					{
						for (t = k + 1; t <= 9; t++)
						{
							if (part_1[t] == 1)
							{
								Read_SuDoKu(H, G, -a[i + 1][j + 1][k]);
								Read_SuDoKu(H, G, -a[i + 1][j + 1][t]);
								Read_SuDoKu(H, G, 0);
							}
						}
						part_1[k] = 0;
					}
			}
		}
	}
}
//ÿ��ȡֵ���
void  Create_Cnf_Row(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t;
	int part_2[10], sign_2[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
	{
		part_2[i] = 0;
		sign_2[i] = 1;//��¼ÿ�л���Ҫ�����
	}

	for (i = 0; i<9; i++) //���������б�
	{
		for (j = 0; j < 9; j++)//�ҵ�����Ҫ������
		{
			if (arr[i][j] != 0)
				sign_2[arr[i][j]] = 0;
		}
		for (t = 1; t <= 9; t++)//�ҵ����ܲ���x��λ��
		{
			if (sign_2[t] == 1)//��Ҫ����t�����ҿ��ܲ�t��λ��
			{
				for (k = 1; k <= 9; k++)
					if (arr[i][k - 1] == 0 && SuDoKu_Judge(arr, i, k - 1, t) == OK)
						part_2[k] = 1;
				for (k = 1; k <= 9; k++)//ȫ�����
				{
					if (part_2[k] == 1)
					{
						for (int r = k + 1; r <= 9; r++)
						{
							if (part_2[r] == 1)
							{
								Read_SuDoKu(H, G, -a[i + 1][k][t]);
								Read_SuDoKu(H, G, -a[i + 1][r][t]);
								Read_SuDoKu(H, G, 0);
							}
						}
						part_2[k] = 0;
					}
				}
			}
			sign_2[t] = 1;
		}
	}
}
//ÿ��ȡֵ���
void Create_Cnf_Col(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t;
	int part_3[10], sign_3[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
	{
		part_3[i] = 0;
		sign_3[i] = 1;//��¼ÿ�л���Ҫ�����
	}

	for (j = 0; j<9; j++) //���������б�
	{
		for (i = 0; i < 9; i++)//�ҵ�����Ҫ������
		{
			if (arr[i][j] != 0)
				sign_3[arr[i][j]] = 0;
		}
		for (t = 1; t <= 9; t++)//�ҵ����ܲ���x��λ��
		{
			if (sign_3[t] == 1)//��Ҫ����t�����ҿ��ܲ�t��λ��
			{
				for (k = 1; k <= 9; k++)//
					if (arr[k - 1][j] == 0 && SuDoKu_Judge(arr, k - 1, j, t) == OK)
						part_3[k] = 1;
				for (k = 1; k <= 9; k++)//ȫ�����
				{
					if (part_3[k] == 1)
					{
						for (int r = k + 1; r <= 9; r++)
						{
							if (part_3[r] == 1)
							{
								Read_SuDoKu(H, G, -a[k][j + 1][t]);
								Read_SuDoKu(H, G, -a[r][j + 1][t]);
								Read_SuDoKu(H, G, 0);
							}
						}
						part_3[k] = 0;
					}
				}
			}
			sign_3[t] = 1;
		}
	}
}
//ÿС����ȡֵ���
void Create_Cnf_Block(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t, n;
	int part_4[10], sign_4[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
	{
		sign_4[i] = 1;//��¼ÿ��С���黹��Ҫ�����
		part_4[i] = 0;//��¼�������λ��
	}

	for (i = 0; i < 9; i += 3)
	{
		for (j = 0; j < 9; j += 3)
		{
			for (k = i; k < i + 3; k++)//�ҵ�С�����п�����������
			{
				for (t = j; t < j + 3; t++)
				{
					if (arr[k][t] != 0)
						sign_4[arr[k][t]] = 0;
				}
			}
			for (n = 1; n <= 9; n++)
			{
				if (sign_4[n] == 1)//�������ҳ���������k��λ��
				{
					for (k = i; k < i + 3; k++)//�ҵ����ܷ����λ��
					{
						for (t = j; t < j + 3; t++)
							if (arr[k][t] == 0 && SuDoKu_Judge(arr, k, t, n) == OK)
							{
								if (k % 3 == 0) part_4[(t % 3) + 1] = 1;
								else if (k % 3 == 1)part_4[4 + (t % 3)] = 1;
								else part_4[7 + (t % 3)] = 1;
							}
					}
					for (k = 1; k <= 9; k++)//ȫ�����
						if (part_4[k] == 1)
						{
							for (t = k + 1; t <= 9; t++)
								if (part_4[t] == 1)
								{
									if (k >= 1 && k <= 3)
										Read_SuDoKu(H, G, -a[i + 1][j + k][n]);
									else if (k >= 4 && k <= 6)
										Read_SuDoKu(H, G, -a[i + 2][j + k - 3][n]);
									else
										Read_SuDoKu(H, G, -a[i + 3][j + k - 6][n]);

									if (t >= 1 && t <= 3)
										Read_SuDoKu(H, G, -a[i + 1][j + t][n]);
									else if (t >= 4 && t <= 6)
										Read_SuDoKu(H, G, -a[i + 2][j + t - 3][n]);
									else
										Read_SuDoKu(H, G, -a[i + 3][j + t - 6][n]);
									Read_SuDoKu(H, G, 0);
								}
							part_4[k] = 0;
						}
				}
				sign_4[n] = 1;
			}
		}
	}
}
//ÿ��ȡֵ��֤
void Insure_Cnf_Position(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k;
	int part_5[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
		part_5[i] = 0;

	for (i = 0; i < 9; i++) //���������б�
	{
		for (j = 0; j < 9; j++)
		{
			if (arr[i][j] == 0)//�ҵ��ո�
			{
				for (k = 1; k <= 9; k++)//�ҳ����ܵ�ȡֵ
					if (arr[i][j] == 0 && SuDoKu_Judge(arr, i, j, k) == OK)
						part_5[k] = 1;
				//������ܵ�ȡֵ
				for (k = 1; k <= 9; k++)
				{
					if (part_5[k] == 1)
					{
						Read_SuDoKu(H, G, a[i + 1][j + 1][k]);
					}
					part_5[k] = 0;
				}
				Read_SuDoKu(H, G, 0);
			}
		}
	}
}
//ÿ��ȡֵ��֤
void  Insure_Cnf_Row(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t;
	int part_6[10], sign_6[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
	{
		part_6[i] = 0;
		sign_6[i] = 1;//��¼ÿ�л���Ҫ�����
	}

	for (i = 0; i<9; i++) //���������б�
	{
		for (j = 0; j < 9; j++)//�ҵ�����Ҫ������
		{
			if (arr[i][j] != 0)
				sign_6[arr[i][j]] = 0;
		}
		for (t = 1; t <= 9; t++)//�ҵ����ܲ���x��λ��
		{
			if (sign_6[t] == 1)//��Ҫ����t�����ҿ��ܲ�t��λ��
			{
				for (k = 1; k <= 9; k++)
					if (arr[i][k - 1] == 0 && SuDoKu_Judge(arr, i, k - 1, t) == OK)
						part_6[k] = 1;
				for (k = 1; k <= 9; k++)//ȫ�����
				{
					if (part_6[k] == 1)
					{
						Read_SuDoKu(H, G, a[i + 1][k][t]);
					}
					part_6[k] = 0;
				}
				Read_SuDoKu(H, G, 0);
			}
			sign_6[t] = 1;
		}
	}
}
//ÿ��ȡֵ��֤
void  Insure_Cnf_Col(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t;
	int part_7[10], sign_7[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
	{
		part_7[i] = 0;
		sign_7[i] = 1;//��¼ÿ�л���Ҫ�����
	}

	for (j = 0; j < 9; j++) //���������б�
	{
		for (i = 0; i < 9; i++)//�ҵ�����Ҫ������
		{
			if (arr[i][j] != 0)
				sign_7[arr[i][j]] = 0;
		}
		for (t = 1; t <= 9; t++)//�ҵ����ܲ���x��λ��
		{
			if (sign_7[t] == 1)//��Ҫ����t�����ҿ��ܲ�t��λ��
			{
				for (k = 1; k <= 9; k++)
					if (arr[k - 1][j] == 0 && SuDoKu_Judge(arr, k - 1, j, t) == OK)
						part_7[k] = 1;
				for (k = 1; k <= 9; k++)
				{
					if (part_7[k] == 1)
					{
						Read_SuDoKu(H, G, a[k][j + 1][t]);
					}
					part_7[k] = 0;
				}
				Read_SuDoKu(H, G, 0);
			}
			sign_7[t] = 1;
		}
	}
}
//ÿС����ȡֵ��֤
void  Insure_Cnf_Block(int arr[][9], int a[][10][10], headnode *H, bignode G)
{
	int i, j, k, t, n;
	int part_8[10][10], sign_8[10];
	for (int i = 1; i <= 9; i++)//��ʼ��
		sign_8[i] = 1;//��¼ÿ��С���黹��Ҫ�����

	for (i = 0; i < 9; i++)//��ʼ��
		for (j = 0; j < 9; j++)
			part_8[i][j] = 0;

	for (i = 0; i < 9; i += 3)
		for (j = 0; j < 9; j += 3)
		{
			for (k = i; k < i + 3; k++)//�ҵ�С�����п�����������
			{
				for (t = j; t < j + 3; t++)
				{
					if (arr[k][t] != 0)
						sign_8[arr[k][t]] = 0;
				}
			}
			for (n = 1; n <= 9; n++)
			{
				if (sign_8[n] == 1)//�������ҳ���������k��λ��
				{
					for (k = i; k < i + 3; k++)//�ҵ����ܷ����λ��
					{
						for (t = j; t < j + 3; t++)
						{
							if (arr[k][t] == 0 && SuDoKu_Judge(arr, k, t, n) == OK)
								part_8[k][t] = 1;
						}
					}
					for (k = i; k < i + 3; k++)//ȫ�����
					{
						for (t = j; t < j + 3; t++)
						{
							if (part_8[k][t] == 1)
							{
								Read_SuDoKu(H, G, a[k + 1][t + 1][n]);
							}
							part_8[k][t] = 0;
						}
					}
					Read_SuDoKu(H, G, 0);
				}
				sign_8[n] = 1;
			}
		}
}
//����ת���ɺ���
void Tranlate(int arr[9][9], headnode *H, bignode  F)
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 10; k++)
				a[i][j][k] = 0;
	Match(arr, a);
	Isinit_SuDoKu(H, F);
	Create_Cnf_Position(arr, a, H, F);
	Create_Cnf_Row(arr, a, H, F);
	Create_Cnf_Col(arr, a, H, F);
	Create_Cnf_Block(arr, a, H, F);
	Insure_Cnf_Position(arr, a, H, F);
	Insure_Cnf_Row(arr, a, H, F);
	Insure_Cnf_Col(arr, a, H, F);
	Insure_Cnf_Block(arr, a, H, F);
}
//���������ĺ���
status  Create_sudoku(headnode *H, bignode  F, int arr[9][9])
{
	int n, m, k, count = 0;
	srand((int)time(0));
	printf("���ڴ����������̣�\n");
	while (count <= 17)
	{

		n = rand() % 9;
		m = rand() % 9;
		k = (rand() % 9) + 1;
		if (arr[n][m] == 0 && SuDoKu_Judge(arr, n, m, k) == OK)
		{
			count++;
			arr[n][m] = k;
		}
	}
	Print_SuDoKu(arr);
	Tranlate(arr, H, F);
	return OK;
}