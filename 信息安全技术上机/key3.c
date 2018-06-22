#include<stdio.h>
#include<string.h>

void main() //Î¬¼ªÀûÑÇÃÜÂë
{
	char c[100], key[100];
	int lenc, lenk, i=0, j=0, tmp;
	printf("input sentence: ");
	gets(c);
	lenc = strlen(c);
	strcpy(c, strupr(c));
	
	printf("Input the key: ");
	gets(key);
	lenk = strlen(key);
	strcpy(key, strupr(key));
	
	for(; i<lenc; i++)
	{
		j = j%lenk;
		if(c[i]>64&&c[i]<91)
		{
			c[i] = (c[i]-65+key[j]-65)%26+65;
			j++;
		}
	}
	printf("Result is: %s\n", c);
	for(i=0, j=0; i<lenc; i++)
	{
		j = j%lenk;
		if(c[i]>64&&c[i]<91)
		{
			tmp = c[i]-65-(key[j]-65);
			if(tmp>=0)
				c[i] = tmp%26+97;
			else
			c[i] = (tmp+26)%26+97;
			j++;
		}
	}

}
