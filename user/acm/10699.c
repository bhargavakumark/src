#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n = 1000000;
int composite[1000000];

void calcfactors(int factnum)
{
	int current,count,i,num,prev;
	count = 0;
//	printf("%3d! =",factnum);
	num = factnum;
	current = 2;
	count = 0;
	prev = 0;
	while(num!=1)
	{
		if(num%current == 0)
		{
			num /= current;
			if(current != prev )
				count++;
			prev = current;
		}
		else
		{
			for(i=current+1 ; i<n;i++)
				if(composite[i] == 0)
				{
					current = i;
					break;
				}
		}
	}
	printf("%d : %d\n",factnum,count);
}
		

void seive()
{
	int i,j;
	for(i=0;i<n;i++)
		composite[i] = 0;
	
	for(i=2;i<10000;i++)
	{
		for(j=i*i;j<n;j=j+i)
			composite[j] = 1;
	}
}

int main()
{
	int num;
	seive();
	while(1)
	{
		scanf("%d",&num);
		if(num == 0)
			break;
		calcfactors(num);
	}
	return 0;
}	


