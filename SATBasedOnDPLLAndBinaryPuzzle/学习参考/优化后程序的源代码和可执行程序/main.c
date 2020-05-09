#include "SuDoKu.h"

//����������
int main(void)
{
	int fuben[9][9];
	for (int i = 0; i < 9; i++)//��ʼ�����������Լ��丱��
		for (int j = 0; j < 9; j++)
		{
			shuzu[i][j] = 0;
			fuben[i][j] = 0;
		}
	headnode T;
	HeadNode ARR;
	T = &ARR;
	BigNode arr;
	bignode G;
	G = &arr;
	FILE  *fp;//�ļ�ָ��
	char filename[70],filename_fuben[70];
	int op = 1,Selet_Sudoku = 1,Selet_Sat=1;
	int t1, t2,t3, isinit = 0, flag = 0, Flag = 0,sign=0,sigg=1;
	while (op) {
		system("cls");
		printf("\n               Menu for SuDoku or SAT              \n");
		printf("------------------------------------------------------\n");
		printf("            1.SAT                  2.SuDoKu           \n");
		printf("            0.Exit                                    \n");
		printf("------------------------------------------------------\n");
		if (scanf("%d", &op))
		{
			switch (op) {
			case 1:
				while (Selet_Sat) {
					system("cls");
					printf("\n              Menu for SAT                \n");
					printf("-------------------------------------------\n");
					printf("     1.ReadCnfFile           2.TraverseCnf  \n");
					printf("     3.SaveCnfFile           4.ImproveDpll   \n");
					printf("     0.Exit                                   \n");
					printf("--------------------------------------------\n");
					if (scanf("%d", &Selet_Sat))
						switch (Selet_Sat) {
						case 1:
							printf("input file name: ");
							scanf("%s", filename);
							strcpy(filename_fuben, filename);//���渱��
							if(isinit == 1)Realse_Link(&T);
							if (ReadFile(&T, G, filename) == OK)
							{
								isinit = 1;
								printf("cnf�ļ���ȡ�ɹ���\n");
							}
							else printf("cnf�ļ���ȡʧ�ܣ�\n");
							getchar(); getchar();
							break;
						case 2:
							if (isinit == 1)
							{
								Traverse(T);
								printf("cnf�ļ�����ɹ���\n");
							}
							else printf("cnf�ļ����ʧ�ܣ�\n");
							getchar(); getchar();
							break;
						case 3:
							if (isinit == 1)
							{
								if (Save_File(T) == OK)
									printf("����Ϊcnf�ļ��ɹ���\n");
								else printf("����Ϊcnf�ļ�ʧ�ܣ�\n");
							}
							else printf("����Ϊcnf�ļ�ʧ�ܣ�\n");
							getchar(); getchar();
						case 4:
							if (isinit == 1)
							{
								for (int i = 1; i < variable + 1; i++)
									mark_ture[i] = 0;
								printf("��ѡ������Ҫ�Ĳ��ԣ�\n");
								printf("1��ѡ����ִ����࣡   2:�������!\n");
								scanf("%d", &Flag);
								if (Flag == 1)//ѡ�����
								{
									FLAG = 1;
									Strategy(T, mark_1, mark_2);
									t1 = clock();
									flag = Dpll_youhua(T, 1, G);
									t2 = clock();
									t3 = (t2 - t1);
									printf("ʱ�䣺%4d ms\n", (t2 - t1));

								}
								else if (Flag == 2)
								{
									FLAG = 2;
									t1 = clock();
									flag = Dpll_youhua(T, 1, G);
									t2 = clock();
									t3 = (t2 - t1);
									printf("ʱ�䣺%4d ms\n\n", t3);
								}
								if (flag == 1)
								{
									for (int i = 0; i < 70; i++)
									{
										if (filename_fuben[i] == '.')
										{
											filename_fuben[i + 1] = 'r';
											filename_fuben[i + 2] = 'e';
											filename_fuben[i + 3] = 's';
											break;
										}
									}
									fp = fopen(filename_fuben, "wb");
									fprintf(fp, "%d\n", 1);
									printf("cnf�ļ��Ľ�Ϊ��\n");
									for (int i = 1; i < variable + 1; i++)
									{
										if (mark_ture[i] == 1)
										{
											fprintf(fp, "%d  ", i);
											printf("%6d", i);
										}
										else if (mark_ture[i] == -1)
										{
											fprintf(fp, "%d  ", -i);
											printf("%6d", -i);
										}
										if (i % 10 == 0) printf("\n");
									}
									fprintf(fp, "\n%d\n", t3);
									printf("\ncnf�Ľ�����ɹ���\n");
									fclose(fp);
								}
								else
								{
									for (int i = 0; i < 70; i++)
									{
										if (filename_fuben[i] == '.')
										{
											filename_fuben[i + 1] = 'r';
											filename_fuben[i + 2] = 'e';
											filename_fuben[i + 3] = 's';
											break;
										}
									}
									fp = fopen(filename_fuben, "wb");
									fprintf(fp, "%d\n", 0);
									fprintf(fp, "%d\n", t3);
									fclose(fp);
									printf("dpll�㷨����ʧ�ܣ�cnf�ļ��޽⣡\n");
								}
								flag = 0;
								isinit = 0;
							}
							else printf("����ʧ�ܣ�\n");
							getchar(); getchar();
							break;
						case 0:
							break;
						default:
							printf("�������\n");
							getchar(); getchar();
							break;
						}
				}
				getchar(); getchar();
				break;
			case 2:
				while (Selet_Sudoku)
				 {
					system("cls");
					printf("\n              Menu for Sudoku                \n");
					printf("-------------------------------------------\n");
					printf("     1.GenerateSuDoku    2.SolveSuDoku    \n");
					printf("     3.SaveCnfFile       0.Exit  \n");
					printf("-------------------------------------------\n");
					if (scanf("%d", &Selet_Sudoku))
						switch (Selet_Sudoku) {
						case 1:
							if (Create_sudoku(&T, G, shuzu) == OK)
							{
								for (int i = 1; i < variable + 1; i++)
									mark_ture[i] = 0;
								FLAG = 2;
								Dpll_youhua(T, 1, G);
								ShuChu_match(shuzu);
								printf("���������Ϊ��\n");
								Print_SuDoKu(shuzu);
								isinit = 1;
								sign = 1;
								printf("�������ɳɹ���\n");
							}
							else printf("��������ʧ�ܣ�\n");
							getchar(); getchar();
							break;
						case 2:
							if (sign == 1)
							{//���渱��
								for (int i = 0; i < 9; i++)
									for (int j = 0; j < 9; j++)
									{
										fuben[i][j] = shuzu[i][j];
										hole[i][j] = 0;
									}
								sigg = 1;
								while (sigg == 1)//�ж�����Ķ��������Ƿ����
								{
									printf("�����������ڵĶ�������(����Լ���Ϸˮƽ)��20-56����");
									scanf("%d", &counnt);
									if (counnt <= 56 && counnt >= 20)
										sigg = 0;
									else sigg = 1;
								}
								if (Dig_Hole(fuben, 1, 1, fuben[0][0],T,G) == OK)
									printf("�ڶ��ɹ���\n");
								else printf("�ڶ�ʧ�ܣ�\n");
								int n,m,k,t=1;
								while (t == 1)//����Ϸ�Ľ�������
								{
									int i = 1;
									system("cls");
									printf("\n      �뿪ʼ���������Ϸ��     \n");
									Print_SuDoKu(fuben);
									printf("    1:��д����      2��ɾ������     \n");
									scanf("%d", &i);
									if (i == 1)
									{
										printf("���������������λ�ã������磺1 2 ��\n");
										scanf("%d%d", &n, &m);
										printf("��ѡ�������������ֵ��1-9����");
										scanf("%d", &k);
										if (m >= 1 && m <= 9 && n >= 1 && n <= 9 && k >= 1 && k <= 9)
										{
											if (fuben[n - 1][m - 1] != 0) continue;
											else fuben[n - 1][m - 1] = k;
										}
										else
										{
											printf("�����������ȷ���룡\n");
											continue;
										}
									}
									else if (i == 2)
									{
										printf("����������ɾ����λ�ã������磺1 2 ��\n");
										scanf("%d%d", &n, &m);
										if (m >= 1 && m <= 9 && n >= 1 && n <= 9 )
										{
											if (fuben[n - 1][m - 1] == 0) continue;
											else fuben[n - 1][m - 1] = 0;
										}
										else
										{
											printf("�����������ȷ���룡\n");
											continue;
										}
									}
									printf("����1��ʾ������д������0��ʾ������д���鿴�𰸣���");
									scanf("%d", &t);
								}
								if (t == 0)//����Ӧ�ļ����.res�ļ���Ĭ���н�ʹ�
								{
									int flag = 0;
									for (int i = 0; i < 9; i++)//�ж��û��������Ƿ���ȷ
										for (int j = 0; j < 9; j++)
											if (fuben[i][j] != shuzu[i][j]) flag = 1;
									if (flag == 1)
									{
										printf("����д����������");
										printf("������ȷ�Ľ�Ϊ��\n");
									}
									else printf("������Ľ���ȷ��\n");
									Print_SuDoKu(shuzu);//�����ȷ��������
									system("pause");
								}
							}
							else printf("����ʧ�ܣ�\n");
							break;
						case 3:
							if (isinit == 1)
							{
								check(T);
								if (Save_File(T) == OK)
									printf("���ݴ����ļ�(Cnf)�ɹ���\n");
								else printf("���ݴ����ļ�(Cnf)ʧ�ܣ�\n");
							}
							else printf("����ʧ�ܣ�\n");
							getchar(); getchar();
							break;
						case 0:
							break;
						default:
							printf("�������\n");
							getchar(); getchar();
							break;
						}
				}
				getchar(); getchar();
				break;
			case 0:
				break;
			default:
				printf("�������\n");
				getchar(); getchar();
				break;
			}//end of switch
		}
		else 
		{
			printf("�������\n");
			getchar(); getchar(); getchar();
		}
	}//end of while
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
	system("pause");
	return 0;
}//end of main()
