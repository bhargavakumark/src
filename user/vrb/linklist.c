#include<stdio.h>
#include<stdlib.h>

typedef struct node *link;
struct node { int item; link next; };
int main( int argc, char *argv[])
{

	int i, N = atoi(argv[1]), M = atoi(argv[2]);
	
	//reserve memory for a node and return a pointer to it in x;
	//link x=malloc(sizeof *x);
	
	link t = malloc(sizeof *t), x = t;

	//to differ between item and a link of node use following terminology
	// (*x).item and (*x).link
	//equivalent to  x->item x->link

	t->item = 1; t->next = t;

	//insert a node
	// t->next = x->next  ; x-next = t;
	
	for (i=2; i<= N; i++)
	{
		x = (x->next = malloc(sizeof *x));
		x->item = i; x->next = t;
	}

	// delete a linked list
	// t = x->next; x->next = t->next;
	// or x->next = x->next->next;


	//apply deletion with rules of Josephus problem
	while (x != x->next )	
	{
		for ( i=1; i< M; i++ ) x = x->next;
		x->next = x->next->next; 
	}
	printf("%d\n",x->item);
}


//NOTE:

//the most common bug =>      i) where we referenced an undefeined pointer
//			      ii)to use a pointer that wehave changed unknowingly
	
