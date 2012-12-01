#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input1[1006];
char input2[1006];

int freq1[27];
int freq2[27];

void count()
{
	int temp,i;
	temp = strlen(input1);
	for(i=0;i<temp;i++)
		freq1[input1[i] - 'a'] += 1;
	temp = strlen(input2);
        for(i=0;i<temp;i++)
                freq2[input2[i] - 'a'] += 1;
	for(i=0;i<26;i++)
	{
		for(;freq1[i] > 0 && freq2[i] > 0; freq1[i]--,freq2[i]--)
			printf("%c",i+'a');
		freq1[i]= 0;
		freq2[i] =0;
	}
	printf("\n");
}

int main()
{
	int i,end=0;
	char c;
	while(1)
	{
		i=0;
		while(1)
		{
			if(scanf("%c",&c)==EOF)
			{
				end = 1;
				break;
			}
			if(c=='\n')
				break;
			input1[i] = c;
			i++;
		}
		input1[i] ='\0';
		i=0;
                while(1)
                {
                        if(scanf("%c",&c)==EOF)
			{
				end =1;
                                break;
			}
			if(c=='\n')
				break;
                        input2[i] = c;
                        i++;
                }
                input2[i] ='\0';
	
		if(end == 1)
			break;
		count();
	}
	return 0;
}	


