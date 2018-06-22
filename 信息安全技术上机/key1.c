#include<stdio.h>
#include<string.h>

int main()//ÒÆÎ»ÃÜÂë
{
	char code[50];
	int length,i=0,key=0;
	printf("Please input primal sentence: ");
	gets(code);
	length = strlen(code);
	
	printf("Input the key(0~26):");
	scanf("%d", &key);
	getchar();
	for(i=0; i<length; i++)
	{
	if(code[i]>96 && code[i]<123)
		code[i] = (code[i]+key-97)%26+65;
	else if(code[i]>64&&code[i]<91)
		code[i] = (code[i]+key-65)%26+65;
	}
	printf("Result is: %s\n",code);
} 

