#include<stdio.h>
#define t_num 100
int main()
{
int i, j,k;
int arr[t_num];
for (i=2; i<t_num; i++ )
{
	 arr[i]=1;
}

for (i=2; i<t_num; i++ )
{
	if ( arr[i] == 1 )
		for (j=i; j<=t_num/i; j++ ) 
		{
			arr[i*j]=0;
		}
}
	for(i=2; i<t_num; i++)
	{
		if ((arr[i]) == 1 )
			 printf("%4d \n", i); 
			
	}
}

