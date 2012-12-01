#include <stdio.h>
#include <stdlib.h>

int digits[4000];
int maxdigits = 0;

void fact(int n)
{
	int i,j,carry,temp,k;
//	printf("%d!\n",n);
	maxdigits = 1;
	digits[0] = 1;
	for(k=0;k<4;k++)
	{
		if(k==0 || k==1)
			j=n;
		else
			j=n+1;
		if(j%2 == 0)
			j=j/2;
		
		temp = 0;
		for(i=0;;i++)
		{
			if(i >= maxdigits )
				digits[i] = 0;
			temp += j*digits[i];
		//	printf("temp %d\n",temp);
			digits[i] = temp % 100;
			temp = temp / 100;
			if(temp == 0 && i >= maxdigits-1)
				break;
		}	
		maxdigits = i+1;
		//printf("maxdigits : %d \n",maxdigits);
	}
	printf("%d",digits[maxdigits-1]);
	for(i=maxdigits-2;i>=0;i--)
	{
		printf("%.2d",digits[i]);
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



