// grammar.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#pragma warning(disable:4996)

#define vn_A 5	// 非终结符数目 
#define vt_a 6	// 终结符数目 

char input[64];	//输入大小为64
char stack[64];	//栈大小定义为64
char stack2[64];
char m[5][6][4];	//m[vn_A][vt_a][num]

int step;  //步骤值  初值为0
int int_top;  //栈顶指针值  初值-1
int int_input;  //输入串数组指针值  初值为0
int len;  //输入串数组长度
char char_top;   //栈顶字符符号:char_top(X)
char char_input; 	//当前剩余输入串首字符: char_input(b)

//非终结符  E'用A替；T'用B替。
enum vn_type { E, A, T, B, F };
enum vn_type vn;  //当前非终结符  用于访问分析表数组的第一维坐标

//终结符: i,+,*,(,),$
enum vt_type {id, add, multi, left, right, end};
enum vt_type vt;  //当前终结符  用于访问分析表数组的第二维坐标

int push(const char in);
int pop();
void init();
int get_input();
void get_m();
int ll1();
void Print();

int push(const char in)  //压栈
{
	if (int_top >= 64)
	{
		printf("stack overflow!\n");
		return -1;
	}
	else
	{
		int_top++;  //当前栈顶指针值  初值为-1
		stack[int_top] = in;
		return 0;
	}
}

int pop() //出栈
{
	if (char_top < 0)
	{
		printf("stack overflow!\n");
		return -1;
	}
	else
	{
		char_top = stack[int_top];   //当前栈顶字符符号:char_top (X)
		int_top--;
		return 0;
	}
}

void init()
{
	char vn_table[5][1];	// 非终结符数组

	vn_table[0][0] = 'E';
	vn_table[1][0] = 'A';
	vn_table[2][0] = 'T';
	vn_table[3][0] = 'B';
	vn_table[4][0] = 'F';

	// ReSharper disable CppDeprecatedEntity
	strcpy(&(m[0][0][0]), "TA ");  //M[E,i] : (1) E -> TE'
	strcpy(&(m[0][1][0]), "?");  //M[E,+] : 
	strcpy(&(m[0][2][0]), "?");  //M[E,*] : 
	strcpy(&(m[0][3][0]), "TA ");  //M[E,(] : (1) E -> TE'
	strcpy(&(m[0][4][0]), "synch");  //M[E,)] : 
	strcpy(&(m[0][5][0]), "synch");  //M[E,$] : 

	strcpy(&(m[1][0][0]), "?");  //M[E',i] : 
	strcpy(&(m[1][1][0]), "+TA ");  //M[E',+] : (2) E' -> +TE'
	strcpy(&(m[1][2][0]), "?");  //M[E',*] : 
	strcpy(&(m[1][3][0]), "?");  //M[E',(] : 
	strcpy(&(m[1][4][0]), "ε");  //M[E',)] : (3) E' -> ε
	strcpy(&(m[1][5][0]), "ε");  //M[E',$] : (3) E' -> ε

	strcpy(&(m[2][0][0]), "FB ");  //M[T,i] : (4) T -> FT'
	strcpy(&(m[2][1][0]), "synch");  //M[T,+] : 
	strcpy(&(m[2][2][0]), "?");  //M[T,*] : 
	strcpy(&(m[2][3][0]), "FB ");  //M[T,(] : (4) T -> FT'
	strcpy(&(m[2][4][0]), "synch");  //M[T,)] : 
	strcpy(&(m[2][5][0]), "synch");  //M[T,$] : 

	strcpy(&(m[3][0][0]), "?");  //M[T',i] : 
	strcpy(&(m[3][1][0]), "ε");  //M[T',+] : (6) T' -> ε
	strcpy(&(m[3][2][0]), "*FB ");  //M[T',*] : (5) T' -> *FT'
	strcpy(&(m[3][3][0]), "?");   //M[T',(] : 
	strcpy(&(m[3][4][0]), "ε");  //M[T',)] : (6) T' -> ε
	strcpy(&(m[3][5][0]), "ε");  //M[T',$] : (6) T' -> ε

	strcpy(&(m[4][0][0]), "i ");  //M[F,i] : (8) F -> i
	strcpy(&(m[4][1][0]), "synch");  //M[F,+] : 
	strcpy(&(m[4][2][0]), "synch");  //M[F,*] : 
	strcpy(&(m[4][3][0]), "(E) ");  //M[F,(] : (7) F -> (E)
	strcpy(&(m[4][4][0]), "synch");  //M[F,)] : 
	strcpy(&(m[4][5][0]), "synch");  //M[F,$] : 
	// ReSharper restore CppDeprecatedEntity

	printf("\n\n				算术表达式文法\n\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(1) E -> TE'	(2) E' -> +TE'	(3) E' -> ε	(4) T -> FT'\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(5) T' -> *FT'	(6) T' -> ε	(7) F -> (E)	(8) F -> i\n");
	printf("	+-----------------------------------------------------------+\n\n");

	printf("\n\n			算术表达式文法的LL(1)分析表\n\n");
	printf("	+-------------------------------------------------------+\n");
	printf("	|	i	+	*	(	)	$	|\n");
	printf("	+-------------------------------------------------------+\n");

	for (auto ivn = 0; ivn < 5; ivn++)
	{
		printf("|  %c ", vn_table[ivn][0]);
		for (auto ivt = 0; ivt < 6; ivt++)
		{
			printf("%c	", m[ivn][ivt][0]);

		}
		printf("|\n");
	}
}

int get_input()	//取得输入串
{
	// ReSharper disable once CppDeprecatedEntity
	strcpy(input," ");	//输入串字符数组
	printf("\n	请输入串($结尾):\n\n");

	auto temp = 0;
	char c = getchar();
	while (c!= '$')
	{
		if ((c != ' ') && (c != '$') && (c != '\n'))	//  忽略 空格 TAB 换行符
		{
			input[temp] = c;
			temp++;
		}
		c = getchar();
	} 

	input[temp] = '$';
	input[temp + 1] = 0;  	//输入串字符数组尾部加ASCII  0
	len = temp + 1;		//当前输入串数组长度


	/*
	 *if (len <= 1)
	{
		printf("	有效输入串为空!!!\n\n");
		return -1;
	}
	*/
	return 0;
}

void get_m()  //取得访问分析表第一二维坐标vn vt
{
	switch (char_top) //取得第一维坐标vn
	{	
		case 'E':vn = E;break;
		case 'A':vn = A;break;
		case 'T':vn = T;break;
		case 'B':vn = B;break;
		case 'F':vn = F;break;
		default: ;
	} 

	switch (char_input) //取得第二维坐标vt
	{
		case 'i':vt = id;break;
		case '+':vt = add;break;
		case '*':vt = multi;break;
		case '(':vt = left;break;
		case ')':vt = right;break;
		case '$':vt = end;break;
		default: ;
	} 
}

int ll1()
{
	auto temp = 0;

	get_input();
	int_input = 0;	//当前输入串数组指针值  初值为0	
	int_top= -1;	//当前栈顶指针值	
	char_input = input[int_input];	//当前输入串首字符: char_input(b)
	step = 0;	//当前步骤值  初值为0

	printf("输入串:%s 的语法分析\n",input);
	printf("步骤-----分析栈内容-----栈顶符号X-----当前输入b-----余留串-----m[X,b]\n");

	push('$');
	push('E');

	bool b = true; 

	while (b)
	{
		for (temp = 0; temp < int_top + 1; temp++)
			stack2[temp] = stack[temp];
		stack2[temp] = 0;  //末尾加\0的语法分析堆栈数组  用于打印

		pop();	//当前栈顶符号:char_top (X)出栈

		if ((char_top == 'i') || (char_top == '+') || (char_top == '*') || (char_top == '(') || (char_top == ')'))
		{
			if (char_top == char_input)
			{
				step++;
				Print();	//输出当前步骤的相关信息
				int_input++;
				char_input = input[int_input];	//当前剩余输入串首字符: char_input  (b)
			}
			else { printf("不匹配！"); }
		}
		else
		{
			if (char_top == '$')
				//if1-2
			{
				if (char_top == char_input)  //字符'$'匹配
									 //if1-2-1
				{
					b = false;

					step++;
					Print();	//输出当前步骤的相关信息
				}
				else	//if1-2-1
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	当前栈顶符号:$ \n\n");
					printf("   +------------------------------------------------------------------------+\n\n");
					return -1;

				}//if1-2-1
			}
			else	//if1-2
			{
				if ((char_top == 'E') || (char_top == 'A') || (char_top == 'T') || (char_top == 'B') || (char_top == 'F'))
					//if1-2-2
				{
					if ((char_input == 'i') || (char_input == '+') || (char_input == '*') || (char_input == '(') || (char_input == ')') || (char_input == '$'))
						//if1-2-2-1
					{
						get_m();	//取得访问分析表三维字符数组的第一二维坐标
					}
					else	//if1-2-2-1
					{
						printf("   +------------------------------------------------------------------------+\n\n");
						printf("	当前输入符号： %c  出错!!!\n\n", char_input);
						printf("   +------------------------------------------------------------------------+\n\n");
						return -1;

					}	//if1-2-2-1

					if ((strcmp(m[5][6] , "?") == 0) || (strcmp(m[5][6] , "ε") == 0))
						//if1-2-2-2
						
					{
						step++;
						Print();	//输出当前步骤的相关信息

						if (strcmp(m[5][6] , "?") == 0)
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
						temp = 0;
						while (m[5][6][4] != ' ')
							//对产生式右部串尾部加一个' '用以识别字符串的结束。				
						{
							temp++;
						}// while1

						while (temp > 0)
						{
							temp--;
							push(m[5][6][temp]);
						}// while2

						step++;
						Print();	//输出当前步骤的相关信息
					}	//if1-2-2-2
				}
				else	//if1-2-2
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	当前栈顶符号： %c  出错!!!\n\n", char_top);
					printf("   +------------------------------------------------------------------------+\n\n");
					return -1;

				}	//if1-2-2

			}//if1-2

		}  //if1

	} 
	return 0;
}

void Print()	//输出当前步骤信息
{
	char s_remain[64];   //余留字符数组
	auto remain = 0;
	auto temp = int_input;
	while (temp <= len)		//当前输入串数组长度
	{
		s_remain[remain] = input[temp];
		temp++;
		remain++;
	}
	//s_remain[temp] = 0;

	if ((char_top == 'i') || (char_top == '+') || (char_top == '*') || (char_top == '(') || (char_top == ')'))
		printf("%d-----%s-----%c-----%c-----%s-----%c 匹配\n", step, stack2, char_top, char_input, s_remain, char_input);
	else
	{
		if (char_top == '$')
			printf("%d-----%s-----%c-----%c-----%s 接受\n", step, stack2, char_top, char_input, s_remain);
		//else{printf("%d-----%s-----%c-----%c-----%s-----%c -> %c\n", step, stack2, char_top, char_input, s_remain, char_top, m[5][6]);}
		else { printf("error!"); }
	}
}
void Hint() //显示实验提示信息函数 
{
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n");
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
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n");

	printf("\n");

} 

int main()
{
	Hint();
	init();
	if(ll1() == -1)    //LL(1)语法分析
	{
		printf("	分析结果：对此输入串：    %s    的LL(1)语法分析出错\n\n", input);
		printf("   +------------------------------------------------------------------------+\n\n");
	}
		else
		{
			printf("	分析结果：对此输入串：    %s    的LL(1)语法分析成功\n\n", input);
			printf("   +------------------------------------------------------------------------+\n\n");
		}
	system("pause");
	return 0;
}


