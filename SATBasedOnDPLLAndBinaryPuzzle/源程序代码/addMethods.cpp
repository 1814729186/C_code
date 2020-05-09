#include"BinaryPuzzle1.h"
/**
*�ı䴢��ṹʵ��DPLL�����Ч�ʸĽ������иĽ��ĺ���
*/

//��count�ṹ����ӱ���variable
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
	//���ĵ�ǰ�ṹ�е����ı���
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

//��count�ṹ��ɾ��variable
void delFormCount(pHeadCount phc, int variable, pCnfFile pcnf)
{
	//�ӽṹ��ɾ��һ��
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
	//���µ�ǰ�ṹ�е����ı���
	if (phc->max_variable == cur_vari)
	{
		phc->sum--;
		if (phc->count[cur_vari].nega == 0)
			phc->cintainNe = false;
		if (phc->count[cur_vari].posi == 0)
			phc->containPo = false;
		for (int i = 1; i <= pcnf->numOfVariable; i++)
		{
			//�������ж��Ƿ���ڳ���sum�ı�������
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

//�ļ���ȡ�����Ľ�--���ͳ��count�Ĳ������
Status readFile_1(pCnfFile pcnf,pHeadCount &_phc,char*pFilename)
{
	char filename[200], *ch, cch;
	char addrFilename[100] = ADDR;

	printf("�����ļ����ƣ�");
	scanf("%s", filename);
	//ƴ�ӵ�ַ+�ļ���
	int i = 0;
	while (addrFilename[i] != '\0')	i++;
	for (int j = 0; filename[j] != '\0'; j++)
		addrFilename[i++] = filename[j];
	addrFilename[i] = '\0';
	strcpy(pFilename, addrFilename);

	FILE* file;
	//���ļ�
	if (!(file = fopen(addrFilename, "r")))
	{
		return ERROR;
	}
	readAndCreate_1(pcnf, file,_phc);
	fclose(file);
	//����������
	//pcnf�ļ���ɴ���
	return OK;

}

//���ļ������ṹ
Status readAndCreate_1(pCnfFile pcnf, FILE*file,pHeadCount &_phc)
{
	char buffer[100];						//�ַ���������
	int index = 0;						//�ַ�����ָ��
	char cch, *ch;
	pHeadCount phc = (pHeadCount)malloc(sizeof(headCount));
	//��ʼ��
	phc->cintainNe = false;
	phc->containPo = false;
	phc->max_variable = 0;
	phc->sum = 0;
	_phc = phc;
	
	printf("�ļ�ע�����£�\n");
	//���ж�ȡ�ļ�
	while (1)
	{
		cch = fgetc(file);
		if (cch == 'c')	//ע����,��ӡ��������
		{
			fgets(buffer,100,file);
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
	//��������ṹ
	phc->count = (pCount)malloc(sizeof(count)*(pcnf->numOfVariable + 1));
	//��ʼ��
	for (int i = 0; i <= pcnf->numOfVariable; i++)
	{
		phc->count[i].nega = 0;
		phc->count[i].posi = 0;
	}
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

		//���浽count��ȥ
		addToCount(phc,cur_variable);
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
			//���浽count��ȥ
			addToCount(phc, cur_variable);
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
		//���浽count��ȥ
		addToCount(phc, cur_variable);
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
			//���浽count��ȥ
			addToCount(phc, cur_variable);
		}
	}
	return OK;
}

//DPLL��д
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
			if (pcnf->numOfchildSentence == 0)	//����Ϊ��
			{
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
			simplyS_1(pcnf, cur_word, phc);
		}

		if (status == true)		//���ڿ��Ӿ䣬����
		{
			if (recovery_1(sta, pcnf,phc) == ERROR)
			{
				return FALSE;
			}
		}
		else {
			//ѡȡ��Ԫ�����з��Ѳ���
			cur_word = select_1(pcnf, phc, mark);
			if (cur_word == 0)	exit(-2);
			addStack(sta, cur_word, mark);
			simplyS_1(pcnf, cur_word,phc);
		}
	} while (cheakStack(sta) == FALSE);
	return FALSE;
}

//�����Ӿ亯����д
Status simplyS_1(pCnfFile pcnf, int word,pHeadCount phc)
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
			pw = pcs->words;
			while (pw)
			{
				//��count�ṹ��ɾ��һ����¼
				if (pw->mark == true)
				{
					delFormCount(phc,pw->varibale,pcnf);
				}
				pw = pw->nextword;
			}
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
					//��count�ṹ��ɾ��һ����¼
					delFormCount(phc, pw->varibale, pcnf);

				}
				pw = pw->nextword;
			}
		}
		pcs = pcs->nextSen;
	}
	return OK;
}

//ѡȡ��Ԫ��д
int select_1(pCnfFile pcnf, pHeadCount phc, bool &mark)
{
	int res = 0;
	//�õ��ṹ��������������
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

//recovery������д
//����ջԪ�ؽ��л���
Status recovery_1(pstack sta, pCnfFile pcnf,pHeadCount phc)
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
	//����-wordΪ�棬���ù��򻯼򼯺ϣ��˴���ջʱ�������Ӿ������֮����ݵ���һ���Ͳ����ٴ�������֧
	//��ջ
	addStack(sta, (-1)*word, false);
	simplyS_1(pcnf, (-1)*word, phc);
	return OK;
}

//count���ݻָ�������pcnf���
void resetCount(pCnfFile pcnf, pHeadCount phc)
{
	//��ʼ��
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