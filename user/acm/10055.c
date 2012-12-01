#include <stdio.h>
#include <stdlib.h>


int main()
{
	long long a,b;
	while(scanf("%Ld%Ld",&a,&b)!=EOF)
		if(a>b)
			printf("%Ld\n",a-b);
		else	
			printf("%Ld\n",b-a);
	return 0;
}	


