#include <stdio.h>
#include <stdlib.h>

int freq[30];

int main()
{
	int num,max,i;
	char c;
	scanf("%d",&num);
	while(scanf("%c",&c)!=EOF)
	{
		if(c >= 'a' && c <= 'z')
			freq[c -'a'] ++;
		if(c >= 'A' && c <= 'Z')
                        freq[c -'A'] ++;		
	}

	while(1)
	{
		max = -1;
		for(i=0;i<26;i++)
			if(freq[i] > max)
				max = freq[i];
		if(max == -1 || max == 0)
			break;
		for(i=0;i<26;i++)
			if(freq[i] == max)
			{
				printf("%c %d\n",'A'+i,max);
				freq[i] = -1;
			}
	}
				
	return 0;
}	


