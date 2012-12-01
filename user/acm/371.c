#include <stdio.h>
#include <stdlib.h>


int main()
{
	long start,end,i,temp,maxindex,max,count,tempstart,tempend;
	while(1)
	{
		if(scanf("%ld %ld",&start,&end)==EOF)
			break;
		//printf("start %d end %d",start,end);
		if(start == 0 && end == 0)
			break;
		max = 0;
		if(start > end)
		{
			tempstart = end;
			tempend = start;
		}
		else
		{
			tempstart = start;
			tempend = end;
		}
		for(i=tempstart;i<=tempend;i++) {
			count =0;
			temp = i;
			while(temp!=1)
			{
				if(temp%2 !=0)
				{
					temp = (3*temp + 1)>>1;
					count++;
				}
				else
					temp = temp>>1;
				count++;
			}
			if(count > max) {
				max = count;
				maxindex = i;
			}
		}
		printf("Between %ld and %ld, %ld generates the longest "
 			"sequence of %ld values.\n",start,end,maxindex,max);
		//break;
	}
	return 0;
}	


