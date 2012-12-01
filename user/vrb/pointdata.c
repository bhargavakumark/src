
#include<stdio.h>
#include<math.h>
#include"Point.h"
typedef struct { float x; float y; } point ;
float distance( point a, point b);
float distance(point a, point b)
	{
	int x=1;
	int y=2;
	
	float dx=a.x - b.x, dy=a.y-b.y;
	return sqrt(dx*dx + dy*dy );
	}

int main()
{
	float a.x=4.0,b.x=5.0;
	printf("the value of sqrt is %d", distance(a.x,b.x));

}

