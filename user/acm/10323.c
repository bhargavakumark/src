#include <stdio.h>
#include <stdlib.h>

int digits[4];
int maxdigits = 0;

void fact(int n)
{
	int i,j,carry,temp;
	maxdigits = 1;
	digits[0] = 1;
	for(j=2;j<=n;j++)
	{
		temp = 0;
		for(i=0;;i++)
		{
			if(i >= maxdigits )
				digits[i] = 0;
			temp += j*digits[i];
		//	printf("temp %d\n",temp);
			digits[i] = temp % 10000;
			temp = temp / 10000;
			if(temp == 0 && i >= maxdigits-1)
				break;
		}	
		maxdigits = i+1;
		//printf("maxdigits : %d \n",maxdigits);
		if(maxdigits > 3)
		{
			printf("Overflow!\n");
			return;
		}
	}
	if(maxdigits == 3 && digits[2] > 62)
	{
		printf("Overflow!\n");
		return;
	}	
	if(maxdigits == 1)
	{
		printf("Underflow!\n");
		return;
	}
	printf("%d",digits[maxdigits-1]);
	for(i=maxdigits-2;i>=0;i--)
	{
		printf("%.4d",digits[i]);
	}	
	printf("\n");
}		

int main()
{
	int num;
	while(scanf("%d",&num)!=EOF)
	{
		fact(num);
	}
	return 0;
}	



