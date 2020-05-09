#include "BinaryPuzzle1.h"

//����������ͼ
//ʹ���ڶ�������������ͼ
//�������������������һЩ�ո񣬶�����������⣬���н⣬�������ϵ�����Ϊ������ʹ���ڶ����������������޽⣬����������
//4������--�������2��λ�õ���
//6������--�������3��λ�õ���
//8������--�������4��λ�õ���		
void generateSoDuKuMap(pCnfFile pcnf, int n)	//n��ʾ����
{
	if (pcnf->sentence != NULL)
	{
		printf("��ǰpcnf�ļ��ṹ�ǿգ�����պ����ԣ�\n");
		getchar();
		getchar();
		return;
	}
	bool* map = (bool*)malloc(sizeof(bool)*n*n);		//���浱ǰ������ͼ
	int numOfBlank = 0;		//�����������
	int location = 0;				//λ��
	//����õ���Ҫ����ĸ���
	if (n == 4)
	{
		numOfBlank = 2;
	}
	else if (n == 6)
	{
		numOfBlank = 3;
	}
	else if (n == 8)
	{
		numOfBlank = 4;
	}
	else exit(-1);	//�����ϲ���
	//��pcnf��ȡ���������ļ��������ļ�������ʱ���Ѵ��������ļ�
	char addrFilename[200] = ADDR;
	strcat(addrFilename, "BinaryPuzzleConditionFile");
	char ch[2] = "4";
	ch[0] = n + '0';
	strcat(addrFilename, ch);
	FILE* file;
	pstack sta = NULL;
	int *saveAnwser = (int *)malloc(sizeof(int)*(n*n));
	int num;		//���ڽ�����������ж��������������
	int index = 0;
	//
	while (1)
	{
		index = 0;
		//��ʼ��
		for (int i = 0; i < n*n; i++)
		{
			saveAnwser[i] = 0;
		}
		file = fopen(addrFilename, "r");
		if (file == NULL)//���ļ�ʧ��
		{
			printf("�����ļ������ڣ������������ļ������ԡ�\n");
			getchar();
			getchar();
			return;
		}

		//��ȡ�ļ�
		readAndCreate(pcnf, file);
		fclose(file);

		//�����λ�����������
		for (int i = 0; i < numOfBlank; i++)
		{
			//�������λ��
			location = rand() % (n*n) + 1;
			//�����λ�ô��뵽pcnf��ȥ
			num = rand() % 2;
			if (num == 1) location = (-1)*location;
			addNodeToPcnf(pcnf, location);
		}

		if (DPLL(pcnf, sta,0,false) == TRUE) break;		//�ɹ����ɽ�
		//����pcnf�ṹ�����¶�ȡ
		destroyCNF(pcnf);
		destroyStack(sta, false, NULL, index);
		sta = NULL;
		pcnf = NULL;
	}
	//��������浽������ȥ
	destroyStack(sta, false, saveAnwser, index);

	//��������pcnf�ṹ
	destroyCNF(pcnf);
	file = fopen(addrFilename, "r");
	if (file == NULL)//���ļ�ʧ��
	{
		printf("�����ļ������ڣ������������ļ������ԡ�\n");
		getchar();
		getchar();
		return;
	}
	//��ȡ�ļ�
	readAndCreate(pcnf, file);
	fclose(file);

	//����õĽ����cnf
	for (int i = 0; i < pcnf->numOfVariable; i++)
	{
		if (saveAnwser[i] == 0) exit(-1);
		addNodeToPcnf(pcnf, saveAnwser[i]);
	}
	//���Ƶ�ǰ������Ϊ�ڶ�������

	int *digHole = (int*)malloc(sizeof(int)*(n*n));
	for (int i = 0; i < (n*n); i++)
	{
		digHole[i] = saveAnwser[i];
	}
	printf("������Ҫ�����ڿյĸ�������ʵ���γɵĿո����û��ϣ������ô�ࣩ\n");
	scanf("%d", &numOfBlank);
	digMap(pcnf, digHole, numOfBlank, n);
	//���ɳɹ�
	//��ӡ������ͼ
	printMap(digHole, saveAnwser, n);
	printf("�Ƿ񴢴�CNF�ļ�����1-- ���� ������-- ������\n>");
	int op = -1;
	scanf("%d", &op);
	if (op == 1)
	{
		writeSoDoKufile(pcnf);
	}
	//��յ�ǰcnf�ļ��ṹ
	destroyCNF(pcnf);
	return;
}

//д���ļ�
Status writeSoDoKufile(pCnfFile pcnf)
{

	char saveName[30];
	char addr[100] = ADDR;
	printf("�������ļ�����\n>");
	scanf("%s", saveName);
	FILE *file = fopen(strcat(addr, saveName), "w");
	if (file == NULL)
	{
		printf("���ļ�ʧ�ܣ�");
		getchar();
		getchar();
		exit(-1);
	}
	fprintf(file, "p cnf %d %d\n", pcnf->numOfVariable, pcnf->numOfchildSentence);
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	while (pcs != NULL)
	{
		if (pcs->isCunzai == true)
		{
			pw = pcs->words;
			while (pw != NULL)
			{
				if (pw->mark == true)
					fprintf(file, "%d ", pw->varibale);
				pw = pw->nextword;
			}
			fprintf(file, "0\n");
		}
		pcs = pcs->nextSen;
	}
	fclose(file);
	return OK;
}

void printMap(int *digHole, int *answer, int n)
{
	int *map = (int*)malloc(sizeof(int)*(n*n + 1));
	//-1��ʾ�ո�
	//��ʼ��
	//for (int i = 0; i < n*n; i++)
	//{
	//	printf("%d ", digHole[i]);
	//}
	//printf("\n");
	//for (int i = 0; i < n*n; i++)
	//{
	//	printf("%d ", answer[i]);
	//}
	//printf("\n");
	for (int i = 0; i < n*n + 1; i++)
	{
		map[i] = -1;
	}
	//��ӡδ����map
	for (int i = 0; i < n*n; i++)
	{
		//��������
		if (digHole[i] != 0)
		{
			if (digHole[i] > 0)
			{
				if (map[digHole[i]] != -1)
				{
					printf("ERROR!");
					exit(-1);
				}
				map[digHole[i]] = 1;
			}
			else
			{
				if (map[-digHole[i]] != -1)
				{
					printf("ERROR!");
					exit(-1);
				}
				map[-digHole[i]] = 0;
			}
		}
	}
	printf("���ɵ���������\n");
	printf("press enter...\n");
	getchar();
	getchar();
	system("cls");
	
	for (int i = 0; i < n; i++)
		printf("--");
	printf("\n");
	for (int i = 1; i <= n * n; i++)
	{
		if (map[i] == 1)
			printf("1 ");
		else if (map[i] == 0)
			printf("0 ");
		else if (map[i] == -1)
			printf("- ");
		else
		{
			printf("error!");
			getchar();
			exit(-1);
		}
		if ((i % n) == 0) printf("|%2d\n", i / n);
	}
	for (int i = 0; i < n; i++)
		printf("--");
	printf("\n");
	for (int i = 0; i < n; i++)
		printf("%-2d", i + 1);
	printf("\n");
	printf("���������...\n");
	getchar();
	getchar();
	//��ӡ���ĺ������
	for (int i = 0; i < n; i++)
		printf("--");
	printf("\n");
	//��ʼ��
	for (int i = 0; i < n*n + 1; i++)
	{
		map[i] = -1;
	}
	/*for (int i = 0; i < n*n + 1; i++)
	{
		printf("%d ", answer[i]);
	}
	printf("\n");*/
	for (int i = 0; i < n*n; i++)
	{
		//��������
		if (answer[i] == 0)
		{
			printf("error!");
			getchar();
			exit(-1);
		}
		if (answer[i] < 0)
		{
			if (map[-answer[i]] != -1)
			{
				printf("error!");
				getchar();
				exit(-1);
			}
			map[-answer[i]] = 0;
		}
		else
		{
			if (map[answer[i]] != -1)
			{
				printf("error!");
				getchar();
				exit(-1);
			}
			map[answer[i]] = 1;
		}

	}
	for (int i = 1; i <= n * n; i++)
	{
		if (map[i] == 1)
			printf("1 ");
		else if (map[i] == 0)
			printf("0 ");
		else
		{
			printf("error!");
			getchar();
			exit(-1);
		}
		if (i % n == 0) printf("|\n");
	}
}

//�ڶ�����������ͼ,�������ɵĿյĸ���,n��ʾ����
void digMap(pCnfFile pcnf, int *digHole, int numOfHole, int n)
{
	int num = 1;
	int i = 1;
	Status status;
	//���׸�λ�ÿ�ʼ��������ģ��鿴�Ƿ��н�
	while (i <= numOfHole)
	{
		num = digHole[i];
		status = changeAndDPLL(pcnf, num, n);
		if (status == FALSE)	//DPLLʧ�ܣ���ɾ���ý��
		{
			//������digHole�н��ý���ֵɾ��
			for (int i = 0; i < n*n; i++)
			{
				if (digHole[i] == num || digHole[i] == (-1)*num)
				{
					digHole[i] = 0;
					break;
				}
			}

		}
		i++;
		if (i >= n * n)
			break;				//�ﵽ�����λ�ô�,����ɾ��num��ô�����
	}
}

//�����Ϊnum��λ����Ϊ���෴�����鿴�Ƿ��н�,nΪ����
Status changeAndDPLL(pCnfFile pcnf, int num, int n)
{
	Status status;		//����DPLL��������ֵ
	//����num��-num
	pstack sta = NULL;
	pChildSentence pcs = NULL;
	int changeNum = 0;
	bool bol = false;
	pcs = findNum(pcnf, num, bol);
	if (pcs == NULL)
	{
		printf("ERROR");
		exit(-1);
	}
	//��pcnf�е�num������Ϊ-num
	if (bol == true) changeNum = (-1)*num;
	else changeNum = num;
	pcs->words->varibale = changeNum;

	//DPLL����Ƿ��н�
	status = DPLL(pcnf, sta,0,false);
	//�����ָ�ԭ״
	int _index = 0;
	if (bol == false)	pcs->words->varibale = (-1)*num;
	else pcs->words->varibale = num;
	destroyStack(sta, false, NULL, _index);
	//�н���֤���ý�㲻��ɾ��������TRUE
	if (status == TRUE)
	{
		//��cnf�ṹ����
		resetPcnf(pcnf);
		return TRUE;
	}
	//�޽⽫����mark��Ϊ-(n*n+1),�ҽ�isCunzai��Ϊfalse ��˵���ý���Ѿ���ɾ��������FALSE
	if (status == FALSE)
	{
		pcs->isCunzai = false;
		pcs->mark = n * n + 1;
		pcnf->numOfchildSentence--;
		//��cnf�ṹ����
		resetPcnf(pcnf);
		return FALSE;
	}
}

//����pcnf��Ϊ���Ӿ�num����-num�Ľ�㣬�ҵ��󷵻�ָ����Ӿ��ָ�룬δ�ҵ��򷵻�NULL,��bol��¼��λ�õ���������
pChildSentence findNum(pCnfFile pcnf, int num, bool &bol)
{
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	while (pcs != NULL)
	{
		if (pcs->mark == 0 && pcs->numOfword == 1)	//���Ӿ�
		{

			pw = pcs->words;
			if (pw->varibale == num)
			{
				bol = true;
				return pcs;
			}
			if (pw->varibale == (-1)*num)
			{
				bol = false;
				return pcs;
			}
		}
		pcs = pcs->nextSen;
	}
	exit(-1);
}


void addNodeToPcnf(pCnfFile pcnf, int word)
{
	pChildSentence pcs = pcnf->sentence;
	while (1)
	{
		if (pcs->nextSen == NULL)
			break;
		pcs = pcs->nextSen;
	}
	//�ҵ����һ���Ӿ��λ��
	pcs->nextSen = (pChildSentence)malloc(sizeof(childSentence));
	if (pcs->nextSen == NULL)	exit(-1);
	pcs = pcs->nextSen;
	pcs->isCunzai = true;
	pcs->mark = 0;
	pcs->numOfword = 1;
	pcs->nextSen = NULL;
	pcs->words = (pWord)malloc(sizeof(word));
	if (pcs->words == NULL)	exit(-1);
	pcs->words->mark = true;
	pcs->words->nextword = NULL;
	pcs->words->varibale = word;
	pcnf->numOfchildSentence++;
	return;

}

//(1)����1��ͬ�С�ͬ�в�������������ͬ
void printCondition1(FILE * file, int n)
{
	//ͬ��
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 2; j++)
		{
			fprintf(file, "%d %d %d 0\n", n * i + j + 1, n * i + j + 2, n * i + j + 3);
			fprintf(file, "%d %d %d 0\n", -1 * (n * i + j + 1), -1 * (n * i + j + 2), -1 * (n * i + j + 3));
		}
	}
	//ͬ��
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 2; j++)
		{
			fprintf(file, "%d %d %d 0\n", i + n * j + 1, i + n * j + 1 + n, i + n * j + 1 + n + n);
			fprintf(file, "%d %d %d 0\n", -1 * (i + n * j + 1), -1 * (i + n * j + 1 + n), -1 * (i + n * j + 1 + n + n));
		}
	}
	return;
}

//(2)����2��ͬһ�л�ͬһ��0��1������ͬ������n/2 +1������ͬ
void printCondition2(FILE*file, int n)
{
	int len = 0;
	int cur_index;
	//����1~n�е�n/2+1������������������鱣��
	bool *bol = generateCombination(n, len);
	//ͨ������ѡ�������
	for (int i = 0; i < len; i++)
	{
		//���Ϊi��n��bool��ɵ�������׸�bool���±�Ϊ��n*i

		//ͬһ��
		for (int j = 0; j < n; j++)
		{
			//�������µ�n��
			//���Ϊj���п�ʼ��λ�ñ��Ϊ��1+n*j
			//��
			cur_index = 1 + n * j;
			for (int k = n * i; k < n*i + n; k++, cur_index++)
			{
				if (bol[k] == true)
				{
					fprintf(file, "%d ", cur_index);
				}
			}
			fprintf(file, "0\n");
			//��
			cur_index = 1 + n * j;
			for (int k = n * i; k < n*i + n; k++, cur_index++)
			{
				if (bol[k] == true)
				{
					fprintf(file, "%d ", -cur_index);
				}
			}
			fprintf(file, "0\n");
		}
		//ͬһ��
		for (int j = 0; j < n; j++)
		{
			//���Ϊj���п�ʼ���׸�λ��Ϊj+1��ÿ������n
			//��
			cur_index = j + 1;
			for (int k = n * i; k < n*i + n; k++, cur_index += n)
			{
				if (bol[k] == true)
				{
					fprintf(file, "%d ", cur_index);
				}
			}
			fprintf(file, "0\n");
			cur_index = j + 1;
			for (int k = n * i; k < n*i + n; k++, cur_index += n)
			{
				if (bol[k] == true)
				{
					fprintf(file, "%d ", -cur_index);
				}
			}
			fprintf(file, "0\n");
		}
	}
	return;
}

//��3������3���������в���ͬ
void printCondition3(FILE*file, int n)
{
	//�õ�C��n,2��
	int sumSeclectLine = 0;		//ѡ�����еĲ�ͬѡ�������
	bool *sceLine = printCn2(n, sumSeclectLine);
	int line1 = 0, line2 = 0;		//��ǰ���������е��ױ��
	int row1 = 0, row2 = 0;		//��ǰ�������е��ױ��
	//�õ���ͬ�������
	int sumOfzuhe = 0;
	bool *zuhe = generateZuhe(n, sumOfzuhe);

	for (int i = 0; i < sumSeclectLine; i++)
	{
		//���Ϊi��һ��c(n,2)��Ͽ�ʼ��λ��Ϊ:n*i
		//��ѡ����ѡ��
		line1 = 0; line2 = 0;//��ʼ��
		for (int j = n * i; j < n*i + n; j++)
		{
			if (sceLine[j] == true)
			{
				if (line1 == 0)
					line1 = (j - n * i)*n + 1;
				else
					line2 = (j - n * i)*n + 1;
			}
		}
		//�����ѡ��
		//����ѡ����н��в���
		for (int k = 0; k < sumOfzuhe; k++)
		{
			//���Ϊk��zuhe��ʼ������Ϊk*n,������k*(n+1)-1
			for (int v = k * n; v < k*n + n; v++)
			{
				if (zuhe[v] == true)
				{
					fprintf(file, "%d %d ", line1 + v - k * n, line2 + v - k * n);
				}
				else
				{
					fprintf(file, "%d %d ", -(line1 + v - k * n), -(line2 + v - k * n));
				}

			}
			fprintf(file, "0\n");
		}
		//������������
		row1 = (line1 / n) + 1;
		row2 = (line2 / n) + 1;
		//����ѡ����н��в���
		for (int k = 0; k < sumOfzuhe; k++)
		{
			//���Ϊk��zuhe��ʼ������Ϊk*n,������k*(n+1)-1
			for (int v = k * n; v < k*n + n; v++)
			{
				if (zuhe[v] == true)
				{
					fprintf(file, "%d %d ", row1 + n * (v - k * n), row2 + n * (v - k * n));
				}
				else
				{
					fprintf(file, "%d %d ", -(row1 + n * (v - k * n)), -(row2 + n * (v - k * n)));
				}

			}
			fprintf(file, "0\n");
		}
	}
	return;
}
void printConfition(void)
{
	char* name = (char*)malloc(sizeof(char) * 30);
	int n = 0;	//�����Ľ���
	int maxLen = 1;
	int sumOfChildScentence = 0;	//�Ӿ�����

	printf("����������������Ľ���:(4 6 8)\n>");
	getchar();
	do
	{
		scanf("%d", &n);
	} while (n != 4 && n != 6 && n != 8);
	if (n == 4)	sumOfChildScentence = 288;
	else if (n == 6) sumOfChildScentence = 2376;
	else if (n == 8) sumOfChildScentence = 16320;
	else exit(-1);
	char addrFilename[200] = ADDR;
	strcat(addrFilename, "BinaryPuzzleConditionFile");
	char ch[2] = "4";
	ch[0] = n + '0';
	strcat(addrFilename, ch);
	FILE* file;
	//���ļ�
	if (!(file = fopen(addrFilename, "w")))
	{
		exit(-3);
	}
	fprintf(file, "c BinaryPuzzleContinue %d*%d\n", n, n);
	fprintf(file, "p cnf %d %d\n", n*n, sumOfChildScentence);
	printCondition1(file, n);
	printCondition2(file, n);
	printCondition3(file, n);
	fclose(file);
	printf("�ļ�%s�Ѵ���\n", addrFilename);
	return;

}


//����1~n�е�n/2+1������������������鱣��
bool* generateCombination(int n, int &len)
{
	if (n == 8)
		len = 56;
	else if (n == 6)
		len = 15;
	else if (n == 4)
		len = 4;
	else
		return NULL;//�������
	bool *combi = (bool*)malloc(sizeof(bool) * n*len);		//n == 8
	bool buffer[8];	//������
	int cur_num;	//��ǰ������
	//�����
	int length = 1;
	int curLen = 0;
	for (int i = 0; i < n; i++)
		length *= 2;				//nλ��������������
	for (int i = 0; i < length; i++)
	{

		cur_num = i;
		//����buffer
		for (int j = 0; j < 8; j++)
			buffer[j] = false;
		//����ȡ�෨�����������
		for (int j = 0; j < n; j++)
		{
			if (cur_num == 0)	break;		//��ǰ�������Ѿ�Ϊ0���������������ȥ
			if (cur_num % 2 == 1)
				buffer[j] = true;
			cur_num /= 2;
		}
		//��ö������룬�ж϶����������м���true���� (n/2)+1��1��ȡ�ô���Ŀ��������ȥ
		if (countNumOfTrue(buffer, n) == (n / 2) + 1)
		{
			for (int j = 0; j < n; j++)
			{
				combi[curLen * n + j] = buffer[j];
			}
			curLen++;			//��ǰλ��
			if (curLen > len)
				exit(-6);		//������������
		}
	}
	return combi;
}
int countNumOfTrue(bool bol[], int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		if (bol[i] == true)
			sum++;
	}
	return sum;
}
//����00000000~11111111�е�������
bool* generateZuhe(int n, int &len)
{
	int length = 1;
	for (int i = 0; i < n; i++)
		length *= 2;
	len = length;
	int cur_num;
	//��������ռ�
	bool *zuHe = (bool*)malloc(sizeof(bool) * n * length);
	//��ʼ��
	for (int i = 0; i < length; i++)
		zuHe[i] = false;
	//����ȡ�෨�г�00000000~11111111�е�������
	for (int i = 0; i < length; i++)
	{
		//����ȡ�෨
		cur_num = i;
		for (int j = 0; j < n; j++)
		{
			if (cur_num == 0)	break;
			if (cur_num % 2 == 1)
				zuHe[j + n * i] = true;	//��i�����洢��λ��
			cur_num /= 2;
		}
	}
	return zuHe;
}

bool* printCn2(int n, int &len)
{
	int cur_num = 0;
	len = (n*(n - 1)) / 2;
	int curLen = 0;
	bool *bol = (bool*)malloc(sizeof(bool)*n*len);
	int maxLen = 1;
	bool buffer[8];
	for (int i = 0; i < n; i++)
	{
		maxLen *= 2;
	}
	for (int i = 0; i < maxLen; i++)
	{
		cur_num = i;
		//����buffer
		for (int j = 0; j < 8; j++)
			buffer[j] = false;
		for (int j = 0; j < n; j++)
		{
			if (cur_num == 0)	break;
			if (cur_num % 2 == 1)
				buffer[j] = true;
			cur_num = cur_num / 2;
		}
		if (countNumOfTrue(buffer, n) == 2)
		{
			for (int j = 0; j < n; j++)
			{
				bol[curLen * n + j] = buffer[j];
			}
			curLen++;
			if (curLen > len)
				exit(-6);		//������������
		}
	}
	return bol;
}