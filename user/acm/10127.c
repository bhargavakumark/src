#include <stdio.h>
#include <stdlib.h>

void process(int num)
{
	int n,rem,count;
	n = 1;
	rem = 1;
	count = 1;
	while(rem != 0)
	{
		rem = rem * 10 +1;
		count ++;
		rem = rem % num;
	}
	printf("%d\n",count);
}
	

int main()
{
	int num;
	while(scanf("%d",&num)!=EOF)
	{
		if(num == 0 || num == 1)
			printf("1\n");
		else
			process(num);
	}
	return 0;
}	


