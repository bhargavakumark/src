#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int notint(char *t);

int charges[24];

struct time
{
	int mm;
	int dd;
	int hh;
	int mmm;
	long min;
	int km;
};

struct user
{
	char name[100];
	struct time intime;
	struct time outtime;
	struct time *times;
	struct user *next;
	int bill;
};

void readtime(struct time *t)
{
	char s[20];
	scanf("%s",s);

	t->mm = 0;
	t->mmm=0;
	t->dd = 0;
	t->hh = 0;

	t->mm = (s[0]-'0')*10 + (s[1]-'0');
	t->dd = (s[3]-'0')*10 + (s[4]-'0');
	t->hh = (s[6]-'0')*10 + (s[7]-'0');
	t->mmm =(s[9]-'0')*10 + (s[10]-'0');
	t->min = (t->dd)*24*60*60 + (t->hh)*60 + t->mmm;
}

int main()
{
 	int noofcases,i,j,new;
	char license[100];
	char enterexit[10];
	struct time *t;
	char c;
	
	scanf("%d",&noofcases);
	printf("Noof cases : %d\n",noofcases);

	scanf("%d",&charges[0]);
	for(i=0;i<noofcases;i++)
	{
		for(j=1;j<24;j++)
			scanf("%d",&charges[j]);
		scanf("\n");
	
		while(1)
		{
			scanf("%s",license);
/*			scanf("%c",&c);
			j=0;
			new = 0;
			while(1)
			{
				scanf("%c",&c);
				if( (j==0 && c=='\n') || c==EOF)
				{
					new = 1;
					break;
				}					
				if(c=='\n' || c==' ')
					break;
				license[j] = c;
			}
			if(new == 1)
				break;
			license[j+1] = '\0';
			printf("license :%s\n",license);	*/
	
			if(notint(license) ==0)
				break;
			if(strlen(license)<=1)
				break;
			t = (struct time *)malloc(sizeof(struct time));
			scanf("%d:%d:%d:%d",&t->mm,&t->dd,&t->hh,&t->mmm);
			scanf("%s",enterexit);
			scanf("%d",&t->km);
			printf("Exit : %s Dist %d\n",enterexit,t->km);
		}
		
	}
	return 0;
}

int notint(char *t)
{
	int i;
	for(i=0;i<strlen(t);i++)
		if(t[i] > '9' || t[i] < '0')
			return 1;
	return 0;
}
