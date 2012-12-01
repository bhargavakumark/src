#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point
{
	float x1;
	float x2;
	float y1;
	float y2;
};

struct point points[1000];

int check(float x,float y, int i)
{
	if( x > points[i].x1 && x < points[i].x2 &&
	    y > points[i].y2 && y < points[i].y1 )
		return 1;
	return 0;
}

int main()
{
	char input[10];
	int i=0,count,j,hits;
	float x,y;

	while(1)
	{
		scanf("%s",input);
		if(strcmp(input,"r")==0)
			scanf("%f%f%f%f",&points[i].x1,&points[i].y1,
					 &points[i].x2,&points[i].y2);
		else
			break;
		i++;
	}
	count = i;

	j=0;
	while(1)
	{
		if(scanf("%f%f",&x,&y)==EOF)
			break;	
		if((int)(x*10) == 99999 && (int)(y*10) == 99999)
			break;
		//printf("%f %f\n",x,y);
		hits = 0;
		j++;
		for(i=0;i<count;i++)
			if(check(x,y,i)==1)
			{
				printf("Point %d is contained in figure %d\n",
								j,i+1);
				hits++;
			}
		if(hits == 0)
			printf("Point %d is not contained in any figure\n",
									j);
	}
	return 0;
}	


