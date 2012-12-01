#include <stdio.h>
#include <stdlib.h>

int numbers[10005];
int noofele = 0;
	
void insert(int num)
{
	int j;
	j=noofele;
	while ((j > 0) && (numbers[j-1] > num))
    	{
      		numbers[j] = numbers[j-1];
      		j = j - 1;
    	}
	numbers[j] = num;
	noofele ++;
	if(noofele % 2 !=0)
		printf("%d\n",numbers[noofele/2]);
	else
		printf("%d\n",(numbers[noofele/2] + numbers[noofele/2 -1])/2);
}

int main()
{
	int num;
	while(scanf("%d",&num)!=EOF)
		insert(num);
	return 0;
}	


