#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[1010];

int main()
{
	int num,sum,tempnum,i,j,degree,temp;

	while(scanf("%s",input)!=EOF)
	{
		degree = 0;
		temp = strlen(input);
		if(temp == 1 && input[0] == '0')
			break;
		sum = 0;
		for(i=0;i<temp;i++)
			sum += input[i] - '0';
		degree ++;
		num = sum;
		while(num >= 10)
		{
			tempnum = num;
			sum = 0;
			while(tempnum != 0)
			{
				sum += (tempnum %10);
				tempnum = tempnum /10;
			}
			num = sum;
			degree ++;
		}
		if(num == 9)
			printf("%s is a multiple of 9 and has 9-degree %d.\n",
								input,degree);
		else
			printf("%s is not a multiple of 9.\n",input);
	}
	return 0;
}	


