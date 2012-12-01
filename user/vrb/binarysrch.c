#include<stdio.h>

int main()
{
	int arr[]={1, 2, 3, 4, 5, 77, 88};
	int e=77;
	int len1=1;
	int total_number1=7;
	printf("after apply binary search %d \n", binsrch(arr,e,total_number1,len1));

}

int binsrch(int ar1[], int element, int total_number, int len)
{
	int i,m;
	while ( total_number >= len )
	{
		printf("inside the while loop: total_number and m is %d %d \n",total_number,m);
		m = (len+total_number)/2;	
		printf("the value of m should be half the previous one now %d  and value of element and ar1[m] is %d %d \n",m, element,ar1[m]);
		if ( element == ar1[m] ) 
		{
			printf("the value of index is %d \n",m);
			return m;
		}
		if ( element < ar1[m] )
		{
			total_number=m-1; 
			printf("less than case:  m is %d  and value of element and ar1[m] is %d %d \n",m, element,ar1[m]);
		}
		else 
		{
			len=m+1;
			printf("greater than case:  m and len is %d %d and value of element and ar1[m] is %d %d \n",m,len,element,ar1[m]);
		}	
	}
	return -1;
}
