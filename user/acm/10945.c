#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input [1000];

int lower(char c)
{
	if(c == ',' || c == '.' || c=='!' || c=='?' || c==' ')
		return 0;
	if(c>='A' && c <= 'Z')
		return c-'A'+1;
	return c-'a'+1;
}

int main()
{
	int start,end,bad,i;
	char c;
	
	while(1)
	{
	i = 0;
	while(scanf("%c",&c)!=EOF)
	{
		if(c == '\n')
			break;
		input [i] = c;
		i++;
	}
	input [i] = '\0';
	
	start = 0;
	end = strlen(input)-1;
	if(strlen(input) == 4 && strcmp(input,"DONE") == 0)
		break;

	bad = 0;
	while(start < end)	
	{
		/* processing for start */
		while(lower(input[start]) == 0)
		{
			if(start > end)
				break;
			start ++;
		}
		while(lower(input[end]) == 0)
		{
			if(start > end)
				break;
			end --;
		}
//		printf("start %d end %d\n",start,end);
//		printf("corresponding chars %c %c\n",input[start],input[end]);
		if(start >= end)
			break;
		if(lower (input[start]) != lower(input[end]))
		{
			printf("Uh oh..\n");
			bad = 1;
			break;
		}
		start ++;
		end --;
	}
	if(bad == 0)
		printf("You won't be eaten!\n");
	}/* end of main while */
	
	return 0;
}	


