#include <stdio.h>
#include <stdlib.h>

int digits[4000];
int maxdigits = 0;

void power(int a, int b)
{
	int i,j,carry,temp,k;
//	printf("%d!\n",n);
	maxdigits = 1;
	digits[0] = 1;
	j = a;
	for(k=0;k<b;k++)
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
	int a,b;
	while(1)
	{
		scanf("%d%d",&a,&b);
		if(a==0 && b==0)
			break;
		power(a,b);
	}
	return 0;
}	



