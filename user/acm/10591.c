#include <stdio.h>
#include <stdlib.h>

int bad[1000];

int calc(int num)
{
	int n,i,temp;
	n = num;
	for(i=0;i<1000;i++)
		bad[i] = 0;
	while(n!= 1)
	{
		if(n < 1000)
                {
                        if(bad[n] == 1)
                                return 0;
                        bad[n] = 1;
                }

		temp = 0;
		while(n!=0)
		{
			temp += (n%10) *(n%10);
			n = n/10;
		}
		n = temp;
	}
	return 1;
}

int main()
{
	int cases,i,num;

	scanf("%d",&cases);
	for(i=0;i<cases;i++)
	{
		scanf("%d",&num);
		if(calc(num))
			printf("Case #%d: %d is a Happy number.\n",i+1,num);
		else
			printf("Case #%d: %d is an Unhappy number.\n",i+1,num); 
	}		
	return 0;
}	


