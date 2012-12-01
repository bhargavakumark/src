#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[1050];

int main()
{
	int i, oddsum,evensum;
	while(1)
	{
		if(scanf("%s",input));
		oddsum = 0;
		for(i=0;i<strlen(input);i=i+2)
			oddsum += input[i] - '0';
		evensum = 0;
		for(i=1;i<strlen(input);i=i+2)
			evensum += input[i] - '0';
		if(oddsum == 0 && evensum ==0 )
			break;
		if((abs(evensum-oddsum) % 11 ) == 0)
			printf("%s is a multiple of 11.\n",input);
		else
			printf("%s is not a multiple of 11.\n",input);
		

	}
	return 0;
}	


