#include <stdio.h>
#include <stdlib.h>

char digits[4000];
int maxdigits = 0;

void fact(int n)
{
	int i,j,carry,temp;
	int count[10];
	for(i=0;i<10;i++)
		count[i] = 0;
	printf("%d! --\n",n);
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
			digits[i] = temp % 10;
			temp = temp / 10;
			if(temp == 0 && i >= maxdigits-1)
				break;
		}	
		maxdigits = i+1;
		//printf("maxdigits : %d \n",maxdigits);
	}
	for(i=0;i<maxdigits;i++)
 		count[digits[i]]++;

	printf("   (0)%5d    (1)%5d    (2)%5d    (3)%5d    (4)%5d\n", 
			count[0],count[1],count[2],count[3],count[4]);
	printf("   (5)%5d    (6)%5d    (7)%5d    (8)%5d    (9)%5d\n", 
			count[5],count[6],count[7],count[8],count[9]);
}		

int main()
{
	int num;
	while(scanf("%d",&num)!=EOF)
	{
		if(num == 0)		
			break;
			
		fact(num);
	}
	return 0;
}	



