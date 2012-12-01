#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n = 20000000;
char composite[20000000];
int primeindex[100010];

void calcfactors(int factnum)
{
	printf("(%d, %d)\n",primeindex[factnum],primeindex[factnum]+2);	
}
		
void build()
{
	int i,j;
	for(i=3,j=0;i<n;i=i+2)
	{
		if(composite[i] == 0 && composite[i+2] == 0)
		{	
			j++;
			primeindex[j] = i;
		}
		if(j > 100000) 
			break;
	}
}			

void seive()
{
	int i,j;
	for(i=0;i<n;i++)
		composite[i] = 0;
	
	for(i=2;i<5000;i++)
	{
		if(composite[i] == 1)
			continue;
		for(j=i*i;j<n;j=j+i)
			composite[j] = 1;
	}
}

int main()
{
	
	int num,i;
	seive();
	build();
	while(1)
	{
		if(scanf("%d",&num)==EOF)
			break;
		if(num == 0)
			break;
		calcfactors(num);
	}
	return 0;
}	


