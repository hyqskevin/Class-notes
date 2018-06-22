// grammar2.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#pragma warning(disable:4996)

#define ADRLEN 512
//  �����ַ�Ͻ�
#define VNNUM 5
// ���ս����Ŀ 
#define VTNUM 6
// �ս����Ŀ 
#define NUMOFRIGHT 4
// ����ʽ�Ҳ���������Ŀ�Ͻ� 

char sInput[ADRLEN];	//���봮����
char sStack[ADRLEN];	//�﷨������ջ����
char sStack2[ADRLEN];   //ĩβ��\0���﷨������ջ����  ���ڴ�ӡ

int iStep;  //��ǰ����ֵ  ��ֵΪ0
int iTop;  //��ǰջ��ָ��ֵ  ��ֵΪ-1
int iInput;  //��ǰ���봮����ָ��ֵ  ��ֵΪ0
int iInLen;  //��ǰ���봮���鳤��

char cTop;   //��ǰջ���ַ�����:cTop  (X)
char cInput; 	//��ǰʣ�����봮���ַ�: cInput  (b)

char sLL1Table[VNNUM][VTNUM][NUMOFRIGHT];	// ��������ά�ַ�����


											//���ս��  Ϊ��ʵ���ϵķ��㣬E'��A���棻T'��B���档
enum VN_Type { E, A, T, B, F };
enum VN_Type vnCur;  //��ǰ���ս��  ���ڷ��ʷ�������ά�ַ�����ĵ�һά����


					 //�ս��: i,+,*,(,),$
enum VT_Type { id, plus, times, lparen, rparen, neq };
enum VT_Type vtCur;  //��ǰ�ս��  ���ڷ��ʷ�������ά�ַ�����ĵڶ�ά����
int Push(char cIn);  //ѹջ
int Pop();  //��ǰջ������:cTop (X)��ջ
void InitAll(); //��ʼ��:���ս�����顢��������ά�ַ������
void GetColRow();  //ȡ�÷��ʷ�������ά�ַ�����ĵ�һ��ά����vnCur vtCur
int GetInput();	//ȡ�����봮
int LL1_Analyze();   //LL(1)�﷨����
void Print();	//�����ǰ����������Ϣ
void version();		//��ʾ��Ȩ��Ϣ����
void Hint();		//��ʾʵ����ʾ��Ϣ���� 


int Push(char cIn)  //ѹջ
{
	if (iTop >= 512)
	{
		printf("\n	��ջ����!!!\n");
		return -1;
	}
	else
	{
		iTop++;  //��ǰջ��ָ��ֵ  ��ֵΪ-1
		sStack[iTop] = cIn;

		return 0;
	}

}  //Push()ѹջ


int Pop() //��ǰջ������:cTop (X)��ջ
{
	if (iTop < 0)
	{
		printf("\n	��ջ����!!!\n");
		return -1;
	}
	else
	{
		cTop = sStack[iTop];   //��ǰջ���ַ�����:cTop  (X)
		iTop--;

		return 0;
	}

}   //int Pop() //��ǰջ������:cTop (X)��ջ


void InitAll()	//��ʼ��:���ս�����顢��������ά�ַ������
{
	//Ϊʵ�ֺ���ʾ�ķ��㣬���ս��E'��A���棻T'��B���档��?��ʾ����
	//�Բ���ʽ�Ҳ���β����һ��' '����ʶ���ַ����Ľ����� 

	int iVN, iVT;
	char sVnTable[5][1];	// ���ս������

	sVnTable[0][0] = 'E';
	sVnTable[1][0] = 'A';
	sVnTable[2][0] = 'T';
	sVnTable[3][0] = 'B';
	sVnTable[4][0] = 'F';

	strcpy(&(sLL1Table[0][0][0]), "TA ");  //M[E,i] : (1) E -> TE'
	strcpy(&(sLL1Table[0][1][0]), "?");  //M[E,+] : 
	strcpy(&(sLL1Table[0][2][0]), "?");  //M[E,*] : 
	strcpy(&(sLL1Table[0][3][0]), "TA ");  //M[E,(] : (1) E -> TE'
	strcpy(&(sLL1Table[0][4][0]), "s");  //M[E,)] : 
	strcpy(&(sLL1Table[0][5][0]), "s");  //M[E,$] : 

	strcpy(&(sLL1Table[1][0][0]), "?");  //M[E',i] : 
	strcpy(&(sLL1Table[1][1][0]), "+TA ");  //M[E',+] : (2) E' -> +TE'
	strcpy(&(sLL1Table[1][2][0]), "?");  //M[E',*] : 
	strcpy(&(sLL1Table[1][3][0]), "?");  //M[E',(] : 
	strcpy(&(sLL1Table[1][4][0]), "��");  //M[E',)] : (3) E' -> ��
	strcpy(&(sLL1Table[1][5][0]), "��");  //M[E',$] : (3) E' -> ��

	strcpy(&(sLL1Table[2][0][0]), "FB ");  //M[T,i] : (4) T -> FT'
	strcpy(&(sLL1Table[2][1][0]), "s");  //M[T,+] : 
	strcpy(&(sLL1Table[2][2][0]), "?");  //M[T,*] : 
	strcpy(&(sLL1Table[2][3][0]), "FB ");  //M[T,(] : (4) T -> FT'
	strcpy(&(sLL1Table[2][4][0]), "s");  //M[T,)] : 
	strcpy(&(sLL1Table[2][5][0]), "s");  //M[T,$] : 

	strcpy(&(sLL1Table[3][0][0]), "?");  //M[T',i] : 
	strcpy(&(sLL1Table[3][1][0]), "��");  //M[T',+] : (6) T' -> ��
	strcpy(&(sLL1Table[3][2][0]), "*FB ");  //M[T',*] : (5) T' -> *FT'
	strcpy(&(sLL1Table[3][3][0]), "?");   //M[T',(] : 
	strcpy(&(sLL1Table[3][4][0]), "��");  //M[T',)] : (6) T' -> ��
	strcpy(&(sLL1Table[3][5][0]), "��");  //M[T',$] : (6) T' -> ��

	strcpy(&(sLL1Table[4][0][0]), "i ");  //M[F,i] : (8) F -> i
	strcpy(&(sLL1Table[4][1][0]), "s");  //M[F,+] : 
	strcpy(&(sLL1Table[4][2][0]), "s");  //M[F,*] : 
	strcpy(&(sLL1Table[4][3][0]), "(E) ");  //M[F,(] : (7) F -> (E)
	strcpy(&(sLL1Table[4][4][0]), "s");  //M[F,)] : 
	strcpy(&(sLL1Table[4][5][0]), "s");  //M[F,$] : 


	printf("\n\n				�������ʽ�ķ�\n\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(1) E -> TE'	(2) E' -> +TE'	(3) E' -> ��	(4) T -> FT'\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(5) T' -> *FT'	(6) T' -> ��	(7) F -> (E)	(8) F -> i\n");
	printf("	+-----------------------------------------------------------+\n\n");

	printf("			�������ʽ�ķ���LL(1)������\n\n");
	printf("	+-------------------------------------------------------+\n");
	printf("	|	i	+	*	(	)	$	|\n");
	printf("	+-------------------------------------------------------+\n");

	for (iVN = 0; iVN < VNNUM; iVN++)
	{
		printf("	|  %c 	", sVnTable[iVN][0]);
		for (iVT = 0; iVT < VTNUM; iVT++)
		{
			printf("%s	", sLL1Table[iVN][iVT]);

		}//for (iVT =0 ; iVT < VTNUM ; iVT++)

		printf("|\n");
	}  //for (iVN =0 ; iVN < VNNUM ; iVN++)
	printf("	+-------------------------------------------------------+\n\n");

}  //InitAll()��ʼ��:���ս�����顢��������ά�ַ������


void GetColRow()  //ȡ�÷��ʷ�������ά�ַ�����ĵ�һ��ά����vnCur vtCur
{
	switch (cTop) //ȡ�÷��ʷ�������ά�ַ�����ĵ�һά����vnCur
	{
	case 'E':
	{
		vnCur = E;

	}  //case 
	break;

	case 'A':     //  E'
	{
		vnCur = A;

	}  //case 
	break;

	case  'T':
	{
		vnCur = T;

	}  //case 
	break;

	case  'B':   //T'
	{
		vnCur = B;

	}  //case 
	break;

	case  'F':
	{
		vnCur = F;

	}  //case 
	break;
	}  //switch( cTop )ȡ�÷��ʷ�������ά�ַ�����ĵ�һά����vnCur	 


	switch (cInput) //ȡ�÷��ʷ�������ά�ַ�����ĵڶ�ά����vtCur
	{
	case 'i':
	{
		vtCur = id;

	}  //case 
	break;

	case  '+':
	{
		vtCur = plus;

	}  //case 
	break;

	case  '*':
	{
		vtCur = times;

	}  //case 
	break;

	case  '(':
	{
		vtCur = lparen;

	}  //case 
	break;

	case  ')':
	{
		vtCur = rparen;

	}  //case 
	break;

	case  '$':
	{
		vtCur = neq;

	}  //case 
	break;

	}  //switch( cInput )  ȡ�÷��ʷ�������ά�ַ�����ĵڶ�ά����vtCur

}//GetColRow//ȡ�÷��ʷ�������ά�ַ�����ĵ�һ��ά����vnCur vtCur


int GetInput()	//ȡ�����봮
{
	int iTemp;
	char cCurChar;   //��ǰ�ַ�

	strcpy(sInput, "");	//���봮�ַ�����

	printf("\n	������Ҫ�����ĵ��ʣ��ս�����������磺i+i*i���������ַ� '$' ����:\n\n");

	iTemp = 0;
	cCurChar = getchar();
	while (cCurChar != '$')
	{
		if ((cCurChar != ' ') && (cCurChar != '$') && (cCurChar != '\n'))	//  ���Կո��TAB�����з�
		{
			sInput[iTemp] = cCurChar;
			iTemp++;
		}
		cCurChar = getchar();

	}  //while (cCurChar != '9')

	sInput[iTemp] = '$';
	sInput[iTemp + 1] = 0;  	//���봮�ַ�����β����ASCII  0
	iInLen = iTemp + 1;		//��ǰ���봮���鳤��


	if (iInLen <= 1)
	{
		printf("	��Ч���봮Ϊ��!!!\n\n");
		return -1;

	}	//if (iInLen<=1)
	return 0;

}	//void GetInput();	//ȡ�����봮


int LL1_Analyze()	//LL(1)�﷨����
{
	int iTemp;
	bool bNotAccepted;  //��δ�������봮Ϊ�Ϸ����ӵı�־��FLAG

	if (GetInput() == -1) return -1;	//ȡ�����봮

	iInput = 0;	//��ǰ���봮����ָ��ֵ  ��ֵΪ0	
	iTop = -1;	//��ǰջ��ָ��ֵ	
	iStep = 0;	//��ǰ����ֵ  ��ֵΪ0

	printf("\n\n	�Դ����봮��    %s    ��LL(1)�﷨�����������£�\n\n", sInput);

	printf("   +------------------------------------------------------------------------+\n");
	printf("   |  ����       ����ջ����    ջ������X    ��ǰ����b    ������    M[X,b]   |\n");
	printf("   +------------------------------------------------------------------------+\n");

	Push('$');
	Push('E');

	cInput = sInput[iInput];	//��ǰʣ�����봮���ַ�: cInput  (b)

	bNotAccepted = true;  //��δ�������봮Ϊ�Ϸ����ӵı�־��FLAG

	while (bNotAccepted)
	{
		for (iTemp = 0; iTemp < iTop + 1; iTemp++)
		{
			sStack2[iTemp] = sStack[iTemp];
		}
		sStack2[iTemp] = 0;  //ĩβ��\0���﷨������ջ����  ���ڴ�ӡ


		Pop();	//��ǰջ������:cTop (X)��ջ
		if ((cTop == 'i') || (cTop == '+') || (cTop == '*') || (cTop == '(') || (cTop == ')'))
			//if1
		{
			if (cTop == cInput)  //�ַ�ƥ��
								 //if1-1
			{
				iStep++;
				Print();	//�����ǰ����������Ϣ

				iInput++;
				cInput = sInput[iInput];	//��ǰʣ�����봮���ַ�: cInput  (b)

			}
			else	//if1-1
			{
				printf("   +------------------------------------------------------------------------+\n\n");
				printf("	��ǰջ������:%c���뵱ǰʣ�����봮���ַ�:%c��ƥ��!!!\n\n", cTop, cInput);
				printf("   +------------------------------------------------------------------------+\n\n");
				return -1;
			}//if1-1
		}
		else	//if1
		{
			if (cTop == '$')
				//if1-2
			{
				if (cTop == cInput)  //�ַ�'$'ƥ��
									 //if1-2-1
				{
					bNotAccepted = false;

					iStep++;
					Print();	//�����ǰ����������Ϣ
				}
				else	//if1-2-1
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	��ǰջ������:$�������ַ�����ǿ�!!!\n\n");
					printf("   +------------------------------------------------------------------------+\n\n");
					return -1;

				}//if1-2-1
			}
			else	//if1-2
			{
				if ((cTop == 'E') || (cTop == 'A') || (cTop == 'T') || (cTop == 'B') || (cTop == 'F') )
					//if1-2-2
				{
					if ((cInput == 'i') || (cInput == '+') || (cInput == '*') || (cInput == '(') || (cInput == ')') || (cInput == '$'))
						//if1-2-2-1
					{
						GetColRow();	//ȡ�÷��ʷ�������ά�ַ�����ĵ�һ��ά����vnCur vtCur
					}
					else	//if1-2-2-1
					{
						printf("   +------------------------------------------------------------------------+\n\n");
						printf("	��ǰ������ţ� %c  ����!!!\n\n", cInput);
						printf("   +------------------------------------------------------------------------+\n\n");
						return -1;

					}	//if1-2-2-1

					if ((strcmp(sLL1Table[vnCur][vtCur], "?") == 0) || (strcmp(sLL1Table[vnCur][vtCur], "��") == 0))
						//if1-2-2-2
						//�ղ���ʽ-->�Ų�����ջ!
					{
						iStep++;
						Print();	//�����ǰ����������Ϣ

						if (strcmp(sLL1Table[vnCur][vtCur], "?") == 0)
							//if1-2-2-2-1				
						{
							printf("   +------------------------------------------------------------------------+\n\n");
							printf("	��������!!!\n\n");
							printf("   +------------------------------------------------------------------------+\n\n");
							return -1;
						}
						
					}
					else	//����ʽ�Ҳ���������ջ: �Ƶ�
					{
						iTemp = 0;
						while (sLL1Table[vnCur][vtCur][iTemp] != ' ')
							//�Բ���ʽ�Ҳ���β����һ��' '����ʶ���ַ����Ľ�����				
						{
							iTemp++;
						}// while1

						while (iTemp > 0)
						{
							iTemp--;
							Push(sLL1Table[vnCur][vtCur][iTemp]);
						}// while2

						iStep++;
						Print();	//�����ǰ����������Ϣ
					}	//if1-2-2-2
				}

				else if ((strcmp(sLL1Table[vnCur][vtCur], "s") == 0))
				{
					Push(sLL1Table[vnCur][vtCur][vtCur]);
					Push(cInput);
					//GetColRow();
				}

				else	//if1-2-2
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	��ǰջ�����ţ� %c  ����!!!\n\n", cTop);
					printf("   +------------------------------------------------------------------------+\n\n");
					return -1;

				}	//if1-2-2

			}//if1-2

		}  //if1

	}  //while(bNotAccepted)
	printf("   +------------------------------------------------------------------------+\n\n");

	return 0;

}  //int LL1_Analyze()  LL(1)�﷨����


void Print()	//�����ǰ����������Ϣ
{
	int iTemp, iRemain;
	char sRemain[ADRLEN];   //�����ַ�����

							//���������ַ�����
	iRemain = 0;
	iTemp = iInput;
	while (iTemp <= iInLen)		//��ǰ���봮���鳤��
	{
		sRemain[iRemain] = sInput[iTemp];
		iTemp++;
		iRemain++;

	}// while

	sRemain[iTemp] = 0;	//�����ַ�����β����ASCII  0

	if ((cTop == 'i') || (cTop == '+') || (cTop == '*') || (cTop == '(') || (cTop == ')'))
		//if1
	{
		printf("\n	%d       %s    %c    %c    %s    '%c'ƥ��\n", iStep, sStack2, cTop, cInput, sRemain, cInput);
	}
	else
	{
		if (cTop == '$')
			//if2
		{
			printf("\n	%d       %s    %c    %c    %s    ����\n", iStep, sStack2, cTop, cInput, sRemain);
		}
		else
		{
			printf("\n	%d       %s    %c    %c    %s    %c -> %s\n", iStep, sStack2, cTop, cInput, sRemain, cTop, sLL1Table[vnCur][vtCur]);
		}//if2
	}////if1

}	//void Print()�����ǰ����������Ϣ



void Hint() //��ʾʵ����ʾ��Ϣ���� 
{
	printf("\n");

	printf("	G[E]:                                                                        \n");
	printf("	(1) E -> TE' (2) E' -> +TE' (3) E' -> ��                                     \n");
	printf("	(4) T -> FT' (5) T' -> *FT' (6) T' -> ��                                     \n");
	printf("	(7) F -> (E) (8) F -> i                                                      \n");
	printf("			i	+	*	(	)	$												\n");
	printf("		E	(1)			(1)														\n");
	printf("		E'		(2)			(3)	(3)												\n");
	printf("		T	(4)			(4)														\n");
	printf("		T'		(6)	(5)		(6)	(6)												\n");
	printf("		F	(8)			(7)														\n");
	printf("\n");

} //void Hint() 


void main()
{
	bool bGoOn;
	char sGoOn[1];

	Hint(); //��ʾʵ����ʾ��Ϣ���� 

	InitAll();	//��ʼ��:���ս�����顢��������ά�ַ������

	bGoOn = true;

	while (bGoOn)
	{
		if (LL1_Analyze() == -1)    //LL(1)�﷨����
		{
			printf("	����������Դ����봮��    %s    ��LL(1)�﷨��������\n\n", sInput);
			printf("   +------------------------------------------------------------------------+\n\n");
		}
		else
		{
			printf("	����������Դ����봮��    %s    ��LL(1)�﷨�����ɹ�\n\n", sInput);
			printf("   +------------------------------------------------------------------------+\n\n");
		}

		bGoOn = false;
		strcpy(sGoOn, " ");

		printf("	Ҫ���������������ʽ�ķ���LL(1)�﷨������?(Y/N)\n\n");
		scanf("%s", sGoOn);
		bGoOn = (sGoOn[0] == 'y' || sGoOn[0] == 'Y');

	}  //while (bGoOn)

}  //main()

