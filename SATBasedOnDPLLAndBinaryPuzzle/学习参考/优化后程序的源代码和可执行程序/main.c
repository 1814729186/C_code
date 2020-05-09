#include "SuDoKu.h"

//主函数部分
int main(void)
{
	int fuben[9][9];
	for (int i = 0; i < 9; i++)//初始化数独棋盘以及其副本
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
	FILE  *fp;//文件指针
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
							strcpy(filename_fuben, filename);//保存副本
							if(isinit == 1)Realse_Link(&T);
							if (ReadFile(&T, G, filename) == OK)
							{
								isinit = 1;
								printf("cnf文件读取成功！\n");
							}
							else printf("cnf文件读取失败！\n");
							getchar(); getchar();
							break;
						case 2:
							if (isinit == 1)
							{
								Traverse(T);
								printf("cnf文件输出成功！\n");
							}
							else printf("cnf文件输出失败！\n");
							getchar(); getchar();
							break;
						case 3:
							if (isinit == 1)
							{
								if (Save_File(T) == OK)
									printf("保存为cnf文件成功！\n");
								else printf("保存为cnf文件失败！\n");
							}
							else printf("保存为cnf文件失败！\n");
							getchar(); getchar();
						case 4:
							if (isinit == 1)
							{
								for (int i = 1; i < variable + 1; i++)
									mark_ture[i] = 0;
								printf("请选择你需要的策略：\n");
								printf("1：选择出现次数多！   2:暴力求解!\n");
								scanf("%d", &Flag);
								if (Flag == 1)//选择策略
								{
									FLAG = 1;
									Strategy(T, mark_1, mark_2);
									t1 = clock();
									flag = Dpll_youhua(T, 1, G);
									t2 = clock();
									t3 = (t2 - t1);
									printf("时间：%4d ms\n", (t2 - t1));

								}
								else if (Flag == 2)
								{
									FLAG = 2;
									t1 = clock();
									flag = Dpll_youhua(T, 1, G);
									t2 = clock();
									t3 = (t2 - t1);
									printf("时间：%4d ms\n\n", t3);
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
									printf("cnf文件的解为：\n");
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
									printf("\ncnf的解输出成功！\n");
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
									printf("dpll算法处理失败，cnf文件无解！\n");
								}
								flag = 0;
								isinit = 0;
							}
							else printf("操作失败！\n");
							getchar(); getchar();
							break;
						case 0:
							break;
						default:
							printf("输入错误！\n");
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
								printf("求解后的棋盘为：\n");
								Print_SuDoKu(shuzu);
								isinit = 1;
								sign = 1;
								printf("数独生成成功！\n");
							}
							else printf("数独生成失败！\n");
							getchar(); getchar();
							break;
						case 2:
							if (sign == 1)
							{//保存副本
								for (int i = 0; i < 9; i++)
									for (int j = 0; j < 9; j++)
									{
										fuben[i][j] = shuzu[i][j];
										hole[i][j] = 0;
									}
								sigg = 1;
								while (sigg == 1)//判断输入的洞的数量是否合理
								{
									printf("请输入你想挖的洞的数量(结合自己游戏水平)（20-56）：");
									scanf("%d", &counnt);
									if (counnt <= 56 && counnt >= 20)
										sigg = 0;
									else sigg = 1;
								}
								if (Dig_Hole(fuben, 1, 1, fuben[0][0],T,G) == OK)
									printf("挖洞成功！\n");
								else printf("挖洞失败！\n");
								int n,m,k,t=1;
								while (t == 1)//玩游戏的交互过程
								{
									int i = 1;
									system("cls");
									printf("\n      请开始你的数独游戏！     \n");
									Print_SuDoKu(fuben);
									printf("    1:填写数字      2：删除数字     \n");
									scanf("%d", &i);
									if (i == 1)
									{
										printf("请输入你想填入的位置：（例如：1 2 ）\n");
										scanf("%d%d", &n, &m);
										printf("请选择你想填入的数值（1-9）：");
										scanf("%d", &k);
										if (m >= 1 && m <= 9 && n >= 1 && n <= 9 && k >= 1 && k <= 9)
										{
											if (fuben[n - 1][m - 1] != 0) continue;
											else fuben[n - 1][m - 1] = k;
										}
										else
										{
											printf("输入错误！请正确输入！\n");
											continue;
										}
									}
									else if (i == 2)
									{
										printf("请输入你想删除的位置：（例如：1 2 ）\n");
										scanf("%d%d", &n, &m);
										if (m >= 1 && m <= 9 && n >= 1 && n <= 9 )
										{
											if (fuben[n - 1][m - 1] == 0) continue;
											else fuben[n - 1][m - 1] = 0;
										}
										else
										{
											printf("输入错误！请正确输入！\n");
											continue;
										}
									}
									printf("输入1表示继续填写，输入0表示放弃填写并查看答案！：");
									scanf("%d", &t);
								}
								if (t == 0)//将对应文件存成.res文件，默认有解就存
								{
									int flag = 0;
									for (int i = 0; i < 9; i++)//判断用户的填入是否正确
										for (int j = 0; j < 9; j++)
											if (fuben[i][j] != shuzu[i][j]) flag = 1;
									if (flag == 1)
									{
										printf("你填写的数独错误！");
										printf("数独正确的解为：\n");
									}
									else printf("你填入的解正确！\n");
									Print_SuDoKu(shuzu);//输出正确的数独解
									system("pause");
								}
							}
							else printf("操作失败！\n");
							break;
						case 3:
							if (isinit == 1)
							{
								check(T);
								if (Save_File(T) == OK)
									printf("数据存入文件(Cnf)成功！\n");
								else printf("数据存入文件(Cnf)失败！\n");
							}
							else printf("操作失败！\n");
							getchar(); getchar();
							break;
						case 0:
							break;
						default:
							printf("输入错误！\n");
							getchar(); getchar();
							break;
						}
				}
				getchar(); getchar();
				break;
			case 0:
				break;
			default:
				printf("输入错误！\n");
				getchar(); getchar();
				break;
			}//end of switch
		}
		else 
		{
			printf("输入错误！\n");
			getchar(); getchar(); getchar();
		}
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	system("pause");
	return 0;
}//end of main()
