#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[100000];
int sum;

int getvalue(char c)
{
	int value=0;
	if(c >= '0' && c <= '9')
		value = c - '0';
	if(c >= 'A' && c <= 'Z')
		value = c - 'A' + 10;
	if(c >= 'a' && c <= 'z')
		value = c - 'a' + 36;
//	printf("value %d\n",value);
	
	return value;
}


int getmin()
{
	int value,min,i	;
	min = 0;
	sum = 0;
	for(i=0;i<strlen(input);i++)
	{
		value = getvalue(input[i]);
		if(value > min)
			min = value;
		sum += value;
	}	
	return min;
}

int trytest(int base)
{
	int num,i,value;
	num = 0;
	for(i=strlen(input)-1 ; i>= 0;i--)
	{
		value = getvalue(input[i]);
		num += value;
	}
	if(num % (base-1)==0)
		return 1;
	else
		return 0;	
}

int main()
{
	
	int min,success,i;

	while(scanf("%s",input)!=EOF)
	{
		min = getmin();
	
		//printf("min %d\n",min);
		if(sum == 0)
		{
			printf("2\n");
			continue;
		}
		for(i=min+1;i<=62;i++)	
		{
			if(sum % (i-1) == 0)
				break;
		}
		if(i!=63)
			printf("%d\n",i);
		else
			printf("such number is impossible!\n");
	}
	return 0;
}	


