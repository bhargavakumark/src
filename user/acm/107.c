#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double logs[100000];

void init()
{
	long i;
	for(i=2;i<100000;i++)
		logs[i] = log(i);
}

int main()
{
	long a,b,temp,c,d;
	long value;
	int n,z,i;
	init();
	
	while(1)
	{
		scanf("%ld%ld",&a,&b);
		if(a ==0 && b==0)
			break;
		if(a == 1 && b==1)
		{
			printf("0 1\n");
			continue;
		}
		if(b == 1)
			n = 1;
		else
		{
		value = log(a) / log(b) * (long)100000000;	
//		printf("value %d\n",value);
		for(i=2;i<99999;i++)
		{
			if((long)(logs[i+1] / logs[i]*(long)100000000) == value)
			{
				n = i;
		//		printf("n=%d\n",i);
				break;
			}
		}
		}
		z=1;
		temp = n+1;
		while(1)
		{
			if(temp == a)
			{
//				printf("z =%d\n",z);
				if(b == 1)
					c = z;
				else
				{	
					c = 1 - b;
					c /= (1-n);
				}
				d = a*(n+1) - b*n;
				printf("%ld %ld\n",c,d);
				break;
			}
			if(temp >  a)
			{
//				printf("bad \n");
				break;
			}
			temp *= (n+1);
			z++;
		}
	}
	return 0;
}	


