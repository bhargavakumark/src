#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void readfloat(int *a);

int main()
{
	int students;
	long inttotal,share,temp,leftover,diff;
	int i;
	int amount[1000],total;

	while(1)
	{
		scanf("%d",&students);
		//printf("Students : %d",students);
		if(students == 0)
			return 0;
		
		total = 0;
		for(i=0;i<students;i++)
		{
			readfloat(&amount[i]);
			total += amount[i];
		}
		
		total = total ;
		inttotal = (int)(total);
		share = total / students;
	//	printf("Share %ld",share);
		
	//	printf("int total %ld\n",inttotal);
		leftover = inttotal % students;
	
		diff = 0;
		for(i=0;i<students;i++)
		{
			temp = (int)((amount[i])) ;
			//printf("temp %d\n",temp);
			if (temp > share)
			{
//				if(leftover > 0)
//				{
					leftover --;
//					//diff += (temp - share - 1);
//				}
				//else
//					diff += (temp - share);
			}
			else
				diff += (share - temp);
		}
		
		if(leftover > 0)
	 		diff += leftover;
		printf("$%.2f\n",diff*1.0/100);
	}	
	return 0;
}

void readfloat(int *a)
{
	char s[10];
	int i,num = 0;
	
	scanf("%s",s);
	//printf("read string %s\n",s);
	
	for(i=0;i<strlen(s);i++)
	{
		if(s[i] == '.')
			continue;
		num = num*10;
		num += s[i] - '0';
	}
	//printf("num %d\n",num);
	*a = num;
}
	
