#include "linkedList.h"



/************************************************************
 *
 * initializeNode() allocates memory for a new node of
 * Linked List and initializes his contents to NULL
 *
 ************************************************************/
	
struct node *initializeNode(){

	struct node *new = (struct node *)malloc( sizeof(struct node) );


	memset( new->fileName, '\0', 16 );

	new -> i_Node = 0;

	new -> next = NULL;
	new -> last = NULL;

return new;
}


/*******************************************************************
 *
 * insertList() insert a new node with fileName member the second
 * given argument and return a pointer to the head of Linked List
 *
 *******************************************************************/

struct node *insertList( struct node *headOfList, char * fileName, int i_Node ) {
	
	struct node * new  = initializeNode();
	struct node * tail = NULL;

	new -> next = NULL;			/* modifies the new node of Linked List */
	memcpy( new->fileName, fileName, 16 );

	new -> i_Node = i_Node;

	tail = findTailList( headOfList );	/* find and return a pointer to the Tail of Linked List */


	if( tail == NULL ) {			/* if new is the head of Linked List */
	
		new -> last = NULL;
		return new;
	}
	else {
		tail->next = new;
		new ->last = tail;
	}

return headOfList;
}


/***************************************************************
 *
 * findTailList() takes as argument a pointer to the head of
 * Linked List and returns an pointer to the last node (tail)
 * of List.
 *
 ***************************************************************/ 

struct node *findTailList( struct node *headOfList ) {

struct node *tail = headOfList;

	if( tail == NULL )
		return tail;

	while( tail->next != NULL )
		tail = tail->next;

return tail;
}


void printList( struct node *headOfList ) {

	struct node *temp = headOfList;


	while ( temp != NULL ) {

			printf("%s", temp->fileName );
			printf("-> ");

			temp = temp->next;
	}

printf("NULL");
}


/*********************************************************************
 *
 * searchList() takes as argument a pointer to the head of the List
 * and a search-key and linearly searches the list for it and
 * returns a pointer to the node where key found or NULL if key
 * doesn't found. 
 *
 **********************************************************************/ 

struct node *searchList( struct node *headOfList, char *fileName ) {

	struct node *temp = headOfList;

	while( temp != NULL ) {

		if ( strcmp( temp->fileName ,fileName ) != 0 )
			temp = temp-> next;
		else
			return temp;
	}

return NULL;
}


/************************************************************************
 * 
 * deleteList() takes as argument a pointer to the head of Linked List
 * and search-key to delete it and delete node whitch key matches with
 * search-key given and then repair the List. Finally returns a pointer 
 * to the head of modified List.
 *
 ************************************************************************/

struct node * deleteList( struct node *headOfList, char *fileName ) {

	struct node *temp1 = headOfList;
	struct node *temp2 = headOfList;

	temp1 = searchList( temp1, fileName );

	if( temp1 == NULL ) {

		printf("\nFile %s does not exist\n", fileName);
	}
	else if( temp1 ->last == NULL ) {

		headOfList = temp1->next;
		headOfList -> last = NULL;

		free( temp1 );
	}
	else if( temp1 -> next == NULL ) {

		temp2         = temp1 -> last;
		temp2 -> next = NULL;

		free(temp1); 
	}
	else {		

		temp2 = temp1->last;
		temp1 = temp1->next;

		free(temp1->last);

		temp2->next = temp1;
		temp1->last = temp2;
	}

return headOfList;
}
	
	



