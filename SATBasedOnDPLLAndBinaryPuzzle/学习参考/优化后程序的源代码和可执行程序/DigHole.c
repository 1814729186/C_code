#include "SuDoKu.h"


//�ҵ���һ��Ҫɾ���ĵ�
int Delete_Num(int arr[][9])
{ 
	int k = 0;
	for (int i = 0; i < 9; i++)//��ʼ��
		if (i % 2 == 0)
		{
			for (int j = 0; j < 9; j++)
			{
				if (arr[i][j] != 0 && hole[i][j] == 0)
				{
					k = arr[i][j];
					arr[i][j] = 0;
					hole[i][j] = 1;
					return (i + 1) * 100 + (j + 1) * 10 + k;
				}
			}
		}
		else
		{
			for (int j = 8; j >=0; j--)
				if (arr[i][j] != 0 && hole[i][j] == 0)
				{
					k = arr[i][j];
					arr[i][j] = 0;
					hole[i][j] = 1;
					return (i + 1) * 100 + (j + 1) * 10 + k;
				}
		}
	return 0;
}
//�ж϶��Ƿ������
status Juge_Hole(int arr[][9], int n, int m, int k, headnode H, bignode  F)
{
	for(int i= 1;i <= 9;i++)
		if (i != k && SuDoKu_Judge(arr, n, m, i)==OK)//�ж���������Ƿ������������
		{
			arr[n][m] = i;
			if (resolve_sudoku(arr, H, F) == OK)//�ж��Ƿ���Ψһ��
			{
				arr[n][m] = k;
				hole[n][m] = 1;
				return FALSE;
			}
			arr[n][m] = k;
		}
	arr[n][m] = 0;
	hole[n][m] = 1;
	return OK;
}
//�ڶ���������
status Dig_Hole(int arr[][9],int n, int m, int k, headnode H, bignode  F)
{
	counnt--;
	int next_hole=0,sig=4;
	if (counnt == 0) return OK;
	else
	{
		if (Juge_Hole(arr, n-1, m-1, k,H,F) == FALSE )//�жϴ�����Ƿ���������
		{//û�⸴ԭ
			arr[n-1][m-1] = k;
			hole[n-1][m-1] = 1;
			counnt++;
			return FALSE;
		}
		else
		{
			next_hole = Delete_Num(arr);//�ҵ���һ��Ҫɾ������
			if (next_hole == 0)return OK;
			if (Dig_Hole(arr, next_hole / 100, (next_hole / 10) % 10, next_hole % 10, H, F) == OK)
				return OK;
			else 
			{
				while (sig--)
				{
					next_hole = Delete_Num(arr);//�ҵ���һ��Ҫɾ������
					if (next_hole == 0)return OK;
					if (Dig_Hole(arr, next_hole / 100, (next_hole / 10) % 10, next_hole % 10, H, F) == OK)
						return OK;
					else return FALSE;
				}
				return FALSE;
			}
		}
	}

}
//����dpll�ж��Ƿ��н�
status resolve_sudoku(int arr[][9], headnode H, bignode  F)
{
	int c[9][9];
	for (int i = 0; i < 9; i++)//��ʼ��
		for (int j = 0; j < 9; j++)
			c[i][j] = 0;
	for (int i = 0; i < 9; i++)//����
		for (int j = 0; j < 9; j++)
			c[i][j] = arr[i][j];

	Tranlate(c, &H, F);
	for (int i = 1; i < variable + 1; i++)
		mark_ture[i] = 0;
	FLAG = 2;
	if (Dpll_youhua(H, 1, F) == OK) return OK;
	else return FALSE;
}