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
	int num,cases,i,j,count;
	long sum;
	float avg;
	
	scanf("%d",&cases);
	for(i=0;i<cases;i++)
	{
		scanf("%d",&num);
		sum=0;
		for(j=0;j<num;j++)
		{
			scanf("%d",&numbers[j]);
			sum += numbers[j];
		}
		avg = sum / num;
//		printf("avg :%f\n",avg);
		count = 0;
		for(j=0;j<num;j++)
			if(numbers[j] > avg )
				count++;
		printf("%.3f\%\n",count * 100.0 / num);
	}
	return 0;
}	


