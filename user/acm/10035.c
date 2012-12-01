#include <stdio.h>
#include <stdlib.h>


int main()
{
	int num1,num2,a,b,carry,leftover;	
	while(1)
	{
		scanf("%d%d",&num1,&num2);
		if(num1 ==0 && num2 == 0)
			break;
		leftover = 0;
		carry = 0;
		while(num1 !=0 || num2 != 0)
		{
			a = num1 % 10;
			b = num2 % 10;
			if(a+b+leftover >= 10)
			{
				carry++;
				leftover = 1;
			}
			else
				leftover = 0;
			num1 /= 10;
			num2 /= 10;		
			//printf("num1 %d num2 %d a %d b %d\n",num1,num2,a,b);
		}
		if(carry == 0)
			printf("No carry operation.\n");
		if(carry == 1)
			printf("1 carry operation.\n");
		if(carry!=0 && carry !=1)
			printf("%d carry operations.\n",carry);
	}	
	return 0;
}	


