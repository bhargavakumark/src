#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *input;
char input1[30];
char input2[30];

int calcfactors(int factnum)
{
	int current,count,i,num,sum,len,tempnum;
	sum = 0;
	len = strlen(input);
	for(i=0;i<len;i++)
	{
		if(input[i] >= 'a' && input[i] <= 'z')
			sum += input[i] - 'a' + 1;
		if(input[i] >= 'A' && input[i] <= 'Z')
                        sum += input[i] - 'A' + 1;
	}
	num = sum;
	while(num >= 10)
	{
		sum = 0;
		tempnum = num;
		while(tempnum!=0)
		{
			sum+= tempnum%10;
			tempnum /= 10;
		}
		num = sum;
	}
	return num;
}
		

int main()
{
	int num,temp,temp2,i,finish=0;
	char c;
	while(1)
	{
		i =0;
		while(1)
		{
			if(scanf("%c",&c)==EOF)
			{
				finish = 1;
				break;
			}
			if(c=='\n')
				break;
			input1[i] = c;
			i++;
		}
		input1[i] = '\0';
//		printf(input1);
		i =0;
                while(1)
                {
                        if(scanf("%c",&c)==EOF)
			{
				finish = 1;
                                break;
			}
                        if(c=='\n')
                                break;
                        input2[i] = c;
                        i++;
                }
		input2[i] = '\0';

		if(finish == 1)
			break;
//		printf(input2);
		input = input1;
		temp = calcfactors(num);
		input = input2;
		temp2 = calcfactors(num);
		if(temp == 0 && temp2 == 0)
		{
			printf("\n");
			continue;
		}
		if(temp == 0 || temp2 == 0)
			printf("0.00 %\n");
		if(temp > temp2)
			printf("%.2f %%\n",temp2*100.0/temp);
		else
			printf("%.2f %%\n",temp*100.0/temp2);
	}
		
	return 0;
}	


