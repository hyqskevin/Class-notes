// grammar.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#pragma warning(disable:4996)

#define vn_A 5	// ���ս����Ŀ 
#define vt_a 6	// �ս����Ŀ 

char input[64];	//�����СΪ64
char stack[64];	//ջ��С����Ϊ64
char stack2[64];
char m[5][6][4];	//m[vn_A][vt_a][num]

int step;  //����ֵ  ��ֵΪ0
int int_top;  //ջ��ָ��ֵ  ��ֵ-1
int int_input;  //���봮����ָ��ֵ  ��ֵΪ0
int len;  //���봮���鳤��
char char_top;   //ջ���ַ�����:char_top(X)
char char_input; 	//��ǰʣ�����봮���ַ�: char_input(b)

//���ս��  E'��A�棻T'��B�档
enum vn_type { E, A, T, B, F };
enum vn_type vn;  //��ǰ���ս��  ���ڷ��ʷ���������ĵ�һά����

//�ս��: i,+,*,(,),$
enum vt_type {id, add, multi, left, right, end};
enum vt_type vt;  //��ǰ�ս��  ���ڷ��ʷ���������ĵڶ�ά����

int push(const char in);
int pop();
void init();
int get_input();
void get_m();
int ll1();
void Print();

int push(const char in)  //ѹջ
{
	if (int_top >= 64)
	{
		printf("stack overflow!\n");
		return -1;
	}
	else
	{
		int_top++;  //��ǰջ��ָ��ֵ  ��ֵΪ-1
		stack[int_top] = in;
		return 0;
	}
}

int pop() //��ջ
{
	if (char_top < 0)
	{
		printf("stack overflow!\n");
		return -1;
	}
	else
	{
		char_top = stack[int_top];   //��ǰջ���ַ�����:char_top (X)
		int_top--;
		return 0;
	}
}

void init()
{
	char vn_table[5][1];	// ���ս������

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
	strcpy(&(m[1][4][0]), "��");  //M[E',)] : (3) E' -> ��
	strcpy(&(m[1][5][0]), "��");  //M[E',$] : (3) E' -> ��

	strcpy(&(m[2][0][0]), "FB ");  //M[T,i] : (4) T -> FT'
	strcpy(&(m[2][1][0]), "synch");  //M[T,+] : 
	strcpy(&(m[2][2][0]), "?");  //M[T,*] : 
	strcpy(&(m[2][3][0]), "FB ");  //M[T,(] : (4) T -> FT'
	strcpy(&(m[2][4][0]), "synch");  //M[T,)] : 
	strcpy(&(m[2][5][0]), "synch");  //M[T,$] : 

	strcpy(&(m[3][0][0]), "?");  //M[T',i] : 
	strcpy(&(m[3][1][0]), "��");  //M[T',+] : (6) T' -> ��
	strcpy(&(m[3][2][0]), "*FB ");  //M[T',*] : (5) T' -> *FT'
	strcpy(&(m[3][3][0]), "?");   //M[T',(] : 
	strcpy(&(m[3][4][0]), "��");  //M[T',)] : (6) T' -> ��
	strcpy(&(m[3][5][0]), "��");  //M[T',$] : (6) T' -> ��

	strcpy(&(m[4][0][0]), "i ");  //M[F,i] : (8) F -> i
	strcpy(&(m[4][1][0]), "synch");  //M[F,+] : 
	strcpy(&(m[4][2][0]), "synch");  //M[F,*] : 
	strcpy(&(m[4][3][0]), "(E) ");  //M[F,(] : (7) F -> (E)
	strcpy(&(m[4][4][0]), "synch");  //M[F,)] : 
	strcpy(&(m[4][5][0]), "synch");  //M[F,$] : 
	// ReSharper restore CppDeprecatedEntity

	printf("\n\n				�������ʽ�ķ�\n\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(1) E -> TE'	(2) E' -> +TE'	(3) E' -> ��	(4) T -> FT'\n");
	printf("	+-----------------------------------------------------------+\n");
	printf("	(5) T' -> *FT'	(6) T' -> ��	(7) F -> (E)	(8) F -> i\n");
	printf("	+-----------------------------------------------------------+\n\n");

	printf("\n\n			�������ʽ�ķ���LL(1)������\n\n");
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

int get_input()	//ȡ�����봮
{
	// ReSharper disable once CppDeprecatedEntity
	strcpy(input," ");	//���봮�ַ�����
	printf("\n	�����봮($��β):\n\n");

	auto temp = 0;
	char c = getchar();
	while (c!= '$')
	{
		if ((c != ' ') && (c != '$') && (c != '\n'))	//  ���� �ո� TAB ���з�
		{
			input[temp] = c;
			temp++;
		}
		c = getchar();
	} 

	input[temp] = '$';
	input[temp + 1] = 0;  	//���봮�ַ�����β����ASCII  0
	len = temp + 1;		//��ǰ���봮���鳤��


	/*
	 *if (len <= 1)
	{
		printf("	��Ч���봮Ϊ��!!!\n\n");
		return -1;
	}
	*/
	return 0;
}

void get_m()  //ȡ�÷��ʷ������һ��ά����vn vt
{
	switch (char_top) //ȡ�õ�һά����vn
	{	
		case 'E':vn = E;break;
		case 'A':vn = A;break;
		case 'T':vn = T;break;
		case 'B':vn = B;break;
		case 'F':vn = F;break;
		default: ;
	} 

	switch (char_input) //ȡ�õڶ�ά����vt
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
	int_input = 0;	//��ǰ���봮����ָ��ֵ  ��ֵΪ0	
	int_top= -1;	//��ǰջ��ָ��ֵ	
	char_input = input[int_input];	//��ǰ���봮���ַ�: char_input(b)
	step = 0;	//��ǰ����ֵ  ��ֵΪ0

	printf("���봮:%s ���﷨����\n",input);
	printf("����-----����ջ����-----ջ������X-----��ǰ����b-----������-----m[X,b]\n");

	push('$');
	push('E');

	bool b = true; 

	while (b)
	{
		for (temp = 0; temp < int_top + 1; temp++)
			stack2[temp] = stack[temp];
		stack2[temp] = 0;  //ĩβ��\0���﷨������ջ����  ���ڴ�ӡ

		pop();	//��ǰջ������:char_top (X)��ջ

		if ((char_top == 'i') || (char_top == '+') || (char_top == '*') || (char_top == '(') || (char_top == ')'))
		{
			if (char_top == char_input)
			{
				step++;
				Print();	//�����ǰ����������Ϣ
				int_input++;
				char_input = input[int_input];	//��ǰʣ�����봮���ַ�: char_input  (b)
			}
			else { printf("��ƥ�䣡"); }
		}
		else
		{
			if (char_top == '$')
				//if1-2
			{
				if (char_top == char_input)  //�ַ�'$'ƥ��
									 //if1-2-1
				{
					b = false;

					step++;
					Print();	//�����ǰ����������Ϣ
				}
				else	//if1-2-1
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	��ǰջ������:$ \n\n");
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
						get_m();	//ȡ�÷��ʷ�������ά�ַ�����ĵ�һ��ά����
					}
					else	//if1-2-2-1
					{
						printf("   +------------------------------------------------------------------------+\n\n");
						printf("	��ǰ������ţ� %c  ����!!!\n\n", char_input);
						printf("   +------------------------------------------------------------------------+\n\n");
						return -1;

					}	//if1-2-2-1

					if ((strcmp(m[5][6] , "?") == 0) || (strcmp(m[5][6] , "��") == 0))
						//if1-2-2-2
						
					{
						step++;
						Print();	//�����ǰ����������Ϣ

						if (strcmp(m[5][6] , "?") == 0)
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
						temp = 0;
						while (m[5][6][4] != ' ')
							//�Բ���ʽ�Ҳ���β����һ��' '����ʶ���ַ����Ľ�����				
						{
							temp++;
						}// while1

						while (temp > 0)
						{
							temp--;
							push(m[5][6][temp]);
						}// while2

						step++;
						Print();	//�����ǰ����������Ϣ
					}	//if1-2-2-2
				}
				else	//if1-2-2
				{
					printf("   +------------------------------------------------------------------------+\n\n");
					printf("	��ǰջ�����ţ� %c  ����!!!\n\n", char_top);
					printf("   +------------------------------------------------------------------------+\n\n");
					return -1;

				}	//if1-2-2

			}//if1-2

		}  //if1

	} 
	return 0;
}

void Print()	//�����ǰ������Ϣ
{
	char s_remain[64];   //�����ַ�����
	auto remain = 0;
	auto temp = int_input;
	while (temp <= len)		//��ǰ���봮���鳤��
	{
		s_remain[remain] = input[temp];
		temp++;
		remain++;
	}
	//s_remain[temp] = 0;

	if ((char_top == 'i') || (char_top == '+') || (char_top == '*') || (char_top == '(') || (char_top == ')'))
		printf("%d-----%s-----%c-----%c-----%s-----%c ƥ��\n", step, stack2, char_top, char_input, s_remain, char_input);
	else
	{
		if (char_top == '$')
			printf("%d-----%s-----%c-----%c-----%s ����\n", step, stack2, char_top, char_input, s_remain);
		//else{printf("%d-----%s-----%c-----%c-----%s-----%c -> %c\n", step, stack2, char_top, char_input, s_remain, char_top, m[5][6]);}
		else { printf("error!"); }
	}
}
void Hint() //��ʾʵ����ʾ��Ϣ���� 
{
	printf("���������������������������������������������������������������������������� \n");
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
	printf("���������������������������������������������������������������������������� \n");

	printf("\n");

} 

int main()
{
	Hint();
	init();
	if(ll1() == -1)    //LL(1)�﷨����
	{
		printf("	����������Դ����봮��    %s    ��LL(1)�﷨��������\n\n", input);
		printf("   +------------------------------------------------------------------------+\n\n");
	}
		else
		{
			printf("	����������Դ����봮��    %s    ��LL(1)�﷨�����ɹ�\n\n", input);
			printf("   +------------------------------------------------------------------------+\n\n");
		}
	system("pause");
	return 0;
}


