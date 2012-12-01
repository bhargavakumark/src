#include <stdio.h>
#include <stdlib.h>


int main()
{
	int cases,count;
	long long num,reverse,temp;
	scanf("%d",&cases);
	for(;cases>0;cases--)
	{
		scanf("%Ld",&num);
		count = 0;
		while(1)
		{
			reverse = 0;
			temp = num;
			while(temp!=0)
			{
				reverse = (reverse*10) + (temp%10);
				temp /= 10;	
			}
			if(reverse == num)
				break;
			num += reverse;
			count ++;
		}
		printf("%d %Ld\n",count,num);
						
	}
	return 0;
}	


