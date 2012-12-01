#include <stdio.h>
#include <stdlib.h>


int main()
{
	int num,count,i,count2,mul,tempnum,hexnum,rem,cases;
	char output[40];
	
	scanf("%d",&cases);
	for(i=0;i<cases;i++)
	{
		scanf("%d",&num);	
		if(num == 0)
			break;
		count = 0;
		tempnum = num;
		while(tempnum!=0)
		{
			if(tempnum & 1)
				count ++;
			tempnum >>= 1;
		}
	
		tempnum = num;
		mul = 1;
		hexnum = 0;
		while(tempnum!=0)
		{
			rem = tempnum%10;
			hexnum += (rem * mul);
			mul *= 16;
			tempnum = tempnum / 10;
		}
		count2 = 0;
		tempnum = hexnum;
                while(tempnum!=0)
                {
                        if(tempnum & 1)
                                count2 ++;
                        tempnum >>= 1;
                }
		printf("%d %d\n",count,count2);
//		printf("%d\n",num^(count*count2));
	}
	return 0;
}	


