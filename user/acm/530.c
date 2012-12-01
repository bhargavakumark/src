#include <stdio.h>
#include <stdlib.h>

int nummax = 0;
int denmax = 0;
int *num;
int *den;

void display();
int gcd(int a, int b);

void clearstuff()
{
	int i,j,temp;
	long numerator,denominator;
	for(i=denmax-1;i>=0;i--)
	{
		while(den[i]!=1)
		{
			for(j=0;j<nummax;j++)
			{
				if(num[j]==1)
					continue;
				if(num[j]%den[i] == 0)
				{
					num[j] /= den[i];
					den[i] = 1;
					break;
				}
				else 	if(den[i]%num[j] == 0)
					{		
						den[i]/= num[j];
						num[j] = 1;
					}
					else
					{
						temp=gcd(den[i],num[j]);
						if(temp!=1)
						{
							den[i] /= temp;
							num[j] /= temp;
						}
					}
		
				if(den[i] == 1)
					break;
			}
		

		}
	//	display();
	}

	numerator = 1;
	for(i=0;i<nummax;i++)
		numerator*=num[i];
	denominator = 1;
	for(i=0;i<denmax;i++)
		denominator*=den[i];		
//	printf("numerator %d denominator %d result %d\n",numerator,
//			denominator,numerator/denominator);
	printf("%ld\n",numerator/denominator);
}

int gcd(int a, int b)
{
  while (a != 0){
    if (b == 0) return (a);
    if (a > b)
      a = a % b;
    else b = b % a;
  };
  return (b);
}



						
void calcncr(int n, int r)
{
	int i;
//	write(1,"step 1",7);
	if(r==1 || r==n-1)
	{
		printf("%d\n",n);
		return;
	}
	if(r==0 || r==n)
	{
		printf("%d\n",1);
		return;
	}
	if(r > n/2)
	{
		/* pouplate numerator from r+1  to n*/
		//write(1,"step 1a",8);
		num = (int *)malloc((n-r)*sizeof(int));
		nummax = n-r;
		for(i=0;i<n-r;i++)
			num[i] = r+1+i;	
		den = (int *) malloc(sizeof(int)*(n-r));
		denmax = n-r;
		for(i=0;i<n-r;i++)
			den[i] = i+1;
		//write(1,"step 1z",8);
	}
	else
	{
		/* pouplate numerator from n-r+1 to n*/
//		write(1,"step 2a",8);
		num = (int *)malloc(sizeof(int)*r);
		nummax = r;
                for(i=0;i<r;i++)
                        num[i] = n-r+1+i;
		den = (int *)malloc(sizeof(int)*r);
		denmax = r;
		for(i=0;i<r;i++)
			den[i]=i+1;
//		write(1,"step 2z",8);
	}
//	write(1,"step x",7);
	//display();
//	write(1,"step y",7);
	clearstuff();
	free(num);
	free(den);
//	write(1,"step z",7);
}

void display()
{
	int i;

	//printf("numerator : ");
	for(i=0;i<nummax;i++)
		printf("%d ",num[i]);
	printf("\n");
	//printf("denominator : ");
	for(i=0;i<denmax;i++)
		printf("%d ",den[i]);
	printf("\n");
}

int main()
{
	int n,r;
	while(1)
	{
		scanf("%d%d",&n,&r);
		if(n==0 && r==0)
			break;
		calcncr(n,r);
	}
	return 0;
}	


