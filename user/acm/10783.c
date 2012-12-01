#include <stdio.h>

int  main()
{
	int i,n,a,b,num;
	scanf("%d",&num);
	for(i=0;i<num;i++)	
	{
		scanf("%d%d",&a,&b);
		if(a%2 !=1)
			a++;
		if(b%2 !=1)
			b--;
		n = (b-a)/2;
		printf("Case %d: %d\n",i+1,(a+n)*(n+1));
	}
	return 0;
}
