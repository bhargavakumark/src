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
		printf("the value of inside the while loop total_number is %d %d \n",total_number,m);
		m = (len+total_number)/2;	
		printf("the value of m should be half the previous one now %d  and value of element and ar1[m] is %d %d \n",m, element,ar1[m]);
		if ( element == ar1[m] ) printf("the value of index is %d \n",m); return m;
		
		if ( element < ar1[m] )	total_number=m-1; 	else len=m+1; 	printf("the value inside greater than case %d  and value of element and ar1[m] is %d %d \n",m, element,ar1[m]);
		
	}
	return -1;
}
