#include<stdio.h>
#include<string.h>
#include<malloc.h>

int main() //ÖÃ»»ÃÜÂë
{
	char c[50], *q;
	int *key, len, m, i, j=0;
	printf("input sentence:");
	gets(c);
	strcpy(c, strupr(c));
	len = strlen(c);
	for(i=0; i<len; i++)
	{
		if(c[i]<65||c[i]>90)
		{
			for(j=i; j<len-1; j++)
			c[j] = c[j+1];
			len--;
		}
	}
	c[len] = '\0';
	printf("Input the length of the key: ");
	scanf("%d", &m);
	key = (int)malloc(m*sizeof(int));
	q = (int)malloc(len*sizeof(int));
	printf("Input the key: ");
	for(i=0; i<m; i++)
	{
		scanf("%d", key+i);
		key[i]--;
	}
	getchar();
	for(i=0; i<len; i++)
	{
	j = (i/m)*m;
	q[i] = c[*(key+i%m)+j];
	}
	q[i] = '\0';
	printf("%s\n", q);
}
