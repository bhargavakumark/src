#include <stdio.h>
#include <stdlib.h>

long numbers[6000];

struct element
{
	long long ele;
	struct element *next;
};
struct element *head = NULL;
struct element *temphead = NULL;

void insert(long long a)
{
	struct element *prev;
	struct element *newele;
	
	newele = (struct element *)malloc(sizeof(struct element));
	newele->ele = a;
	
	/* insert at begining */
	if(temphead->ele > a)
	{
		newele->next = temphead;
		temphead = newele;	
		printf("HELLO\n"); 
		return;
	}
	
	
	prev = temphead;
	temphead = temphead->next;
	for(; temphead != NULL ; temphead = temphead->next)
	{
		if(temphead->ele > a )
			break;
		if(temphead->ele == a)
			return;
		prev = temphead;
	}
	prev->next = newele;
	newele->next = temphead;
	temphead = newele;
	return;
}

void print()
{
	struct element *temp;
	
	for(temp=head;temp!=NULL;temp=temp->next)
		printf("%ld\t",(long)temp->ele);
	printf("\n");
}

void process()
{
	int i;
	long long value;
	for(i=0;i<5842;i++)
	{
		temphead = head;
		value = (head->ele)*2;
		//printf("%ld\t",(long)head->ele);
		numbers[i+1] = head->ele;
		if(value > (long long)2000000000)
			break;
		/* insert for 2 */
		insert(value);
		//print();
		value = (head->ele)*3;
		if(value <= (long long)2000000000)
			insert(value);
		//print();
		value = (head->ele)*5;
		if(value <= (long long)2000000000)
			insert(value);
		//print();
		value = (head->ele)*7;
		if(value <= (long long)2000000000)
			insert(value);
		//print();
		temphead = head;
		head = head->next;
		free(temphead);
	}
	if(i!=5842)
	{
		for(;i<5842;i++)
		{
// 			if(head == NULL)
// 			{
// 				printf("SOMETHING WRONG\n");
// 				break;
// 			}
				
			temphead = head;
			numbers[i+1] = head->ele;
			head = head->next;
			free(temphead);
		}
	}
}
	
int main()
{
	int num,temp;
	head = (struct element *)malloc(sizeof(struct element));
	head->ele = 1;
	head->next = NULL;
	temphead = head;
	process();
	//printf("SIZE OF %d \n",sizeof(long long));
	while(1)
	{
		scanf("%d",&num);
		if(num == 0)
			break;
		temp = num % 10;
		if(num%100 >= 11 && num%100 <= 19)
		{
			printf("The %dth humble number is %ld.\n",
							num,numbers[num]);
			continue;
		}
		if(temp == 1)
			printf("The %dst humble number is %ld.\n",
							num,numbers[num]);
		else if(temp == 2)
			printf("The %dnd humble number is %ld.\n",
							num,numbers[num]);
		     else if(temp == 3)
				printf("The %drd humble number is %ld.\n",
							num,numbers[num]);
		         else
				printf("The %dth humble number is %ld.\n",
							num,numbers[num]);
	}
	return 0;
}	 


