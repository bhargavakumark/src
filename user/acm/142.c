#include <stdio.h>
#include <stdlib.h>

int regions[30][4];
int icons[60][2];
int regionindex=0,iconindex=0;
int mousecord[2];
int disableicons[60];

void readicon()
{
	scanf("%d%d",&icons[iconindex][0],&icons[iconindex][1]);
	iconindex ++;
}

void readregion()
{
	scanf("%d%d%d%d",&regions[regionindex][0],&regions[regionindex][1],
			 &regions[regionindex][2],&regions[regionindex][3]);
	regionindex++;
}

int inside(int i)
{
	if(mousecord[0] >= regions[i][0] && mousecord [0] <= regions[i][2])
		if(mousecord[1] >=regions[i][1] && mousecord[1]<=regions[i][3])
			return 1;
	return 0;
}

int dist(int i)
{
	int  temp =0;
	if(disableicons[i] == 1)
		return 260000;
	temp = (mousecord[0] - icons[i][0]) * (mousecord[0] - icons[i][0]);
	temp += (mousecord[1] - icons[i][1]) * (mousecord[1] - icons[i][1]);
	return temp;
}

void readmouse()
{
	int foundregion = -1,i,mindist;	
	scanf("%d%d",&mousecord[0],&mousecord[1]);	
	foundregion = -1;
	for(i=0;i<regionindex;i++)
	{
		if(inside(i))
			foundregion = i;
			
	}
	if(foundregion != -1)
		printf("%c\n",'A'+foundregion);
	else
	{
		/* search icons */
		mindist = 250000;
		for(i=0;i<iconindex;i++)
		{
			if(dist(i) < mindist)
				mindist = dist(i);
		}
		for(i=0;i<iconindex;i++)
                {
                        if(dist(i)== mindist)
				printf("%3d",i+1);
                }
		printf("\n");
	}
}
			 		
void clear()
{
	int i,j;

	for(i=0;i<iconindex;i++)
	{
		disableicons[i] = 0;
		mousecord[0] = icons[i][0];
		mousecord[1] = icons[i][1];
		for(j=0;j<regionindex;j++)
		{
			if(inside(j))
			{
				disableicons[i] = 1;
				break;
			}
		}
	}
}

int main()
{
	char temp[10];
	int first = 0;
	while(1)
	{
		scanf("%s",temp);
		if(temp[0] == 'I')
			readicon();
		if(temp[0] == 'R')
			readregion();
		if(temp[0] == 'M')
		{
			if(first == 0)
				clear();
			first = 1;
			readmouse();
		}
		if(temp[0] == '#')
			break;
	}
	return 0;
}	


