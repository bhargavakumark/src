#include<stdio.h>
int main()
{
//	int i,j;
	//strlen(char *p);
	char p[]={"the value of animals"};
	char *j;
//	printf("the value of length is %d", strleng(p));
	printf("the value after copy  %d \n", strcpa(p,j));
		

}

int strleng(char *s)
{

	int i;
	char *n = s;
	while ( *n != '\0' )
		n++;
	printf("the value of n and s is %d %d\n",n,s);
	return n-s;
}

//copt s to t
int strcpa(char *s, char *t)
{

	while ( *t++ == *s++);
		printf("the value of t is %d %d \n",*t, t);
		;	
}

