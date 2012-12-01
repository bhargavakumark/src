#include <stdio.h>
#include <stdlib.h>

void calclast(int num)
{
	int i;
	long rem,temp;
	rem = 1;
	for(i=2;i<=num;i++)
	{
		temp = rem * i;
		while(temp%10 ==0)
			temp /= 10;
		rem = temp %100000;
	}
	printf("%5d -> %d\n",num,rem%10);
}
	


int main()
{
	int num;
	while(1)
	{
		if(scanf("%d",&num)==EOF)
			break;
		if(num == 0 || num == 1)
		{
			printf("%5d -> %d\n",1,1);
			continue;
		}
		calclast(num);	
	}
	return 0;
}	


