#ifndef LINKEDLIST_H
#define LINKEDLIST_H



	#include "standard.h"



	/********************
	 * GLOBAL VARIABLES *
	 ********************/


	struct node {			/* Double Linked list's node struct */

        	char fileName[16];
		
		int i_Node;

        	struct node *last;	/*Points to the last node of list*/
        	struct node *next;	/*Points to the next node of list*/
	};




	/*********************************
	 * FUNCTION PROTOTYPE DEFINITION *
	 *********************************/


	struct node * initializeNode();

	struct node * searchList   ( struct node *, char * );
	struct node * deleteList   ( struct node *, char * );
        struct node * insertList   ( struct node *, char *, int );
	struct node * findTailList ( struct node * );

	void          printList    ( struct node * );




	


#endif
