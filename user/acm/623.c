#include <stdio.h>
#include <stdlib.h>

int digits[4000];
int maxdigits = 0;

void fact(int n)
{
	int i,j,carry,temp;
	printf("%d!\n",n);
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
			digits[i] = temp % 1000000;
			temp = temp / 1000000;
			if(temp == 0 && i >= maxdigits-1)
				break;
		}	
		maxdigits = i+1;
		//printf("maxdigits : %d \n",maxdigits);
	}
	printf("%d",digits[maxdigits-1]);
	for(i=maxdigits-2;i>=0;i--)
	{
		printf("%.6d",digits[i]);
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



