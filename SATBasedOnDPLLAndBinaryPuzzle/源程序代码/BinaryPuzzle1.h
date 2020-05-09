#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
//�궨������
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define OVERFLOW -2
#define OK 2
#define ADDR ".\\testFile\\"

typedef int Status;
typedef struct word;
typedef struct childSentence;
typedef struct cnfFile;

//����ͳ��cnf�����������Ľṹ
typedef struct count {
	int posi;		//���浱ǰ�ṹ�д��ڵ��������ֵĸ���
	int nega;		//��ǰ���ָ��ĸ���
}count,*pCount;
//count��ͷ���
typedef struct headCount {
	int max_variable;		//��ǰ�ṹ�и����������ֱ���
	int sum;					//�����������ֱ���������
	bool containPo;			//��ǵ�ǰ�������������Ƿ��������
	bool cintainNe;			//��ǵ�ǰ�ṹ���Ƿ���ڸ���
	pCount count;			//ָ��ͳ������
}headCount,*pHeadCount;
//�� ���cnf�ļ���������Ϣ
typedef struct cnfFile {
	int numOfVariable;//�漰�������ֵĸ���
	int numOfchildSentence;
	struct childSentence* sentence;
}cnfFile, *pCnfFile;

//�Ӿ���
typedef struct childSentence {
	struct childSentence * nextSen;//ָ����һ���Ӿ�
	struct word* words;				//ָ�����֣�������Ϊ��ʱ��˵��Ϊ���Ӿ�
	int numOfword;					//���Ӿ������ֵĸ���
	int mark;							//��Ǹýڵ��Ƿ�ɾ������markΪ0ʱ����ʾ�ýڵ���ڣ�����ýڵ��ֵ���ڷ��ѹ�����ߵ��Ӿ�����е��¸��־䱻ɾ��������
	bool isCunzai;						//�������������������ͼʱ����ǵ�ǰ�ո��Ƿ�ɾ����
}childSentence, *pChildSentence;

//���ֽ��
typedef struct word {
	int varibale;					//���ֱ�Ԫ
	bool mark;					//��ǣ����ڱ�Ǹýڵ��Ǳ�ɾ��
	struct word *nextword;	//������ָ����һ������
}word, *pWord;

//��ջ�����ڷǵݹ�ʵ��DPLL����
typedef struct stack {
	struct stackNode *top;
	struct stackNode *tail;
}*pstack;

typedef struct stackNode {
	int setTrueWord;			//�ڵ��Ӿ����ѡ�еĵ��Ӿ����֣������ڷ��ѹ�����ѡ�еļٶ�Ϊ�������
	bool methodMark;					//��Ǹôλ���ʹ����ʹ�õĹ���false--���Ӿ����ʹ�û���ʱֱ�ӻָ����ɣ�true--���ѹ���ʹ�û���ʱ��������
	stackNode* nextNode;
}stackNode, *pStackNode;
//���Ӿ��������־�Ľṹ,�����±���Ϊ����
//��ʼ��ջ
void initStack(pstack &sta);
//��ջ
void addStack(pstack sta, int word, bool methodMark);
//��ջ
Status outStack(pstack sta, int &word, bool &methodMark);
//����ջ
bool cheakStack(pstack sta);
//����ջԪ�ؽ��л���
Status recovery(pstack sta, pCnfFile pcnf);
//������򣬵��Ӿ��еĻ���/���ѹ����еĻ���
Status simplyS(pCnfFile pcnf, int word);
//����Ӿ����Ƿ���Ŀ������
bool checkWord(pChildSentence pcs, int word);
//ѡ����ѹ�����ʹ�õ����֣�ѡ��ṹ��ͬʱ����ԭ���֡������ֵ�����
int seclect(pCnfFile pcnf, int *count, bool &mark);
//Ѱ�ҵ��Ӿ䣬�ɹ��ҵ��򷵻ص��Ӿ�����֣����򷵻�0
int findSingal(pCnfFile pcnf);
//���S���Ƿ���ڿ��Ӿ�
bool checkNull(pCnfFile pcnf);
//DPLL�㷨����
Status DPLL(pCnfFile pcnf, pstack &stack,int beginTime,bool change);
//�ļ���ȡ����
Status readFile(pCnfFile pcnf,char *pFilename);
//���ļ������ṹ
Status readAndCreate(pCnfFile pcnf, FILE*file);
//����ջ���ݹ����·��
void destroyStack(pstack sta, bool isSuccess, int * answer, int &index);

void destroy(pStackNode psn, pstack sta, int &i, bool isSuccess, int * answer, int &index);
//���ٵ�ǰ��cnf�ṹ
void destroyCNF(pCnfFile pcnf);
//�����Ӿ�
void destroyChildScen(pChildSentence pcs);
//C��n,2�������
bool* printCn2(int n, int &len);
//����1~n�е�n/2+1������������������鱣��
bool* generateCombination(int n, int &len);

int countNumOfTrue(bool bol[], int n);
//����00000000~11111111�е���������λ�γɵ�bool����
bool* generateZuhe(int n, int &len);
//(1)����1��ͬ�С�ͬ�в�������������ͬ
void printCondition1(FILE * file, int n);
//(2)����2��ͬһ�л�ͬһ��0��1������ͬ������n/2 +1������ͬ
void printCondition2(FILE*file, int n);
//��3������3���������в���ͬ
void printCondition3(FILE*file, int n);

void printConfition(void);

void addNodeToPcnf(pCnfFile pcnf, int word);
//�����������pcnf��mask
void resetPcnf(pCnfFile pcnf);
//����������ͼ
void generateSoDuKuMap(pCnfFile pcnf, int n);
//д���ļ�
Status writeSoDoKufile(pCnfFile pcnf);

void printMap(int *digHole, int *answer, int n);
//�ڶ�����������ͼ,�������ɵĿյĸ���,n��ʾ����
void digMap(pCnfFile pcnf, int *digHole, int numOfHole, int n);
//�����Ϊnum��λ����Ϊ���෴�����鿴�Ƿ��н�,nΪ����
Status changeAndDPLL(pCnfFile pcnf, int num, int n);

pChildSentence findNum(pCnfFile pcnf, int num, bool &bol);
//��count�ṹ����ӱ���variable
void addToCount(pHeadCount phc, int variable);
//��count�ṹ��ɾ��variable
void delFormCount(pHeadCount phc, int variable, pCnfFile pcnf);
//�ļ���ȡ�����Ľ�--���ͳ��count�Ĳ������
Status readFile_1(pCnfFile pcnf, pHeadCount &phc,char * pFilename);
//���ļ������ṹ
Status readAndCreate_1(pCnfFile pcnf, FILE*file, pHeadCount &phc);
//ѡȡ��Ԫ��д
int select_1(pCnfFile pcnf, pHeadCount phc, bool &mark);
//�����Ӿ��д
Status simplyS_1(pCnfFile pcnf, int word, pHeadCount phc);
//DPLL��д
Status DPLL_1(pCnfFile pcnf, pstack &stack, pHeadCount phc);
//recovery������д
//����ջԪ�ؽ��л���
Status recovery_1(pstack sta, pCnfFile pcnf, pHeadCount phc);

void resetCount(pCnfFile pcnf, pHeadCount phc);

Status writeRes(char* fileName, bool isSuccess, int sumTime, int* answer, int n);