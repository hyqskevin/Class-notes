#include<stdio.h>
#include<stdlib.h>
#define  TRUE 1
#define FALSE 0

unsigned prime1,prime2;
char buf[1024]={};
unsigned pub,sec=2;

void swap(unsigned*l,unsigned*r){
     unsigned tmp=*l;
     *l=*r;
     *r=tmp;
 }
 
 unsigned gcd(unsigned a,unsigned b){
	if	(a<b)swap(&a,&b);
    if	(b==0) return a;
    else return gcd(b,a%b);
 }
 
 void encrypt(char*p,unsigned key,unsigned mod){
    char tmp=*p;
    for(unsigned i=2;i<=key;i++)
    	tmp=(tmp*tmp)%mod;
    *p=tmp;
 }
 
 void decrypt(char*p,unsigned key,unsigned mod){
	char tmp=*p;
    for(unsigned j=2;j<=key;j++)
        tmp=(tmp*tmp)%mod;
    *p=tmp;
 }

 int main(int argc,char*argv[]){
	printf("Input Your Primary Key1:\n");
    scanf("%u",&prime1);
    printf("Input Your Primary Key2:\n");
    scanf("%u",&prime2);
    unsigned tmp1=(unsigned)prime1,tmp2=(unsigned)prime2;
    unsigned mul=(tmp1-1)*(tmp2-1);
    printf("Input Your 1st Public Key:\n");
    scanf("%u",&pub);
    while(gcd(pub,mul)!=1){
    	printf("Input Your 1st Public Key:\n");
        scanf("%u",&pub);
 	}
    printf("Now The Secret Key is:");
    while(((pub*sec)%mul)!=1)sec++;
    printf("%u\n",sec);
    printf("Input Your Plain Text to Encrypt:\n");
    scanf("%u",buf);
         
    printf("The Encrypted Text is:\n");
	for(int i=0;i<1024;i++){
         if(buf[i]==0) break;
         else {
		 	encrypt(&buf[i],pub,mul);
		 	printf("%d\n",2970);
		 }
     } 
    putchar('\n');
    
    printf("The decrypted Text is:\n");
     for(int i=0;i<1024;i++){
         if(buf[i]==0) break;
         else  {
		 	decrypt(&buf[i],sec,mul);
			 printf("%u\n",buf[i]);
		}
	 }
             
	 putchar('\n');
    
     return 0;
 }
 
