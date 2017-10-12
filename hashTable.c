#include "hashTable.h"


/*******************************************************
 *
 * hashFunction() takes as argument a valid filename
 * and return an integer, used as an index for 
 * hashTable. Our hashFunction try to store with 
 * alphabetical order filenames, to makes faster their
 * searching. We use one more linked list for valid 
 * filenames which uses other characters as first
 * letter.
 *
 ********************************************************/ 


int hashFunction( char *fileName ) {


int index = 0;
int temp  = 0;

char firstChar = '\0';


	firstChar = fileName[0];

	temp = (int) firstChar;	


		if( temp >= 65 && temp <= 90 ) {

			index = temp - 65;

		} else if( temp >= 97 && temp <= 122 ) {

			index = temp - 97;

		}else {
			index = 26;
		}

return index;
}


/*******************************************************************
 *
 * createHashTable() allocates memory for 27 starting nodes. Each  
 * node points to the head of a double Linked List, as the 
 * prototype of hash Table commands. 
 * Returns a pointer to the first node of Table.
 *
 ********************************************************************/

struct node *createHashTable() {

	int i;

	struct node *hashTable = calloc( 27, sizeof(struct node) );
	struct node *temp;

                        

        for( i=0; i<27; i++ ) {
         
                temp = &hashTable[i];
                temp -> next = insertList( NULL, "\0", -1 );

        }


return &hashTable[0];
}



void printHashTable( struct node *hashTable ) {

	int i;

	struct node *pointer = &hashTable[0];
	struct node *temp;


	for( i=0; i<27; i++ ) {

		temp = (struct node *)(&pointer[i]) -> next -> next;	/* Points on head of the linked list,                 */
                                                                	/* that hashFunction shows that this file-name exists */

		printf("\t%d. ", i+1);

		printList( temp );

		printf("\n");

	}

}



/**************************************************************************
 *
 * searchHashTable() takes two arguments, the first is a pointer to the
 * hash Table we use and the second is the file-name we use as a keyword
 * to search the Table. Returns a pointer to the list's node where 
 * file-name found, or else a NULL pointer. 
 * 
 **************************************************************************/

struct node *searchHashTable( struct node *hashTable, char *fileName ) {


	struct node *pt = NULL;
	int index = 0;
	
	index = (int)hashFunction( fileName );

	pt = (struct node *)(&hashTable[index]) -> next -> next;	/* Points on head of the linked list,                 */
									/* that hashFunction shows that this file-name exists */


		if( pt == NULL )					/* if hash Table hasn't any record on this    */
			return NULL;					/* list meens that there is no such file-name */
			

		while( pt != NULL ) {				

			if( memcmp( pt->fileName, fileName, strlen(fileName) ) == 0 )
				return pt;
			else
				pt = pt -> next;
		}
	

	return pt;
}
