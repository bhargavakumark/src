#include <stdio.h>
#include <stdlib.h>

int dist(int b,int a)
{
	int temp=0;
	temp += (a+b) * (a+b+1) / 2;
	temp += b;
//	printf("temp : %d\n",temp);
	return temp;
}


int main()
{
	int cases;
	int i,a,b,temp;
	scanf("%d",&cases);
	for(i=0;i<cases;i++)
	{
		scanf("%d%d",&a,&b);
		temp = dist(a,b);
		scanf("%d%d",&a,&b);
		printf("Case %d: %d\n",i+1,dist(a,b) - temp);
	}
	return 0;
}	


