#include <stdio.h>
#include <stdlib.h>

int f91(int num)
{
	int temp;
	if(num <= 100)
	{
		temp = f91(num+11);
		return f91(temp);
	}
	else
		return num-10;
}

int main()
{
	int num;
	while(1)
	{
		scanf("%d",&num);
		if(num == 0)
			break;
		printf("f91(%d) = %d\n",num,f91(num));
	}
	return 0;
}

