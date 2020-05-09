#include"BinaryPuzzle1.h"
/**
*改变储存结构实现DPLL，提高效率改进方法中改进的函数
*/

//在count结构中添加变量variable
void addToCount(pHeadCount phc, int variable)
{
	int cur_vari = 0;
	if (variable > 0)
	{
		cur_vari = variable;
		phc->count[variable].posi++;
	}
	else if (variable < 0)
	{
		cur_vari = (-1)*variable;
		phc->count[-variable].nega++;
	}
	else {
		printf("addToCount error!");
		getchar();
		getchar();
		exit(-1);
	}
	//更改当前结构中的最多的变量
	if (phc->count[cur_vari].posi + phc->count[cur_vari].nega > phc->sum)
	{
		phc->max_variable = cur_vari;
		phc->sum = phc->count[cur_vari].posi + phc->count[cur_vari].nega;
		if (phc->count[cur_vari].posi > 0)
			phc->containPo = true;
		else
			phc->containPo = false;
		if (phc->count[cur_vari].nega > 0)
			phc->cintainNe = true;
		else
			phc->cintainNe = false;
	}

}

//从count结构中删除variable
void delFormCount(pHeadCount phc, int variable, pCnfFile pcnf)
{
	//从结构中删除一个
	int cur_vari = 0;
	if (variable > 0)
	{
		cur_vari = variable;
		phc->count[variable].posi--;
	}
	else if (variable < 0)
	{
		cur_vari = (-1)*variable;
		phc->count[cur_vari].nega--;
	}
	else {
		printf("delFromCount error!");
		getchar();
		getchar();
		exit(-1);
	}
	//更新当前结构中的最多的变量
	if (phc->max_variable == cur_vari)
	{
		phc->sum--;
		if (phc->count[cur_vari].nega == 0)
			phc->cintainNe = false;
		if (phc->count[cur_vari].posi == 0)
			phc->containPo = false;
		for (int i = 1; i <= pcnf->numOfVariable; i++)
		{
			//遍历，判断是否存在超过sum的变量存在
			if (phc->count[i].nega + phc->count[i].posi > phc->sum)
			{
				phc->sum = phc->count[i].nega + phc->count[i].posi > phc->sum;
				phc->max_variable = i;
				if (phc->count[i].nega == 0)
					phc->cintainNe = false;
				else phc->cintainNe = true;
				if (phc->count[i].posi == 0)
					phc->containPo = false;
				else phc->containPo = true;
			}
		}
	}
	
}

//文件读取函数改进--添加统计count的操作入口
Status readFile_1(pCnfFile pcnf,pHeadCount &_phc,char*pFilename)
{
	char filename[200], *ch, cch;
	char addrFilename[100] = ADDR;

	printf("输入文件名称：");
	scanf("%s", filename);
	//拼接地址+文件名
	int i = 0;
	while (addrFilename[i] != '\0')	i++;
	for (int j = 0; filename[j] != '\0'; j++)
		addrFilename[i++] = filename[j];
	addrFilename[i] = '\0';
	strcpy(pFilename, addrFilename);

	FILE* file;
	//打开文件
	if (!(file = fopen(addrFilename, "r")))
	{
		return ERROR;
	}
	readAndCreate_1(pcnf, file,_phc);
	fclose(file);
	//创建结点完成
	//pcnf文件完成创建
	return OK;

}

//读文件创建结构
Status readAndCreate_1(pCnfFile pcnf, FILE*file,pHeadCount &_phc)
{
	char buffer[100];						//字符串缓冲区
	int index = 0;						//字符索引指针
	char cch, *ch;
	pHeadCount phc = (pHeadCount)malloc(sizeof(headCount));
	//初始化
	phc->cintainNe = false;
	phc->containPo = false;
	phc->max_variable = 0;
	phc->sum = 0;
	_phc = phc;
	
	printf("文件注释如下：\n");
	//按行读取文件
	while (1)
	{
		cch = fgetc(file);
		if (cch == 'c')	//注释行,打印到窗口中
		{
			fgets(buffer,100,file);
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
	//创建数组结构
	phc->count = (pCount)malloc(sizeof(count)*(pcnf->numOfVariable + 1));
	//初始化
	for (int i = 0; i <= pcnf->numOfVariable; i++)
	{
		phc->count[i].nega = 0;
		phc->count[i].posi = 0;
	}
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

		//储存到count中去
		addToCount(phc,cur_variable);
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
			//储存到count中去
			addToCount(phc, cur_variable);
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
		//储存到count中去
		addToCount(phc, cur_variable);
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
			//储存到count中去
			addToCount(phc, cur_variable);
		}
	}
	return OK;
}

//DPLL改写
Status DPLL_1(pCnfFile pcnf, pstack &stack,pHeadCount phc)
{
	int cur_time;
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
			simplyS_1(pcnf, cur_word, phc);
		}

		if (status == true)		//存在空子句，回溯
		{
			if (recovery_1(sta, pcnf,phc) == ERROR)
			{
				return FALSE;
			}
		}
		else {
			//选取变元，进行分裂操作
			cur_word = select_1(pcnf, phc, mark);
			if (cur_word == 0)	exit(-2);
			addStack(sta, cur_word, mark);
			simplyS_1(pcnf, cur_word,phc);
		}
	} while (cheakStack(sta) == FALSE);
	return FALSE;
}

//化简单子句函数改写
Status simplyS_1(pCnfFile pcnf, int word,pHeadCount phc)
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
			pw = pcs->words;
			while (pw)
			{
				//从count结构中删除一个记录
				if (pw->mark == true)
				{
					delFormCount(phc,pw->varibale,pcnf);
				}
				pw = pw->nextword;
			}
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
					//从count结构中删除一个记录
					delFormCount(phc, pw->varibale, pcnf);

				}
				pw = pw->nextword;
			}
		}
		pcs = pcs->nextSen;
	}
	return OK;
}

//选取变元改写
int select_1(pCnfFile pcnf, pHeadCount phc, bool &mark)
{
	int res = 0;
	//得到结构中数量最多的文字
	if (phc->containPo == true && phc->cintainNe == true)
	{
		mark = true;
		res = phc->max_variable;
	}
	else if (phc->cintainNe == true && phc->containPo == false)
	{
		mark = false;
		res = (-1)*phc->max_variable;
	}
	else if (phc->containPo == true && phc->cintainNe == false)
	{
		mark = false;
		res = phc->max_variable;
	}
	else
	{
		printf("select_1 error!");
		getchar();
		getchar();
		exit(-1);
	}
	return res;
}

//recovery函数改写
//根据栈元素进行回溯
Status recovery_1(pstack sta, pCnfFile pcnf,pHeadCount phc)
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
				pw = pcs->words;
				while (pw)
				{
					if (pw->mark == true)
					{
						addToCount(phc,pw->varibale);
					}
					pw = pw->nextword;
				}
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
						addToCount(phc,pw->varibale);
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
	simplyS_1(pcnf, (-1)*word, phc);
	return OK;
}

//count数据恢复和重置pcnf标记
void resetCount(pCnfFile pcnf, pHeadCount phc)
{
	//初始化
	phc->cintainNe = false;
	phc->containPo = false;
	phc->max_variable = 0;
	phc->sum = 0;
	for (int i = 1; i <= pcnf->numOfVariable; i++)
	{
		phc->count[i].nega = 0;
		phc->count[i].posi = 0;
	}

	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	while (pcs != NULL)
	{
		pw = pcs->words;
		while (pw != NULL)
		{
			addToCount(phc,pw->varibale);
			pw = pw->nextword;
		}
		pcs = pcs->nextSen;
	}
}