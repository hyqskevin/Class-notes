// grammar2.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#pragma warning(disable:4996)

#define ADRLEN 512
//  数组地址上界
#define VNNUM 5
// 非终结符数目 
#define VTNUM 6
// 终结符数目 
#define NUMOFRIGHT 4
// 产生式右部串符号数目上界 

char sInput[ADRLEN];	//输入串数组
char sStack[ADRLEN];	//语法分析堆栈数组
char sStack2[ADRLEN];   //末尾加\0的语法分析堆栈数组  用于打印

int iStep;  //当前步骤值  初值为0
int iTop;  //当前栈顶指针值  初值为-1
int iInput;  //当前输入串数组指针值  初值为0
int iInLen;  //当前输入串数组长度

char cTop;   //当前栈顶字符符号:cTop  (X)
char cInput; 	//当前剩余输入串首字符: cInput  (b)

char sLL1Table[VNNUM][VTNUM][NUMOFRIGHT];	// 分析表三维字符数组


											//非终结符  为了实现上的方便，E'用A代替；T'用B代替。
enum VN_Type { E, A, T, B, F };
enum VN_Type vnCur;  //当前非终结符  用于访问分析表三维字符数组的第一维坐标


					 //终结符: i,+,*,(,),$
enum VT_Type { id, plus, times, lparen, rparen, neq };
enum VT_Type vtCur;  //当前终结符  用于访问分析表三维字符数组的第二维坐标
int Push(char cIn);  //压栈
int Pop();  //当前栈顶符号:cTop (X)出栈
void InitAll(); //初始化:非终结符数组、分析表三维字符数组等
void GetColRow();  //取得访问分析表三维字符数组的第一二维坐标vnCur vtCur
int GetInput();	//取得输入串
int LL1_Analyze();   //LL(1)语法分析
void Print();	//输出当前步骤的相关信息
void version();		//显示版权信息函数
void Hint();		//显示实验提示信息函数 


int Push(char cIn)  //压栈
{
	if (iTop >= 512)
	{
		printf("\n	堆栈上溢!!!\n");
		return -1;
	}
	else
	{
		iTop++;  //当前栈顶指针值  初值为-1
		sStack[iTop] = cIn;

		return 0;
	}

}  //Push()压栈


int Pop() //当前栈顶符号:cTop (X)出栈
{
	if (iTop < 0)
	{
		printf("\n	堆栈下溢!!!\n");
		return -1;
	}
	else
	{
		cTop = sStack[iTop];   //当前栈顶字符符号:cTop  (X)
		iTop--;

		return 0;
	}

}   //int Pop() //当前栈顶符号:cTop (X)出栈


void InitAll()	//初始化:非终结符数组、分析表三维字符数组等
{
	//为实现和显示的方便，非终结符E'用A代替；T'用B代替。用?表示出错。
	//对产生式右部串尾部加一个' '用以识别字符串的结束。 

	int iVN, iVT;
	char sVnTable[5][1];	// 非终结符数组

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
	strcpy(&(sLL1Table[1][4][0]), "ε");  //M[E',)] : (3) E' -> ε
	strcpy(&(sLL1Table[1][5][0]), "ε");  //M[E',$] : (3) E' -> ε

	strcpy(&(sLL1Table[2][0][0]), "FB ");  //M[T,i] : (4) T -> FT'
	strcpy(&(sLL1Table[2][1][0]), "s");  //M[T,+] : 
	strcpy(&(sLL1Table[2][2][0]), "?");  //M[T,*] : 
	strcpy(&(sLL1Table[2][3][0]), "FB ");  //M[T,(] : (4) T -> FT'
	strcpy(&(sLL1Table[2][4][0]), "s");  //M[T,)] : 
	strcpy(&(sLL1Table[2][5][0]), "s");  //M[T,$] : 

	strcpy(&(sLL1Table[3][0][0]), "?");  //M[T',i] : 
	strcpy(&(sLL1Table[3][1][0]), "ε");  //M[T',+] : (6) T' -> ε
	strcpy(&(sLL1Table[3][2][0]), "*FB ");  //M[T',*] : (5) T' -> *FT'
	strcpy(&(sLL1Table[3][3][0]), "?");   //M[T',(] : 
	strcpy(&(sLL1Table[3][4][0]), "ε");  //M[T',)] : (6) T' -> ε
	strcpy(&(sLL1Table[3][5][0]), "ε");  //M[T',$] : (6) T' -> ε

	strcpy(&(sLL1Table[4][0][0]), "i ");  //M[F,i] : (8) F -> i
	strcpy(&(sLL1Table[4][1][0]), "s");  //M[F,+] : 
	strcpy(&(sLL1Table[4][2][0]), "s");  //M[F,*] : 
	strcpy(&(sLL1Table[4][3][0]), "(E) ");  //M[F,(] : (7) F -> (E)
	strcpy(&(sLL1Table[4][4][0]), "s");  //M[F,)] : 
	strcpy(&(sLL1Table[4][5][0]), "s");  //M[F,$] : 


	printf("\n\n				算术表达式文法\n\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(1) E -> TE'	(2) E' -> +TE'	(3) E' -> ε	(4) T -> FT'\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(5) T' -> *FT'	(6) T' -> ε	(7) F -> (E)	(8) F -> i\n");
	printf("	+-----------------------------------------------------------+\n\n");

	printf("			算术表达式文法的LL(1)分析表\n\n");
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

}  //InitAll()初始化:非终结符数组、分析表三维字符数组等


void GetColRow()  //取得访问分析表三维字符数组的第一二维坐标vnCur vtCur
{
	switch (cTop) //取得访问分析表三维字符数组的第一维坐标vnCur
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
	}  //switch( cTop )取得访问分析表三维字符数组的第一维坐标vnCur	 


	switch (cInput) //取得访问分析表三维字符数组的第二维坐标vtCur
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

	}  //switch( cInput )  取得访问分析表三维字符数组的第二维坐标vtCur

}//GetColRow//取得访问分析表三维字符数组的第一二维坐标vnCur vtCur


int GetInput()	//取得输入串
{
	int iTemp;
	char cCurChar;   //当前字符

	strcpy(sInput, "");	//输入串字符数组

	printf("\n	请输入要分析的单词（终结符）串（例如：i+i*i）以数字字符 '$' 结束:\n\n");

	iTemp = 0;
	cCurChar = getchar();
	while (cCurChar != '$')
	{
		if ((cCurChar != ' ') && (cCurChar != '$') && (cCurChar != '\n'))	//  忽略空格和TAB及换行符
		{
			sInput[iTemp] = cCurChar;
			iTemp++;
		}
		cCurChar = getchar();

	}  //while (cCurChar != '9')

	sInput[iTemp] = '$';
	sInput[iTemp + 1] = 0;  	//输入串字符数组尾部加ASCII  0
	iInLen = iTemp + 1;		//当前输入串数组长度


	if (iInLen <= 1)
	{
		printf("	有效输入串为空!!!\n\n");
		return -1;

	}	//if (iInLen<=1)
	return 0;

}	//void GetInput();	//取得输入串


int LL1_Analyze()	//LL(1)语法分析
{
	int iTemp;
	bool bNotAccepted;  //尚未接受输入串为合法句子的标志：FLAG

	if (GetInput() == -1) return -1;	//取得输入串

	iInput = 0;	//当前输入串数组指针值  初值为0	
	iTop = -1;	//当前栈顶指针值	
	iStep = 0;	//当前步骤值  初值为0

	printf("\n\n	对此输入串：    %s    的LL(1)语法分析过程如下：\n\n", sInput);

	printf("   +------------------------------------------------------------------------+\n");
	printf("   |  步骤       分析栈内容    栈顶符号X    当前输入b    余留串    M[X,b]   |\n");
	printf("   +------------------------------------------------------------------------+\n");

	Push('$');
	Push('E');

	cInput = sInput[iInput];	//当前剩余输入串首字符: cInput  (b)

	bNotAccepted = true;  //尚未接受输入串为合法句子的标志：FLAG

	while (bNotAccepted)
	{
		for (iTemp = 0; iTemp < iTop + 1; iTemp++)
		{
			sStack2[iTemp] = sStack[iTemp];
		}
		sStack2[iTemp] = 0;  //末尾加\0的语法分析堆栈数组  用于打印


		Pop();	//当前栈顶符号:cTop (X)出栈
		if ((cTop == 'i') || (cTop == '+') || (cTop == '*') || (cTop == '(') || (cTop == ')'))
			//if1
		{
			if (cTop == cInput)  //字符匹配
								 //if1-1
			{
				iStep++;
				Print();	//输出当前步骤的相关信息

				iInput++;
				cInput = sInput[iInput];	//当前剩余输入串首字符: cInput  (b)

			}
			else	//if1-1
			{
				printf("   +------------------------------------------------------------------------+\n\n");
				printf("	当前栈顶符号:%c，与当前剩余输入串首字符:%c不匹配!!!\n\n", cTop, cInput);
				printf("   +------------------------------------------------------------------------+\n\n");
				return -1;
			}//if1-1
		}
		else	//if1
		{
			if (cTop == '$')
				//if1-2
			{
				if (cTop == cInput)  //字符'$'匹配
									 //if1-2-1
				{
					bNotAccepted = false;

					iStep++;
					Print();	//输出当前步骤的相关信息
				}
				else	//if1-2-1
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	当前栈顶符号:$，余留字符数组非空!!!\n\n");
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
						GetColRow();	//取得访问分析表三维字符数组的第一二维坐标vnCur vtCur
					}
					else	//if1-2-2-1
					{
						printf("   +------------------------------------------------------------------------+\n\n");
						printf("	当前输入符号： %c  出错!!!\n\n", cInput);
						printf("   +------------------------------------------------------------------------+\n\n");
						return -1;

					}	//if1-2-2-1

					if ((strcmp(sLL1Table[vnCur][vtCur], "?") == 0) || (strcmp(sLL1Table[vnCur][vtCur], "ε") == 0))
						//if1-2-2-2
						//空产生式-->ε不用入栈!
					{
						iStep++;
						Print();	//输出当前步骤的相关信息

						if (strcmp(sLL1Table[vnCur][vtCur], "?") == 0)
							//if1-2-2-2-1				
						{
							printf("   +------------------------------------------------------------------------+\n\n");
							printf("	分析表报错!!!\n\n");
							printf("   +------------------------------------------------------------------------+\n\n");
							return -1;
						}
						
					}
					else	//产生式右部串反序入栈: 推导
					{
						iTemp = 0;
						while (sLL1Table[vnCur][vtCur][iTemp] != ' ')
							//对产生式右部串尾部加一个' '用以识别字符串的结束。				
						{
							iTemp++;
						}// while1

						while (iTemp > 0)
						{
							iTemp--;
							Push(sLL1Table[vnCur][vtCur][iTemp]);
						}// while2

						iStep++;
						Print();	//输出当前步骤的相关信息
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
					printf("	当前栈顶符号： %c  出错!!!\n\n", cTop);
					printf("   +------------------------------------------------------------------------+\n\n");
					return -1;

				}	//if1-2-2

			}//if1-2

		}  //if1

	}  //while(bNotAccepted)
	printf("   +------------------------------------------------------------------------+\n\n");

	return 0;

}  //int LL1_Analyze()  LL(1)语法分析


void Print()	//输出当前步骤的相关信息
{
	int iTemp, iRemain;
	char sRemain[ADRLEN];   //余留字符数组

							//生成余留字符数组
	iRemain = 0;
	iTemp = iInput;
	while (iTemp <= iInLen)		//当前输入串数组长度
	{
		sRemain[iRemain] = sInput[iTemp];
		iTemp++;
		iRemain++;

	}// while

	sRemain[iTemp] = 0;	//余留字符数组尾部加ASCII  0

	if ((cTop == 'i') || (cTop == '+') || (cTop == '*') || (cTop == '(') || (cTop == ')'))
		//if1
	{
		printf("\n	%d       %s    %c    %c    %s    '%c'匹配\n", iStep, sStack2, cTop, cInput, sRemain, cInput);
	}
	else
	{
		if (cTop == '$')
			//if2
		{
			printf("\n	%d       %s    %c    %c    %s    接受\n", iStep, sStack2, cTop, cInput, sRemain);
		}
		else
		{
			printf("\n	%d       %s    %c    %c    %s    %c -> %s\n", iStep, sStack2, cTop, cInput, sRemain, cTop, sLL1Table[vnCur][vtCur]);
		}//if2
	}////if1

}	//void Print()输出当前步骤的相关信息



void Hint() //显示实验提示信息函数 
{
	printf("\n");

	printf("	G[E]:                                                                        \n");
	printf("	(1) E -> TE' (2) E' -> +TE' (3) E' -> ε                                     \n");
	printf("	(4) T -> FT' (5) T' -> *FT' (6) T' -> ε                                     \n");
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

	Hint(); //显示实验提示信息函数 

	InitAll();	//初始化:非终结符数组、分析表三维字符数组等

	bGoOn = true;

	while (bGoOn)
	{
		if (LL1_Analyze() == -1)    //LL(1)语法分析
		{
			printf("	分析结果：对此输入串：    %s    的LL(1)语法分析出错\n\n", sInput);
			printf("   +------------------------------------------------------------------------+\n\n");
		}
		else
		{
			printf("	分析结果：对此输入串：    %s    的LL(1)语法分析成功\n\n", sInput);
			printf("   +------------------------------------------------------------------------+\n\n");
		}

		bGoOn = false;
		strcpy(sGoOn, " ");

		printf("	要继续进行算术表达式文法的LL(1)语法分析吗?(Y/N)\n\n");
		scanf("%s", sGoOn);
		bGoOn = (sGoOn[0] == 'y' || sGoOn[0] == 'Y');

	}  //while (bGoOn)

}  //main()

