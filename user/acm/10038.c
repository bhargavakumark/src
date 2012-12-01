#include <stdio.h>
#include <stdlib.h>

char jolly[3050];

int main()
{
	int num,prev,i,current,diff;
	while(1)
	{
		if(scanf("%d",&num)==EOF)
			break;
		if(num == 0)
			break;
		scanf("%d",&prev);
		for(i=0;i<num-1;i++)
		{
			scanf("%d",&current);
			diff = abs(prev - current);
			jolly[diff] = 1;
			prev = current;
		}
		for(i=1;i<num;i++)
			if(jolly[i] == 0)
				break;
		if(i!=num)
			printf("Not jolly\n");
		else
			printf("Jolly\n");
		for(i=0;i<=num;i++)
			jolly[i] =0;
	}	
	
	return 0;
}	


