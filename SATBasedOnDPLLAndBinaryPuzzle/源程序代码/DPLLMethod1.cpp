#include "BinaryPuzzle1.h"


//��ʼ��ջ
void initStack(pstack &sta) {
	if (sta != NULL) return;	//ջ�ǿգ����ܳ�ʼ��
	sta = (pstack)malloc(sizeof(stack));
	sta->tail = (stackNode*)malloc(sizeof(stackNode));		//����ջ��
	sta->top = sta->tail;
	sta->tail->nextNode = NULL;
}
//��ջ
void addStack(pstack sta, int word, bool methodMark)
{
	pStackNode psn = (pStackNode)malloc(sizeof(stackNode));
	if (psn == NULL) exit(-4);
	psn->methodMark = methodMark;
	psn->setTrueWord = word;
	psn->nextNode = sta->top;
	sta->top = psn;
}
//��ջ,��������ֵ��word&methodMark,ջΪ��ʱ����ERROR�����򷵻�OK
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
//����ջ
bool cheakStack(pstack sta)
{
	if (sta->tail == sta->top)	return true;
	return false;
}

//����ջԪ�ؽ��л���
Status recovery(pstack sta, pCnfFile pcnf)
{
	//��ȡջ�е�Ԫ�أ����������Ϊ��һ�εĽṹ
	int word = 0;
	bool methodMark = false;
	Status status;
	pChildSentence pcs = NULL;
	pWord pw = NULL;
	while (1)//����ֱ���ҵ���֧Ϊֹ
	{

		status = outStack(sta, word, methodMark);
		if (status == ERROR)	//��ջ
			return ERROR;
		if (word == 0)	exit(-6);
		//����
		pcs = pcnf->sentence;
		while (pcs != NULL)
		{
			//�ؽ���Ϊword����ɾ�������Ӿ䣨��mark��Ϊ0��
			if (pcs->mark == word)
			{
				pcs->mark = 0;
				pcnf->numOfchildSentence++;
			}


			//�ؽ���Ϊword����ɾ����-word����
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
	//����-wordΪ�棬���ù��򻯼򼯺ϣ��˴���ջʱ�������Ӿ������֮����ݵ���һ���Ͳ����ٴ�������֧
	//��ջ
	addStack(sta, (-1)*word, false);
	simplyS(pcnf, (-1)*word);
	return OK;
}

//������򣬵��Ӿ��еĻ���/���ѹ����еĻ���
Status simplyS(pCnfFile pcnf, int word)
{
	pChildSentence pcs = pcnf->sentence;
	pWord pw = NULL;
	//ɾ������word���Ӿ�
	while (pcs != NULL)
	{
		if (pcs->mark == 0 && (checkWord(pcs, word)))
		{
			pcs->mark = word;
			pcnf->numOfchildSentence--;
		}

		pcs = pcs->nextSen;
	}
	//���ѹ���,ɾ������-word������
	pcs = pcnf->sentence;
	while (pcs != NULL)
	{
		if (pcs->mark == 0)
		{
			//�������Ӿ䣬����word
			pw = pcs->words;
			while (pw != NULL)
			{
				if (pw->varibale + word == 0 && pw->mark == true)
				{
					//ɾ��������
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
//����Ӿ����Ƿ���Ŀ������
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


//ѡ����ѹ�����ʹ�õ����֣�ѡ��ṹ��ͬʱ����ԭ���֡������ֵ�����
int seclect(pCnfFile pcnf, int *count, bool &mark)
{
	//��������
	//�������飬ͳ�Ƴ��ֵĸ����ֳ��ֵĴ���
	//��ʼ��
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
	//δ�ҵ�ͬʱ�������������֣������������׸����ֹ�����
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
//Ѱ�ҵ��Ӿ䣬�ɹ��ҵ��򷵻ص��Ӿ�����֣����򷵻�0
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
	//δ�ҵ����Ӿ�
	return 0;
}
//���S���Ƿ���ڿ��Ӿ�
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
			if (pcnf->numOfchildSentence == 0)	//����Ϊ��
			{
				free(count);
				return TRUE;
			}
			status = false;
			if ((status = checkNull(pcnf)) == true)		//S�д��ڿ��Ӿ䣬��ʼ����
				break;
			cur_word = findSingal(pcnf);
			if (cur_word == 0)		//�޵��Ӿ�
				break;
			//�����Ӿ�
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

		if (status == true)		//���ڿ��Ӿ䣬����
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

//�ļ���ȡ����
Status readFile(pCnfFile pcnf,char *pFilename)
{
	char filename[100], *ch, cch;
	char addrFilename[150] = ADDR;

	printf("�����ļ����ƣ�");
	scanf("%s", filename);
	//ƴ�ӵ�ַ+�ļ���
	int i = 0;
	while (addrFilename[i] != '\0')	i++;
	for (int j = 0; filename[j] != '\0'; j++)
		addrFilename[i++] = filename[j];
	addrFilename[i] = '\0';
	strcpy(pFilename,addrFilename);

	FILE* file;
	//���ļ�
	if (!(file = fopen(addrFilename, "r")))
	{
		return ERROR;
	}
	readAndCreate(pcnf, file);
	fclose(file);
	//����������
	//pcnf�ļ���ɴ���
	return OK;

}

//���ļ������ṹ
Status readAndCreate(pCnfFile pcnf, FILE*file)
{
	char buffer[100];						//�ַ���������
	int index = 0;						//�ַ�����ָ��
	char cch, *ch;
	//���ж�ȡ�ļ�
	while (1)
	{
		cch = fgetc(file);
		if (cch == 'c')	//ע����,��ӡ��������
		{
			fgets(buffer, 100, file);
			//��ӡ��������
			printf("%s", buffer);
		}
		else if (cch == 'p')	//��Ϣ�У�ֹͣ��ӡ
		{
			fgets(buffer, 6, file);//����ǰ׺��ȡ
			break;
		}
	}
	fscanf(file, "%d %d", &(pcnf->numOfVariable), &(pcnf->numOfchildSentence));

	//��ȡ�ļ��������Ӿ�
	int cur_variable = 1;
	//��ȡ�׸��Ӿ�
	pChildSentence pcs = NULL;
	pWord pw = NULL;
	//��ȡ�׸���Ԫ
	fscanf(file, "%d ", &cur_variable);

	//�����׸��Ӿ䴢��ռ�
	pcnf->sentence = (pChildSentence)malloc(sizeof(childSentence));
	//��ʼ��
	pcs = pcnf->sentence;
	pcs->isCunzai = true;
	pcs->mark = 0;
	pcs->nextSen = NULL;
	pcs->numOfword = 0;
	pcs->words = NULL;


	if (cur_variable != 0)
	{


		//�����׸��Ӿ�ı�Ԫ����
		pcs->words = (pWord)malloc(sizeof(word));
		//��ʼ���͸�ֵ
		pw = pcs->words;
		pw->varibale = cur_variable;
		pw->nextword = NULL;
		pw->mark = true;
		pcs->numOfword++;
		//������ȡ����������
		while (1)
		{
			fscanf_s(file, "%d ", &cur_variable);
			if (cur_variable == 0)
				break;
			pw->nextword = (pWord)malloc(sizeof(word));
			//��ֵ
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
		//��ʼ��
		pcs = pcs->nextSen;
		pcs->isCunzai = true;
		pcs->nextSen = NULL;
		pcs->mark = 0;
		pcs->numOfword = 0;
		pcs->words = NULL;
		//��һ��Ԫ�ؾ���0�����Ӿ�
		if (cur_variable == 0) continue;
		//�����׸����ֽ��
		pcs->words = (pWord)malloc(sizeof(word));
		//��ֵ����ʼ��
		pw = pcs->words;
		pw->varibale = cur_variable;
		pw->mark = true;
		pw->nextword = NULL;
		pcs->numOfword++;

		//��ȡ��������ֲ��������
		while (1)
		{
			fscanf_s(file, "%d ", &cur_variable);
			if (cur_variable == 0)
				break;
			pw->nextword = (pWord)malloc(sizeof(word));
			//��ֵ
			pw = pw->nextword;
			pw->nextword = NULL;
			pw->varibale = cur_variable;
			pw->mark = true;
			pcs->numOfword++;
		}
	}
	return OK;
}
//����ջ���ݹ����·��
void destroyStack(pstack sta, bool isSuccess, int * answer, int &index)
{
	//�ɹ�ʱ���·����δ�ɹ�ʱ�����
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

//���ٵ�ǰ��cnf�ṹ
void destroyCNF(pCnfFile pcnf)
{
	//�����Ӿ�
	pChildSentence pcs = pcnf->sentence;
	pChildSentence pcs_l = pcs;
	/*while (pcs!=NULL)
	{
		pcs_l = pcs->nextSen;
		destroyChildScen(pcs);
		pcs = pcs_l;
	}*/ //����֮���bug
	pcnf->numOfchildSentence = 0;
	pcnf->numOfVariable = 0;
	pcnf->sentence = NULL;
	return;
}
//�����Ӿ�
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

//�����������pcnf��mask
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

//���res�ļ�
Status writeRes(char* fileName,bool isSuccess , int sumTime,int *answer,int n)
{
	//���ļ�����׺�ĳ�res
	int i = 0;
	printf("\n�Ƿ����res�ļ���(1-�ǣ�����-��)\n");
	scanf(" %d", &i);
	if (i != 1) return OK;
	i = 0;
	while (fileName[i] != '\0')		i++;//�ҵ���β
	fileName[i - 3] = '\0';
	strcat(fileName, "res");//�ı��ļ������
	

	FILE* fp = NULL;
	fp = fopen(fileName,"w");//���ļ�
	if (fp == NULL)	exit(-1);
	if (isSuccess == true)
		fprintf(fp, "s 1\n");
	else 
		fprintf(fp, "s 0\n");

	printf("�ļ�д��ɹ���\n");
	
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
