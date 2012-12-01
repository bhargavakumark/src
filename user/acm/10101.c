#include<stdio.h>
#include <stdlib.h>
#include <string.h>

void printfront(char *input, int len);
void printlast(char *input, int len);
int count;
int prevnum =0 ;

int main()
{
	char input[100];
	int i=0, len,index;
	
	while(scanf("%s",input)!= EOF)
	{
 	//printf("%s",input);
		len = strlen(input);
		i++;
/*	rem = len % 9;
	if(rem != 0)
		printfront(input,rem);
*/
		index = 0;
		count = len / 9;
/*	for(i=0;i<count;i++)
	{*/
		printf("%4d.",i);
		if(count != 0)
		{
			printfront(input+index,len - 9);	
			index += len-9;
		}
		printlast(input+index,9);
		printf("\n");
	}
	return 0;
}

void printfront(char *input, int len)
{
	char output[10];
	int num,lakh,hajar,shata,kuti;
	strncpy(output,input,len);
	//printf("output : %s\n",output);
	num = atoi(output);
	prevnum = num;
	kuti = num / 100000;
	num = num % 100000;
	lakh = num / 1000;
	num = num % 1000;
	hajar = num/ 10;
	num = num % 10;
	shata = num;
	if(kuti != 0)
		printf(" %d kuti",kuti);
	if(lakh !=0)
		printf(" %d lakh",lakh);
	if(hajar!=0)
 		printf(" %d hajar",hajar);
	if(shata != 0)
		printf(" %d shata",shata);
}

void printlast(char *input, int len)
{
	char output[10];
	int num,lakh,hajar,shata,kuti;
	strncpy(output,input,len);
	//printf("output : %s\n",output);
	num = atoi(output);
	
	if(prevnum == 0 && num == 0)
	{
		printf(" 0");
		return;
	}
	if(num == 0 && count !=0)
	{
		printf(" kuti");
		return;
	}
	if(num == 0 && count ==0)
	{
		printf(" 0");
		return;
	}

	kuti = num / 10000000;
	num = num%10000000;
	lakh = num / 100000;
	num = num % 100000;
	hajar = num/ 1000;
	num = num % 1000;
	shata = num/100;
	num = num % 100;
	if(kuti != 0)
		printf(" %d kuti",kuti);
	else if(count != 0)
		printf(" kuti");
	if(lakh !=0)
		printf(" %d lakh",lakh);
	if(hajar!=0)
 		printf(" %d hajar",hajar);
	if(shata != 0)
		printf(" %d shata",shata);
	if(num != 0)
		printf(" %d",num);
	
}
