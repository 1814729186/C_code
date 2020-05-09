#include "BinaryPuzzle1.h"

int main(void)
{
	//���ô���
	HWND hwnd = GetForegroundWindow();
	SetWindowTextA(hwnd, "DPLL");
	int _x = 800, _y = 800;
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, _x, _y, NULL);						//���ô��ڶ���
	//SetWindowRgn(hwnd, CreateRectRgn(30, 30, _x + 1300, _y + 1300),true);	//���ô��ڴ�С
	MoveWindow(hwnd, 800, 0, _x, _y, true);
	//���Ĵ�����ɫ
	system("color 70");

	char filename[150] ;	//�ļ���
	int _operate = 0;
	int _state = 0;		//��¼�˵����� 0--������  1--DPLL����  2--��������
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
	//��ȡ�ļ������ṹ
	//*************
	//�˵�
	while (1)
	{
		if (_state == 0)//���˵�
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
					printf("�����ʽ��������������!\n");
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
				printf("��ӭʹ�ã�\n");
				getchar();
				getchar();
				return 0;
			}
		}
		else if (_state == 1)	//DPLL�˵�
		{
			system("CLS");
			printf("------------------sat menu------------------\n");
			printf("1.��ȡ�ļ�ʹ�ù���1ѡȡ��֧��Ԫ����DPLL\t2.��ȡ�ļ�ʹ�ù���2ѡȡ��֧��Ԫ����DPLL\n\n3.ʹ�ù���1����20S����ù���2�ķ����������\n\n4.��յ�ǰ�ļ��ṹ\t5.��ӡCNF�ļ�\n\n0.�˳�\n");
			printf("------------------menu end------------------\n");
			printf(">");
			while (1)
			{
				status = scanf(" %d", &_operate);
				if (_operate >= 0 && _operate <= 5 && status == 1)
					break;
				else
				{
					printf("�����ʽ��������������!\n");
					getchar();
					printf(">");
				}
			}
			switch (_operate)
			{
			case 2:
				//���ļ�
				if (pcnf->sentence != NULL)
				{
					printf("��ǰcnf�ļ��ṹ�ǿգ�����պ��ٶ��롣\n");
					getchar();
					getchar();
					break;
				}
				status = readFile_1(pcnf,phc,filename);
				if (status == OK)
					printf("��ȡ�ļ���ɣ�\n�����ļ�����%d���Ӿ䣬�漰��%d������.\n", pcnf->numOfchildSentence, pcnf->numOfVariable);
				else
				{
					printf("���ļ�ʧ�ܣ�");
					getchar();
					getchar();
					continue;
				}
				if (pcnf->sentence == NULL)
				{
					printf("��ǰ�ļ��ṹΪ�գ������cnf�ļ������ԣ�\n");
				}
				printf("��ʼ����DPLL��⣡\n");
				index = 0;
				beginTime = clock();
				status = DPLL_1(pcnf, stack, phc);
				overTime = clock();
				if (status == TRUE) {
					//����ջ�����·��
					printf("cnf�����\n���·��Ϊ��\n");
					answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
					index = 0;
					destroyStack(stack, true, answer, index);
					printf("\nʱ�䣺%dms", overTime - beginTime);
					writeRes(filename,true, overTime - beginTime,answer,pcnf->numOfVariable);
					free(answer);
					answer = NULL;
				}
				else
				{
					destroyStack(stack, false, NULL, index);
					printf("�������\n");
					printf("\nʱ�䣺%dms", overTime - beginTime);
					writeRes(filename, false, overTime - beginTime, NULL, pcnf->numOfVariable);
				}
				

				resetPcnf(pcnf);
				resetCount(pcnf,phc);
				getchar();
				getchar();
				break;
			case 3:
				//��ʹ�õڶ��ּ�֦��������15s�󣬻��õ�һ�ַ����������
				if (pcnf->sentence != NULL)
				{
					printf("��ǰcnf�ļ��ṹ�ǿգ�����պ��ٶ��롣\n");
					getchar();
					getchar();
					break;
				}
				//���ļ������ǵ�ʹ�����ַ��������÷���һ�ķ�ʽ��ȡ�ļ�
				status = readFile_1(pcnf, phc, filename);
				if (status == OK)
					printf("��ȡ�ļ���ɣ�\n�����ļ�����%d���Ӿ䣬�漰��%d������.\n", pcnf->numOfchildSentence, pcnf->numOfVariable);
				else
				{
					printf("���ļ�ʧ�ܣ�");
					getchar();
					getchar();
					continue;
				}
					
				index = 0;
				printf("�Ȳ��÷������������\n");
				beginTime = clock();
				status = DPLL(pcnf, stack,beginTime,true);
				overTime = clock();
				if (status == ERROR)
				{
					//���÷���һ�������
					printf("20s stop!\n���÷���һ�������\n");
					resetPcnf(pcnf);
					destroyStack(stack,false,NULL,index);
					index = 0;
					status = DPLL_1(pcnf,stack,phc);

					overTime = clock();
					if (status == TRUE) {
						//����ջ�����·��
						printf("cnf�����\n���·��Ϊ��\n");


						answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
						index = 0;
						destroyStack(stack, true, answer, index);
						printf("\nʱ�䣺%dms", overTime - beginTime);
						writeRes(filename, true, overTime - beginTime, answer, index);
						free(answer);
						answer = NULL;

					}
					else
					{
						destroyStack(stack, false, NULL, index);
						printf("�������\n");
						printf("\nʱ�䣺%dms", overTime - beginTime);
						writeRes(filename, false, overTime - beginTime, NULL, index);
					}
				}
				else if (status == TRUE) {
					overTime = clock();
					//����ջ�����·��
					printf("cnf�����\n���·��Ϊ��\n");
					answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
					index = 0;
					destroyStack(stack, true, answer, index);
					printf("\nʱ�䣺%dms", overTime - beginTime);

					writeRes(filename, true, overTime - beginTime, answer, index);
					free(answer);
					answer = NULL;
				}
				else
				{
					overTime = clock();
					destroyStack(stack, false, NULL, index);
					printf("�������\n");
					printf("\nʱ�䣺%dms", overTime - beginTime);
					writeRes(filename, false, overTime - beginTime, NULL, index);
				}
				

				getchar();
				getchar();
				resetPcnf(pcnf);
				break;
				
				
			case 4:
				//���ٵ�ǰ���ڵĽṹ
				if (pcnf->sentence == NULL)
				{
					printf("��ǰ�����cnf�ļ��ṹΪ�գ�");
					getchar();
					printf("δ����cnf�ļ���\n");
				}
				else
				{
					//����
					destroyCNF(pcnf);
					//����count�ṹ
					if (phc != NULL)
					{
						free(phc->count);
						free(phc);
						phc = NULL;
					}
					printf("���ٳɹ�!\n");
					getchar();
					getchar();
				}
				break;
			case 1:
				//DPLL����һ
				//�����ļ�
				//���ļ�
				if (pcnf->sentence != NULL)
				{
					printf("��ǰcnf�ļ��ṹ�ǿգ�����պ��ٶ��롣\n");
					getchar();
					getchar();
					break;
				}
				status = readFile(pcnf, filename);
				if (status == OK)
					printf("��ȡ�ļ���ɣ�\n�����ļ�����%d���Ӿ䣬�漰��%d������.\n", pcnf->numOfchildSentence, pcnf->numOfVariable);
				else
				{
					printf("���ļ�ʧ�ܣ�");
					getchar();
					getchar();
					continue;
				}

				if (pcnf->sentence == NULL)
				{
					printf("��ǰ�ļ��ṹΪ�գ������cnf�ļ������ԣ�\n");
				}

				printf("��ʼ����DPLL��⣡\n");
				index = 0;
				beginTime = clock();
				status = DPLL(pcnf, stack,0,false);
				overTime = clock();
				if (status == TRUE) {
					//����ջ�����·��
					printf("cnf�����\n���·��Ϊ��\n");

					answer = (int*)malloc(sizeof(int) * (pcnf->numOfVariable));
					index = 0;
					destroyStack(stack, true, answer, index);
					printf("\nʱ�䣺%dms", overTime - beginTime);
					writeRes(filename, true, overTime - beginTime, answer, index);
					free(answer);
					answer = NULL;
				}
				else
				{
					destroyStack(stack, false, NULL, index);
					printf("�������\n");
					printf("\nʱ�䣺%dms", overTime - beginTime);
					writeRes(filename, false, overTime - beginTime, NULL, index);
				}
				

				resetPcnf(pcnf);
				getchar();
				getchar();
				break;
			case 5:
				//��ӡCNF�ļ�
				if (pcnf == NULL)
				{
					printf("��ǰCNF�ṹΪ�գ��������CNF�ļ�!\n");
				}
				else
				{
					writeSoDoKufile(pcnf);
					printf("�ļ�д��ɹ���\n");
				}
				getchar();
				getchar();
				break;
			default:
				//�˳�
				printf("�˻����˵���ͬʱ��յ�ǰCNF�ṹ��\n");
				if (pcnf != NULL)
					destroyCNF(pcnf);
				getchar();
				getchar();
				_state = 0;
			}
		}
		else if (_state == 2)		//BinaryPuzzle�˵�
		{
			system("CLS");
			printf("------------------sat menu------------------\n");
			printf("1.�����ͼ������cnf�ļ�\t2.����������ͼ�����ɺ�ɴ�ӡ�ļ���\n\n0.�˻����˵�\n");
			printf("------------------menu end------------------\n");
			printf(">");
			while (1)
			{
				status = scanf(" %d", &_operate);
				if (_operate >= 0 && _operate <= 2 && status == 1)
					break;
				else
				{
					printf("�����ʽ��������������!\n");
					getchar();
					printf(">");
				}
			}
			switch (_operate)
			{
			case 2:
				printf("��������Ҫ���ɵ������Ľ���:(4 6 8)\n>");
				scanf("%d", &_operate);
				if (!(_operate == 4 || _operate == 6 || _operate == 8))
				{
					printf("�������");
					getchar();
					getchar();
				}
				else
				{
					generateSoDuKuMap(pcnf, _operate);
				}
				break;
			case 1:	//��ӡ�����ļ�
				printConfition();
				getchar();
				getchar();
				break;
			case 0:
				_state = 0;
				printf("�˻����˵�");
				getchar();
			}
		}
	}
}