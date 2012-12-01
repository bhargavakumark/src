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
		case 'B':
		case 'C':
			printf("2");
			break;
		case 'D':
		case 'E':
		case 'F':
			printf("3");
			break;
		case 'G':
		case 'H':
		case 'I':
			printf("4");
			break;
		case 'J':
		case 'K':
		case 'L':
			printf("5");
			break;
		case 'M':
		case 'N':
                case 'O':
			printf("6");
			break;
                case 'P':
                case 'Q':
		case 'R':
		case 'S':
			printf("7");
			break;
		case 'T':
		case 'U':
		case 'V':
			printf("8");
                        break;
		case 'W':
                case 'X':
		case 'Y':
                case 'Z':
			printf("9");
			break;
		default:
			printf("%c",c);

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


