#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int isvowel(char c)
{
	if(c =='a' || c=='e' || c=='i' || c=='o' || c=='u' ||
	   c =='A' || c=='E' || c=='I' || c=='O' || c=='U')
		return 1;
	return 0;
}

int main()
{
	char output[10000],c,nonvowel;
	int outputindex =0,vowel=0;;
	while(1)
	{
		if(scanf("%c",&c)==EOF)
			break;
		if(c == '\n')
		{
			output[outputindex++] = '\0';
			printf("%s\n",output);
			outputindex = 0;
			continue;
		}
		if(!isalpha(c))
		{
			output[outputindex++] = c;
			continue;
		}
		else
		{
			vowel = 0;
			if(isvowel(c))
			{
				output[outputindex++]=c;
				vowel = 1;
			}
			else
				nonvowel = c;
			while(1)
			{
				scanf("%c",&c);
				if(isalpha(c))
					output[outputindex++] = c;
				else
					break;
			}
			if(!vowel)
				output[outputindex++] = nonvowel;
			output[outputindex++] = 'a';
			output[outputindex++] = 'y';
			output[outputindex++] = c;
			output[outputindex++] = '\0';
			printf(output);
			outputindex = 0;
		}
	}
	
	return 0;
}	


