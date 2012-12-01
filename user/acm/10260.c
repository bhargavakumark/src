#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int map[8];
int prev;

void check(char c)
{
	switch(c)
	{
		case 'A':
		case 'E':
		case 'I':
		case 'O':
		case 'U':
		case 'H':
		case 'W':
		case 'Y':
			prev = 0;
			break;
		case 'B':
		case 'F':
		case 'P':
		case 'V':
			if(prev != 1)
				printf("1");
			prev = 1;
			break;
		case 'C':
                case 'G':
                case 'J':
                case 'K':
		case 'Q':
		case 'S':
		case 'X':
		case 'Z':
                        if(prev != 2)
                                printf("2");
			prev = 2;
                        break;
		case 'D':
                case 'T':
                        if(prev != 3)
                                printf("3");
			prev = 3;
                        break;
		case 'L':
                        if(prev != 4)
                                printf("4");
			prev = 4;
//			map[4] = 1;
                        break;
		case 'M':
                case 'N':
                        if(prev != 5)
                                printf("5");
			prev = 5;
//			map [5] = 1;
                        break;
		case 'R':
                        if(prev != 6)
                                printf("6");
			prev = 6;
			map [6] = 1;
                        break;
	}
}
	
void clear()
{
	int i;
	for(i=0;i<8;i++)
		map[i]=0;
	prev = 0;
}

int main()
{
	char input[30];
	int len,i;
	
	while(scanf("%s",input)!=EOF)
	{
		len = strlen(input);
		for(i=0;i<len;i++)
			check(input[i]);
		clear();
		printf("\n");
	}	
	return 0;
}	


