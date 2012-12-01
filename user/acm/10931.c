#include <stdio.h>
#include <stdlib.h>


int main()
{
	int num,count,i;
	char output[40];
	while(1)
	{
		scanf("%d",&num);	
		if(num == 0)
			break;
		count = 0;
		i = 0;
		while(num!=0)
		{
			if(num & 1)
			{
				count ++;
				output[i] = '1';
			}
			else
				output[i] = '0';
			num >>= 1;
			i++;
		}
		printf("The parity of ");
		i--;
		for(;i>=0;i--)
			printf("%c",output[i]);
		printf(" is");
		printf(" %d (mod 2).\n",count);
		
	}
	return 0;
}	


