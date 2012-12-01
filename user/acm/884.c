#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n = 3000000;
int composite[3000000];
int cum[3000000];

void calcfactors(int factnum)
{
	int current,count,i,num,internalcount,tempnum;
	count = 0;
	
// 	printf("%3d! =",factnum);
	count = 0;
	for(tempnum = 2;tempnum <= factnum;tempnum++ )
	{
	num = tempnum;
	current = 2;
// 	if(num%10000 == 0)
// 		printf("num : %d\n",num);
	internalcount = 0;
	if(cum[num] != 0)
	{
		count += cum[num];
		continue;
	}
		
	while(num!=1)
	{
		if(num%current == 0)
		{
			num /= current;
			count++;
			internalcount++;
			if(cum[num] != 0)
			{	
				count += cum[num];
				internalcount += cum[num];
				break;
			}
			//cum[current] ++;
			//printf("cum[%d] = %d",current,cum[current]);
		}
		else
		{
	//		printf(" %d",count);
			//count =0;
			for(i=current+1 ; i<n;i++)
				if(composite[i] == 0)
				{
					current = i;
					break;
				}
		}
		
		//printf("current %d\n",current);
	}
	cum[tempnum] = internalcount;
	}
	
// 	count = 0;
// 	for(i=2;i<n;i++)
// 	{
// 		if(composite[i] == 1)
// 			continue;
// 		if(cum[i] == 0)
// 			break;
// 		count++;
// 		if(count == 16)
// 		{
// 			printf("\n      ");
// 			count=0;
// 		}
// 		printf("%3d",cum[i]);
// 	}
// 	printf("\n");
	printf("%d\n",count);
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
	for(j=2;j<n;j++)
	{
		if(composite[j] != 1)
			cum[j] = 1;
	}
}

int main()
{
	
	int num,i;
	for(i=0;i<n;i++)
		cum[i] = 0;
	seive();
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


