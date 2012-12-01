#include <stdio.h>
#include <stdlib.h>

int bin[3][3],selected[3];
long long conf[7];
int main()
{
	int i,j,maxindex,max,renew;
	long long sum = 0,min=0,index;
	
	while(1)
	{
		sum =0;
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
			{
				if(scanf("%d",&bin[i][j])==EOF)
					return 0;
				sum += bin[i][j];
			}
	
	/* conf 1 BCG */
	conf[1]=bin[0][1]+bin[0][2]+bin[1][0]+bin[1][1]+bin[2][0]+bin[2][2];
	conf[2]=bin[0][1]+bin[0][2]+bin[1][0]+bin[1][2]+bin[2][0]+bin[2][1];
	conf[4]=bin[0][0]+bin[0][1]+bin[1][0]+bin[1][2]+bin[2][1]+bin[2][2];
	conf[3]=bin[0][0]+bin[0][1]+bin[1][1]+bin[1][2]+bin[2][0]+bin[2][2];
	conf[5]=bin[0][0]+bin[0][2]+bin[1][1]+bin[1][2]+bin[2][0]+bin[2][1];
	conf[6]=bin[0][0]+bin[0][2]+bin[1][0]+bin[1][1]+bin[2][2]+bin[2][1];

	min = sum;
	for(i=1;i<=6;i++)
		if(conf[i] < min)
		{
			min = conf[i];
			index = i;
		}
//	printf("index %d\n",index);
	switch(index)
	{
		case 1 :
			printf("BCG %Ld\n",min);
			break;
		case 2 :
                        printf("BGC %Ld\n",min);
                        break;
		case 3 :
                        printf("CBG %Ld\n",min);
                        break;
		case 4 :
                        printf("CGB %Ld\n",min);
                        break;
		case 5 :
                        printf("GBC %Ld\n",min);
                        break;
		case 6 :
                        printf("GCB %Ld\n",min);
                        break;
	}
	}
}

