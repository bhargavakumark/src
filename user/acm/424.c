#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int digits[300];
char input[130];
int maxdigits = 25;

void add()
{
	char temp[10];
	int len,tempnum,i,num;
	len = strlen(input);
	tempnum = 0;
	for(i=22;;i--)
	{
		if(len > 5)
		{
			strcpy(temp,input+len-5);	
			num = atoi(temp);
//			printf("num :%d",num);
			input[len-5] = '\0';
			len = strlen(input);
		}
		else
		{
			strcpy(temp,input);
			num = atoi(temp);
//			printf("num :%d\n",num);
			input[0]= '\0';
			len = strlen(input);
		}
		if(i <= maxdigits )
			maxdigits = i;
		tempnum  += num + digits[i];
		digits[i] = tempnum % 100000;
		tempnum = tempnum / 100000;
		if(tempnum == 0&& len == 0)
			break;
//		printf("maxdigits : %d \n",maxdigits);
	}
//	printf("%d",digits[maxdigits-1]);
	return;
}

void print()
{
	int i;
	printf("%d",digits[maxdigits]);
	for(i=maxdigits+1;i<23;i++)
	{
		printf("%.5d",digits[i]);
	}	
	printf("\n");
}		

int main()
{
	int a,b;
	while(1)
	{
		scanf("%s",input);
		if(strlen(input) == 1 && input[0] == '0')
		{
			print();
			break;
		}
		add();
	}
	return 0;
}	



