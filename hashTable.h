#ifndef HASHTABLE_H
#define HASHTABLE_H


	#include "standard.h"
	#include "linkedList.h"


	
	struct node *createHashTable ( );
	struct node *searchHashTable ( struct node *, char * );

	int   hashFunction    ( char * );
	void  printHashTable  ( struct node * );	
	


#endif





