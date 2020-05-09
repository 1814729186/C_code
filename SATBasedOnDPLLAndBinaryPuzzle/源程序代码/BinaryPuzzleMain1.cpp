#include "BinaryPuzzle1.h"

int main(void)
{
	//设置窗口
	HWND hwnd = GetForegroundWindow();
	SetWindowTextA(hwnd, "DPLL");
	int _x = 800, _y = 800;
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, _x, _y, NULL);						//设置窗口顶置
	//SetWindowRgn(hwnd, CreateRectRgn(30, 30, _x + 1300, _y + 1300),true);	//设置窗口大小
	MoveWindow(hwnd, 800, 0, _x, _y, true);
	//更改窗口颜色
	system("color 70");

	char filename[150] ;	//文件名
	int _operate = 0;
	int _state = 0;		//记录菜单界面 0--主界面  1--DPLL界面  2--数独界面
	int beginTime, overTime;
	pstack stack = NULL;
	cnfFile cnf;
	pHeadCount phc = NULL;
	Status status = 0;
	cnf.numOfchildSentence = 0;
	cnf.numOfVariable = 0;
	cnf.sentence = NULL;
	pCnfFile pcnf = &cnf;
	int index = 0;
	int* answer = NULL;
	//读取文件创建结构
	//*************
	//菜单
	while (1)
	{
		if (_state == 0)//主菜单
		{
			system("CLS");
			printf("------------------sat menu------------------\n");
			printf("1.SAT\t2.Binary Puzzle\n\n0.exit\n");
			printf("------------------menu end------------------\n");
			while (1)
			{
				status = scanf(" %d", &_operate);
				if (_operate >= 0 && _operate <= 2&&status == 1)
					break;
				else
				{
					printf("输入格式错误！请重新输入!\n");
					getchar();
					printf(">");
				}
			}
			switch (_operate)
			{
			case 1:
				_state = 1;
				break;
			case 2:
				_state = 2;
				break;
			default:
				printf("欢迎使用！\n");
				getchar();
				getchar();
				return 0;
			}
		}
		else if (_state == 1)	//DPLL菜单
		{
			system("CLS");
			printf("------------------sat menu------------------\n");
			printf("1.读取文件使用规则1选取分支变元进行DPLL\t2.读取文件使用规则2选取分支变元进行DPLL\n\n3.使用规则1超过20S后采用规则2的方法进行求解\n\n4.清空当前文件结构\t5.打印CNF文件\n\n0.退出\n");
			printf("------------------menu end------------------\n");
			printf(">");
			while (1)
			{
				status = scanf(" %d", &_operate);
				if (_operate >= 0 && _operate <= 5 && status == 1)
					break;
				else
				{
					printf("输入格式错误！请重新输入!\n");
					getchar();
					printf(">");
				}
			}
			switch (_operate)
			{
			case 2:
				//读文件
				if (pcnf->sentence != NULL)
				{
					printf("当前cnf文件结构非空，请清空后再读入。\n");
					getchar();
					getchar();
					break;
				}
				status = readFile_1(pcnf,phc,filename);
				if (status == OK)
					printf("读取文件完成！\n所读文件共有%d个子句，涉及到%d种文字.\n", pcnf->numOfchildSentence, pcnf->numOfVariable);
				else
				{
					printf("打开文件失败！");
					getchar();
					getchar();
					continue;
				}
				if (pcnf->sentence == NULL)
				{
					printf("当前文件结构为空，请读入cnf文件后再试！\n");
				}
				printf("开始进行DPLL求解！\n");
				index = 0;
				beginTime = clock();
				status = DPLL_1(pcnf, stack, phc);
				overTime = clock();
				if (status == TRUE) {
					//销毁栈并输出路径
					printf("cnf能求解\n求解路径为：\n");
					answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
					index = 0;
					destroyStack(stack, true, answer, index);
					printf("\n时间：%dms", overTime - beginTime);
					writeRes(filename,true, overTime - beginTime,answer,pcnf->numOfVariable);
					free(answer);
					answer = NULL;
				}
				else
				{
					destroyStack(stack, false, NULL, index);
					printf("不能求解\n");
					printf("\n时间：%dms", overTime - beginTime);
					writeRes(filename, false, overTime - beginTime, NULL, pcnf->numOfVariable);
				}
				

				resetPcnf(pcnf);
				resetCount(pcnf,phc);
				getchar();
				getchar();
				break;
			case 3:
				//在使用第二种剪枝方法超过15s后，换用第一种方法进行求解
				if (pcnf->sentence != NULL)
				{
					printf("当前cnf文件结构非空，请清空后再读入。\n");
					getchar();
					getchar();
					break;
				}
				//读文件，考虑到使用两种方法，采用方法一的方式读取文件
				status = readFile_1(pcnf, phc, filename);
				if (status == OK)
					printf("读取文件完成！\n所读文件共有%d个子句，涉及到%d种文字.\n", pcnf->numOfchildSentence, pcnf->numOfVariable);
				else
				{
					printf("打开文件失败！");
					getchar();
					getchar();
					continue;
				}
					
				index = 0;
				printf("先采用方法二进行求解\n");
				beginTime = clock();
				status = DPLL(pcnf, stack,beginTime,true);
				overTime = clock();
				if (status == ERROR)
				{
					//换用方法一进行求解
					printf("20s stop!\n换用方法一进行求解\n");
					resetPcnf(pcnf);
					destroyStack(stack,false,NULL,index);
					index = 0;
					status = DPLL_1(pcnf,stack,phc);

					overTime = clock();
					if (status == TRUE) {
						//销毁栈并输出路径
						printf("cnf能求解\n求解路径为：\n");


						answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
						index = 0;
						destroyStack(stack, true, answer, index);
						printf("\n时间：%dms", overTime - beginTime);
						writeRes(filename, true, overTime - beginTime, answer, index);
						free(answer);
						answer = NULL;

					}
					else
					{
						destroyStack(stack, false, NULL, index);
						printf("不能求解\n");
						printf("\n时间：%dms", overTime - beginTime);
						writeRes(filename, false, overTime - beginTime, NULL, index);
					}
				}
				else if (status == TRUE) {
					overTime = clock();
					//销毁栈并输出路径
					printf("cnf能求解\n求解路径为：\n");
					answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
					index = 0;
					destroyStack(stack, true, answer, index);
					printf("\n时间：%dms", overTime - beginTime);

					writeRes(filename, true, overTime - beginTime, answer, index);
					free(answer);
					answer = NULL;
				}
				else
				{
					overTime = clock();
					destroyStack(stack, false, NULL, index);
					printf("不能求解\n");
					printf("\n时间：%dms", overTime - beginTime);
					writeRes(filename, false, overTime - beginTime, NULL, index);
				}
				

				getchar();
				getchar();
				resetPcnf(pcnf);
				break;
				
				
			case 4:
				//销毁当前存在的结构
				if (pcnf->sentence == NULL)
				{
					printf("当前程序的cnf文件结构为空！");
					getchar();
					printf("未读入cnf文件！\n");
				}
				else
				{
					//销毁
					destroyCNF(pcnf);
					//销毁count结构
					if (phc != NULL)
					{
						free(phc->count);
						free(phc);
						phc = NULL;
					}
					printf("销毁成功!\n");
					getchar();
					getchar();
				}
				break;
			case 1:
				//DPLL方法一
				//读入文件
				//读文件
				if (pcnf->sentence != NULL)
				{
					printf("当前cnf文件结构非空，请清空后再读入。\n");
					getchar();
					getchar();
					break;
				}
				status = readFile(pcnf, filename);
				if (status == OK)
					printf("读取文件完成！\n所读文件共有%d个子句，涉及到%d种文字.\n", pcnf->numOfchildSentence, pcnf->numOfVariable);
				else
				{
					printf("打开文件失败！");
					getchar();
					getchar();
					continue;
				}

				if (pcnf->sentence == NULL)
				{
					printf("当前文件结构为空，请读入cnf文件后再试！\n");
				}

				printf("开始进行DPLL求解！\n");
				index = 0;
				beginTime = clock();
				status = DPLL(pcnf, stack,0,false);
				overTime = clock();
				if (status == TRUE) {
					//销毁栈并输出路径
					printf("cnf能求解\n求解路径为：\n");

					answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
					index = 0;
					destroyStack(stack, true, answer, index);
					printf("\n时间：%dms", overTime - beginTime);
					writeRes(filename, true, overTime - beginTime, answer, index);
					free(answer);
					answer = NULL;
				}
				else
				{
					destroyStack(stack, false, NULL, index);
					printf("不能求解\n");
					printf("\n时间：%dms", overTime - beginTime);
					writeRes(filename, false, overTime - beginTime, NULL, index);
				}
				

				resetPcnf(pcnf);
				getchar();
				getchar();
				break;
			case 5:
				//打印CNF文件
				if (pcnf == NULL)
				{
					printf("当前CNF结构为空，不能输出CNF文件!\n");
				}
				else
				{
					writeSoDoKufile(pcnf);
					printf("文件写入成功！\n");
				}
				getchar();
				getchar();
				break;
			default:
				//退出
				printf("退回主菜单，同时清空当前CNF结构！\n");
				if (pcnf != NULL)
					destroyCNF(pcnf);
				getchar();
				getchar();
				_state = 0;
			}
		}
		else if (_state == 2)		//BinaryPuzzle菜单
		{
			system("CLS");
			printf("------------------sat menu------------------\n");
			printf("1.输出地图条件的cnf文件\t2.生成数独地图（生成后可打印文件）\n\n0.退回主菜单\n");
			printf("------------------menu end------------------\n");
			printf(">");
			while (1)
			{
				status = scanf(" %d", &_operate);
				if (_operate >= 0 && _operate <= 2 && status == 1)
					break;
				else
				{
					printf("输入格式错误！请重新输入!\n");
					getchar();
					printf(">");
				}
			}
			switch (_operate)
			{
			case 2:
				printf("请输入想要生成的数独的阶数:(4 6 8)\n>");
				scanf("%d", &_operate);
				if (!(_operate == 4 || _operate == 6 || _operate == 8))
				{
					printf("输入错误！");
					getchar();
					getchar();
				}
				else
				{
					generateSoDuKuMap(pcnf, _operate);
				}
				break;
			case 1:	//打印配置文件
				printConfition();
				getchar();
				getchar();
				break;
			case 0:
				_state = 0;
				printf("退回主菜单");
				getchar();
			}
		}
	}
}