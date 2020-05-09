#include "BinaryPuzzle1.h"

//生成数读地图
//使用挖洞法生成数独地图
//首先随机在数独中填入一些空格，对数独进行求解，若有解，以求解完毕的数独为基础，使用挖洞法生成数独，若无解，则重新填入
//4阶数组--随机生成2个位置的数
//6阶数组--随机生成3个位置的数
//8阶数组--随机生成4个位置的数		
void generateSoDuKuMap(pCnfFile pcnf, int n)	//n表示阶数
{
	if (pcnf->sentence != NULL)
	{
		printf("当前pcnf文件结构非空，请清空后再试！\n");
		getchar();
		getchar();
		return;
	}
	bool* map = (bool*)malloc(sizeof(bool)*n*n);		//储存当前数独地图
	int numOfBlank = 0;		//生成随机个数
	int location = 0;				//位置
	//随机得到将要填入的个数
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
	else exit(-1);	//不符合操作
	//用pcnf读取数独配置文件，配置文件不存在时提醒创建配置文件
	char addrFilename[200] = ADDR;
	strcat(addrFilename, "BinaryPuzzleConditionFile");
	char ch[2] = "4";
	ch[0] = n + '0';
	strcat(addrFilename, ch);
	FILE* file;
	pstack sta = NULL;
	int *saveAnwser = (int *)malloc(sizeof(int)*(n*n));
	int num;		//用于接收随机数，判断随机的数的正负
	int index = 0;
	//
	while (1)
	{
		index = 0;
		//初始化
		for (int i = 0; i < n*n; i++)
		{
			saveAnwser[i] = 0;
		}
		file = fopen(addrFilename, "r");
		if (file == NULL)//打开文件失败
		{
			printf("配置文件不存在，请生成配置文件后再试。\n");
			getchar();
			getchar();
			return;
		}

		//读取文件
		readAndCreate(pcnf, file);
		fclose(file);

		//在随机位置填入随机数
		for (int i = 0; i < numOfBlank; i++)
		{
			//生成随机位置
			location = rand() % (n*n) + 1;
			//将随机位置存入到pcnf中去
			num = rand() % 2;
			if (num == 1) location = (-1)*location;
			addNodeToPcnf(pcnf, location);
		}

		if (DPLL(pcnf, sta,0,false) == TRUE) break;		//成功生成解
		//销毁pcnf结构，重新读取
		destroyCNF(pcnf);
		destroyStack(sta, false, NULL, index);
		sta = NULL;
		pcnf = NULL;
	}
	//将结果储存到数组中去
	destroyStack(sta, false, saveAnwser, index);

	//重新生成pcnf结构
	destroyCNF(pcnf);
	file = fopen(addrFilename, "r");
	if (file == NULL)//打开文件失败
	{
		printf("配置文件不存在，请生成配置文件后再试。\n");
		getchar();
		getchar();
		return;
	}
	//读取文件
	readAndCreate(pcnf, file);
	fclose(file);

	//将求得的解存入cnf
	for (int i = 0; i < pcnf->numOfVariable; i++)
	{
		if (saveAnwser[i] == 0) exit(-1);
		addNodeToPcnf(pcnf, saveAnwser[i]);
	}
	//复制当前数组作为挖洞的依据

	int *digHole = (int*)malloc(sizeof(int)*(n*n));
	for (int i = 0; i < (n*n); i++)
	{
		digHole[i] = saveAnwser[i];
	}
	printf("输入想要尝试挖空的个数：（实际形成的空格可能没有希望的那么多）\n");
	scanf("%d", &numOfBlank);
	digMap(pcnf, digHole, numOfBlank, n);
	//生成成功
	//打印数独地图
	printMap(digHole, saveAnwser, n);
	printf("是否储存CNF文件？（1-- 储存 ，其他-- 放弃）\n>");
	int op = -1;
	scanf("%d", &op);
	if (op == 1)
	{
		writeSoDoKufile(pcnf);
	}
	//清空当前cnf文件结构
	destroyCNF(pcnf);
	return;
}

//写入文件
Status writeSoDoKufile(pCnfFile pcnf)
{

	char saveName[30];
	char addr[100] = ADDR;
	printf("请输入文件名：\n>");
	scanf("%s", saveName);
	FILE *file = fopen(strcat(addr, saveName), "w");
	if (file == NULL)
	{
		printf("打开文件失败！");
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
	//-1表示空格
	//初始化
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
	//打印未求解的map
	for (int i = 0; i < n*n; i++)
	{
		//复制数组
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
	printf("生成的数独如下\n");
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
	printf("求解后的数独...\n");
	getchar();
	getchar();
	//打印求解的后的数独
	for (int i = 0; i < n; i++)
		printf("--");
	printf("\n");
	//初始化
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
		//复制数组
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

//挖洞生成数独地图,传入生成的空的个数,n表示阶数
void digMap(pCnfFile pcnf, int *digHole, int numOfHole, int n)
{
	int num = 1;
	int i = 1;
	Status status;
	//从首个位置开始，将其更改，查看是否有解
	while (i <= numOfHole)
	{
		num = digHole[i];
		status = changeAndDPLL(pcnf, num, n);
		if (status == FALSE)	//DPLL失败，可删除该结点
		{
			//在数组digHole中将该结点的值删除
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
			break;				//达到了最大位置处,不能删掉num这么多个点
	}
}

//将编号为num的位置设为其相反数，查看是否有解,n为阶数
Status changeAndDPLL(pCnfFile pcnf, int num, int n)
{
	Status status;		//接受DPLL函数返回值
	//查找num或-num
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
	//将pcnf中的num结点更改为-num
	if (bol == true) changeNum = (-1)*num;
	else changeNum = num;
	pcs->words->varibale = changeNum;

	//DPLL检测是否有解
	status = DPLL(pcnf, sta,0,false);
	//将结点恢复原状
	int _index = 0;
	if (bol == false)	pcs->words->varibale = (-1)*num;
	else pcs->words->varibale = num;
	destroyStack(sta, false, NULL, _index);
	//有解则证明该结点不可删除，返回TRUE
	if (status == TRUE)
	{
		//将cnf结构重置
		resetPcnf(pcnf);
		return TRUE;
	}
	//无解将结点的mark置为-(n*n+1),且将isCunzai置为false ，说明该结点已经被删除，返回FALSE
	if (status == FALSE)
	{
		pcs->isCunzai = false;
		pcs->mark = n * n + 1;
		pcnf->numOfchildSentence--;
		//将cnf结构重置
		resetPcnf(pcnf);
		return FALSE;
	}
}

//查找pcnf中为单子句num或者-num的结点，找到后返回指向该子句的指针，未找到则返回NULL,用bol记录该位置的数的正负
pChildSentence findNum(pCnfFile pcnf, int num, bool &bol)
{
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	while (pcs != NULL)
	{
		if (pcs->mark == 0 && pcs->numOfword == 1)	//单子句
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
	//找到最后一个子句的位置
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

//(1)限制1，同行、同列不能连续三个相同
void printCondition1(FILE * file, int n)
{
	//同行
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 2; j++)
		{
			fprintf(file, "%d %d %d 0\n", n * i + j + 1, n * i + j + 2, n * i + j + 3);
			fprintf(file, "%d %d %d 0\n", -1 * (n * i + j + 1), -1 * (n * i + j + 2), -1 * (n * i + j + 3));
		}
	}
	//同列
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

//(2)限制2，同一行或同一列0，1个数相同，任意n/2 +1个不相同
void printCondition2(FILE*file, int n)
{
	int len = 0;
	int cur_index;
	//生成1~n中的n/2+1的所有组合数，用数组保存
	bool *bol = generateCombination(n, len);
	//通过数组选择组合数
	for (int i = 0; i < len; i++)
	{
		//编号为i的n个bool组成的组合数首个bool的下标为：n*i

		//同一行
		for (int j = 0; j < n; j++)
		{
			//从上往下的n行
			//编号为j的行开始的位置编号为：1+n*j
			//正
			cur_index = 1 + n * j;
			for (int k = n * i; k < n*i + n; k++, cur_index++)
			{
				if (bol[k] == true)
				{
					fprintf(file, "%d ", cur_index);
				}
			}
			fprintf(file, "0\n");
			//负
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
		//同一列
		for (int j = 0; j < n; j++)
		{
			//编号为j的列开始的首个位置为j+1，每次增加n
			//正
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

//（3）限制3，任意两行不相同
void printCondition3(FILE*file, int n)
{
	//得到C（n,2）
	int sumSeclectLine = 0;		//选择两行的不同选择的总数
	bool *sceLine = printCn2(n, sumSeclectLine);
	int line1 = 0, line2 = 0;		//当前操作的两行的首编号
	int row1 = 0, row2 = 0;		//当前操作的列的首编号
	//得到不同的配合数
	int sumOfzuhe = 0;
	bool *zuhe = generateZuhe(n, sumOfzuhe);

	for (int i = 0; i < sumSeclectLine; i++)
	{
		//编号为i的一组c(n,2)组合开始的位置为:n*i
		//行选择，列选择
		line1 = 0; line2 = 0;//初始化
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
		//完成行选择
		//对于选择的行进行操作
		for (int k = 0; k < sumOfzuhe; k++)
		{
			//标号为k的zuhe开始的坐标为k*n,结束于k*(n+1)-1
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
		//根据行设置列
		row1 = (line1 / n) + 1;
		row2 = (line2 / n) + 1;
		//对于选择的列进行操作
		for (int k = 0; k < sumOfzuhe; k++)
		{
			//标号为k的zuhe开始的坐标为k*n,结束于k*(n+1)-1
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
	int n = 0;	//数独的阶数
	int maxLen = 1;
	int sumOfChildScentence = 0;	//子句总数

	printf("请输入二进制数独的阶数:(4 6 8)\n>");
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
	//打开文件
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
	printf("文件%s已存入\n", addrFilename);
	return;

}


//生成1~n中的n/2+1的所有组合数，用数组保存
bool* generateCombination(int n, int &len)
{
	if (n == 8)
		len = 56;
	else if (n == 6)
		len = 15;
	else if (n == 4)
		len = 4;
	else
		return NULL;//错误情况
	bool *combi = (bool*)malloc(sizeof(bool) * n*len);		//n == 8
	bool buffer[8];	//缓冲区
	int cur_num;	//当前操作数
	//组合数
	int length = 1;
	int curLen = 0;
	for (int i = 0; i < n; i++)
		length *= 2;				//n位二进制数最长的情况
	for (int i = 0; i < length; i++)
	{

		cur_num = i;
		//重置buffer
		for (int j = 0; j < 8; j++)
			buffer[j] = false;
		//除二取余法，求二进制码
		for (int j = 0; j < n; j++)
		{
			if (cur_num == 0)	break;		//当前操作数已经为0，无需继续储存下去
			if (cur_num % 2 == 1)
				buffer[j] = true;
			cur_num /= 2;
		}
		//求得二进制码，判断二进制码中有几个true，有 (n/2)+1个1则取用存入目标数组中去
		if (countNumOfTrue(buffer, n) == (n / 2) + 1)
		{
			for (int j = 0; j < n; j++)
			{
				combi[curLen * n + j] = buffer[j];
			}
			curLen++;			//当前位置
			if (curLen > len)
				exit(-6);		//溢出，错误情况
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
//生成00000000~11111111中的所有数
bool* generateZuhe(int n, int &len)
{
	int length = 1;
	for (int i = 0; i < n; i++)
		length *= 2;
	len = length;
	int cur_num;
	//创建储存空间
	bool *zuHe = (bool*)malloc(sizeof(bool) * n * length);
	//初始化
	for (int i = 0; i < length; i++)
		zuHe[i] = false;
	//除二取余法列出00000000~11111111中的所有数
	for (int i = 0; i < length; i++)
	{
		//除二取余法
		cur_num = i;
		for (int j = 0; j < n; j++)
		{
			if (cur_num == 0)	break;
			if (cur_num % 2 == 1)
				zuHe[j + n * i] = true;	//第i个数存储的位置
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
		//重置buffer
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
				exit(-6);		//溢出，错误情况
		}
	}
	return bol;
}