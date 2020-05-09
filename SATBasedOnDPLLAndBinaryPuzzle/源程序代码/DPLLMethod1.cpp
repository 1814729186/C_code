#include "BinaryPuzzle1.h"


//初始化栈
void initStack(pstack &sta) {
	if (sta != NULL) return;	//栈非空，不能初始化
	sta = (pstack)malloc(sizeof(stack));
	sta->tail = (stackNode*)malloc(sizeof(stackNode));		//设置栈底
	sta->top = sta->tail;
	sta->tail->nextNode = NULL;
}
//入栈
void addStack(pstack sta, int word, bool methodMark)
{
	pStackNode psn = (pStackNode)malloc(sizeof(stackNode));
	if (psn == NULL) exit(-4);
	psn->methodMark = methodMark;
	psn->setTrueWord = word;
	psn->nextNode = sta->top;
	sta->top = psn;
}
//出栈,返回两个值，word&methodMark,栈为空时返回ERROR，否则返回OK
Status outStack(pstack sta, int &word, bool &methodMark)
{
	if (sta->tail == sta->top)
		return ERROR;
	pStackNode psn = sta->top;
	methodMark = psn->methodMark;
	word = psn->setTrueWord;
	sta->top = sta->top->nextNode;
	free(psn);
	return OK;
}
//检测空栈
bool cheakStack(pstack sta)
{
	if (sta->tail == sta->top)	return true;
	return false;
}

//根据栈元素进行回溯
Status recovery(pstack sta, pCnfFile pcnf)
{
	//获取栈中的元素，并将其回溯为上一次的结构
	int word = 0;
	bool methodMark = false;
	Status status;
	pChildSentence pcs = NULL;
	pWord pw = NULL;
	while (1)//回溯直到找到分支为止
	{

		status = outStack(sta, word, methodMark);
		if (status == ERROR)	//空栈
			return ERROR;
		if (word == 0)	exit(-6);
		//回溯
		pcs = pcnf->sentence;
		while (pcs != NULL)
		{
			//重建因为word而被删除掉的子句（将mark置为0）
			if (pcs->mark == word)
			{
				pcs->mark = 0;
				pcnf->numOfchildSentence++;
			}


			//重建因为word而被删掉的-word文字
			else if (pcs->mark == 0)
			{
				pw = pcs->words;
				while (pw != NULL)
				{
					if (pw->mark == false && pw->varibale + word == 0)
					{
						pw->mark = true;
						pcs->numOfword++;
					}
					pw = pw->nextword;
				}
			}
			pcs = pcs->nextSen;
		}
		if (methodMark == true)
			break;
	}
	//假设-word为真，利用规则化简集合，此处入栈时当作单子句规则处理，之后回溯到这一步就不用再次遇见分支
	//入栈
	addStack(sta, (-1)*word, false);
	simplyS(pcnf, (-1)*word);
	return OK;
}

//化简规则，单子句中的化简/分裂规则中的化简
Status simplyS(pCnfFile pcnf, int word)
{
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	//删除含有word的子句
	while (pcs != NULL)
	{
		if (pcs->mark == 0 && (checkWord(pcs, word)))
		{
			pcs->mark = word;
			pcnf->numOfchildSentence--;
		}

		pcs = pcs->nextSen;
	}
	//分裂规则,删除含有-word的文字
	pcs = pcnf->sentence;
	while (pcs != NULL)
	{
		if (pcs->mark == 0)
		{
			//遍历该子句，查找word
			pw = pcs->words;
			while (pw != NULL)
			{
				if (pw->varibale + word == 0 && pw->mark == true)
				{
					//删除该文字
					pw->mark = false;
					pcs->numOfword--;

				}
				pw = pw->nextword;
			}
		}
		pcs = pcs->nextSen;
	}
	return OK;
}
//检测子句中是否含有目标文字
bool checkWord(pChildSentence pcs, int word)
{
	pWord pw = pcs->words;
	while (pw != NULL)
	{
		if (pw->varibale == word && pw->mark == true)
			return true;
		pw = pw->nextword;
	}
	return false;
}


//选择分裂规则中使用的文字，选择结构中同时出现原文字、非文字的文字
int seclect(pCnfFile pcnf, int *count, bool &mark)
{
	//定义数组
	//遍历数组，统计出现的各文字出现的次数
	//初始化
	for (int i = 0; i <= pcnf->numOfVariable; i++)
		count[i] = 0;
	pChildSentence pcs = NULL;
	pWord pw = NULL;
	pcs = pcnf->sentence;
	mark = true;
	while (pcs != NULL)
	{
		if (pcs->isCunzai != true || pcs->mark != 0)
		{
			pcs = pcs->nextSen;
			continue;
		}

		pw = pcs->words;
		while (pw != NULL)
		{
			if (pw->mark == true)
			{
				if (pw->varibale > 0)
				{
					if (count[pw->varibale] < 0)
					{
						return pw->varibale;
					}

					else
						count[pw->varibale]++;
				}
				else
				{
					if (count[-1 * pw->varibale] > 0)
					{
						return pw->varibale;
					}
					else
						count[-1 * pw->varibale]--;
				}
			}
			pw = pw->nextword;
		}
		pcs = pcs->nextSen;
	}
	//未找到同时出现正负的文字，返回数组中首个出现过的数
	mark = false;
	for (int i = 1; i <= pcnf->numOfVariable; i++)
	{
		if (count[i] < 0)
		{
			return (-1)*i;
		}
		else if (count[i] > 0)
		{
			return i;
		}

	}
	for (int i = 1; i <= pcnf->numOfVariable; i++) {
		printf("%d ", count[i]);
	}
	exit(-99);

}
//寻找单子句，成功找到则返回单子句的文字，否则返回0
int findSingal(pCnfFile pcnf)
{
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	while (pcs != NULL)
	{
		if (pcs->mark == 0 && pcs->numOfword == 1)
		{
			pw = pcs->words;
			while (pw != NULL)
			{
				if (pw->mark == true)
					return pw->varibale;
				pw = pw->nextword;
			}
		}
		pcs = pcs->nextSen;
	}
	//未找到单子句
	return 0;
}
//检测S中是否存在空子句
bool checkNull(pCnfFile pcnf)
{
	pChildSentence pcs = pcnf->sentence;
	while (pcs != NULL)
	{
		if (pcs->mark == 0 && pcs->numOfword == 0)
			return true;
		pcs = pcs->nextSen;
	}
	return false;
}

Status DPLL(pCnfFile pcnf, pstack &stack,int beginTime,bool change)
{
	int cur_time;
	int * count;
	count = (int*)malloc(sizeof(int)*(1 + pcnf->numOfVariable));
	pstack sta = NULL;
	initStack(sta);
	stack = sta;
	bool mark = false;
	bool status;
	if (sta == NULL)		exit(-1);
	int cur_word = 0;
	do {
		while (1)
		{
			if (pcnf->numOfchildSentence == 0)	//集合为空
			{
				free(count);
				return TRUE;
			}
			status = false;
			if ((status = checkNull(pcnf)) == true)		//S中存在空子句，开始回溯
				break;
			cur_word = findSingal(pcnf);
			if (cur_word == 0)		//无单子句
				break;
			//化简单子句
			addStack(sta, cur_word, false);
			simplyS(pcnf, cur_word);
		}
		if (change == true)
		{
			cur_time = clock();
			if (cur_time - beginTime >= 20000)//20s
			{
				free(count);
				return ERROR;
			}
		}

		if (status == true)		//存在空子句，回溯
		{
			if (recovery(sta, pcnf) == ERROR)
			{
				free(count);
				return FALSE;
			}
		}
		else {



			cur_word = seclect(pcnf, count, mark);
			if (cur_word == 0)	exit(-10);
			addStack(sta, cur_word, mark);
			simplyS(pcnf, cur_word);
		}

	} while (cheakStack(sta) == FALSE);
	free(count);
	return FALSE;

}

//文件读取函数
Status readFile(pCnfFile pcnf,char *pFilename)
{
	char filename[100], *ch, cch;
	char addrFilename[150] = ADDR;

	printf("输入文件名称：");
	scanf("%s", filename);
	//拼接地址+文件名
	int i = 0;
	while (addrFilename[i] != '\0')	i++;
	for (int j = 0; filename[j] != '\0'; j++)
		addrFilename[i++] = filename[j];
	addrFilename[i] = '\0';
	strcpy(pFilename,addrFilename);

	FILE* file;
	//打开文件
	if (!(file = fopen(addrFilename, "r")))
	{
		return ERROR;
	}
	readAndCreate(pcnf, file);
	fclose(file);
	//创建结点完成
	//pcnf文件完成创建
	return OK;

}

//读文件创建结构
Status readAndCreate(pCnfFile pcnf, FILE*file)
{
	char buffer[100];						//字符串缓冲区
	int index = 0;						//字符索引指针
	char cch, *ch;
	//按行读取文件
	while (1)
	{
		cch = fgetc(file);
		if (cch == 'c')	//注释行,打印到窗口中
		{
			fgets(buffer, 100, file);
			//打印到窗口中
			printf("%s", buffer);
		}
		else if (cch == 'p')	//信息行，停止打印
		{
			fgets(buffer, 6, file);//跳过前缀读取
			break;
		}
	}
	fscanf(file, "%d %d", &(pcnf->numOfVariable), &(pcnf->numOfchildSentence));

	//读取文件并创建子句
	int cur_variable = 1;
	//读取首个子句
	pChildSentence pcs = NULL;
	pWord pw = NULL;
	//读取首个变元
	fscanf(file, "%d ", &cur_variable);

	//创建首个子句储存空间
	pcnf->sentence = (pChildSentence)malloc(sizeof(childSentence));
	//初始化
	pcs = pcnf->sentence;
	pcs->isCunzai = true;
	pcs->mark = 0;
	pcs->nextSen = NULL;
	pcs->numOfword = 0;
	pcs->words = NULL;


	if (cur_variable != 0)
	{


		//创建首个子句的变元文字
		pcs->words = (pWord)malloc(sizeof(word));
		//初始化和赋值
		pw = pcs->words;
		pw->varibale = cur_variable;
		pw->nextword = NULL;
		pw->mark = true;
		pcs->numOfword++;
		//继续读取，创建文字
		while (1)
		{
			fscanf_s(file, "%d ", &cur_variable);
			if (cur_variable == 0)
				break;
			pw->nextword = (pWord)malloc(sizeof(word));
			//赋值
			pw = pw->nextword;
			pw->nextword = NULL;
			pw->mark = true;
			pw->varibale = cur_variable;
			pcs->numOfword++;
		}
	}
	while (fscanf(file, "%d ", &cur_variable) != EOF)
	{
		pcs->nextSen = (pChildSentence)malloc(sizeof(childSentence));
		//初始化
		pcs = pcs->nextSen;
		pcs->isCunzai = true;
		pcs->nextSen = NULL;
		pcs->mark = 0;
		pcs->numOfword = 0;
		pcs->words = NULL;
		//第一个元素就是0，空子句
		if (cur_variable == 0) continue;
		//创建首个文字结点
		pcs->words = (pWord)malloc(sizeof(word));
		//赋值、初始化
		pw = pcs->words;
		pw->varibale = cur_variable;
		pw->mark = true;
		pw->nextword = NULL;
		pcs->numOfword++;

		//读取后面的文字并创建结点
		while (1)
		{
			fscanf_s(file, "%d ", &cur_variable);
			if (cur_variable == 0)
				break;
			pw->nextword = (pWord)malloc(sizeof(word));
			//赋值
			pw = pw->nextword;
			pw->nextword = NULL;
			pw->varibale = cur_variable;
			pw->mark = true;
			pcs->numOfword++;
		}
	}
	return OK;
}
//销毁栈并递归输出路径
void destroyStack(pstack sta, bool isSuccess, int * answer, int &index)
{
	//成功时输出路径，未成功时不输出
	int count = 0;
	pStackNode psn = sta->top;
	destroy(psn, sta, count, isSuccess, answer, index);
	return;
}
void destroy(pStackNode psn, pstack sta, int &i, bool isSuccess, int *answer, int &index)
{
	int word = 0;
	if (psn == sta->tail)
		free(psn);
	else
	{
		word = psn->setTrueWord;
		destroy(psn->nextNode, sta, i, isSuccess, answer, index);
		if (answer != NULL)
			answer[index++] = word;
		if (isSuccess == true)
		{
			printf("%d\t", word);
			i++;
			if (i % 10 == 0) printf("\n");
		}
	}
	return;
}

//销毁当前的cnf结构
void destroyCNF(pCnfFile pcnf)
{
	//销毁子句
	pChildSentence pcs = pcnf->sentence;
	pChildSentence pcs_l = pcs;
	/*while (pcs!=NULL)
	{
		pcs_l = pcs->nextSen;
		destroyChildScen(pcs);
		pcs = pcs_l;
	}*/ //意料之外的bug
	pcnf->numOfchildSentence = 0;
	pcnf->numOfVariable = 0;
	pcnf->sentence = NULL;
	return;
}
//销毁子句
void destroyChildScen(pChildSentence pcs)
{
	pWord pw = pcs->words;
	pWord pw1;
	while (1)
	{
		if (pw == NULL)
			break;
		pw1 = pw->nextword;
		free(pw);
		pw = pw1;
	}
	free(pcs);
	return;
}
//test

//完成求解后，重置pcnf的mask
void resetPcnf(pCnfFile pcnf)
{
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	while (pcs != NULL)
	{
		if (pcs->mark != 0 && pcs->isCunzai == true)
		{
			pcs->mark = 0;
			pcnf->numOfchildSentence++;
		}
		pw = pcs->words;
		while (pw != NULL)
		{
			if (pw->mark == false)
			{
				pw->mark = true;
				pcs->numOfword++;
			}
			pw = pw->nextword;
		}
		pcs = pcs->nextSen;
	}
}

//输出res文件
Status writeRes(char* fileName,bool isSuccess , int sumTime,int *answer,int n)
{
	//将文件名后缀改成res
	int i = 0;
	printf("\n是否输出res文件？(1-是，其他-否)\n");
	scanf(" %d", &i);
	if (i != 1) return OK;
	i = 0;
	while (fileName[i] != '\0')		i++;//找到结尾
	fileName[i - 3] = '\0';
	strcat(fileName, "res");//改变文件名完毕
	

	FILE* fp = NULL;
	fp = fopen(fileName,"w");//打开文件
	if (fp == NULL)	exit(-1);
	if (isSuccess == true)
		fprintf(fp, "s 1\n");
	else 
		fprintf(fp, "s 0\n");

	printf("文件写入成功！\n");
	
	fprintf(fp, "v");
	if (isSuccess == true)
	{
		for(i = 0;i < n;i ++)
		{
			fprintf(fp," %d", answer[i]);		
			if ((i + 1) % 10 == 0)
				fprintf(fp,"\n  ");
		}
	}
		
	fprintf(fp,"\nt %d", sumTime);
		
	fclose(fp);
	return OK;
}
