#include "standard.h"
#include "help.h"
#include "init.h"
#include "hashTable.h"



/************************************************************
 *
 * linearSearch() has an arguments, a pointer to the 
 * head of a Bit Map char array.
 *
 * Returns the position of the first empty block/i-Node,
 * or -1 if there is no space available.
 * 
 ************************************************************/

int linearSearch( char *BitMap ) {

int i;
	
	for( i=0; i<strlen(BitMap); i++ ) {

		if( BitMap[i] == '0' )
			return i;

	}


return -1;
}



/**************************************************************
 *
 * walkHashTable() takes as argument the number of system's
 * files and allocates memory to store their names into a 
 * memory segment and returns it.
 *
 **************************************************************/

char **walkHashTable( int num ) {

int i;
int numCur = 0;

	struct node *hashTable = globalINFO->hashTable;
	struct node *temp;

	        char *names = (char *)malloc( 16 * sizeof(char) );
		char **filenames;

		for( i=0; i<27; i++ ) {

			temp = &hashTable[i];
			temp = temp->next->next;					/* Points to the top of i-th linked list. */

			while( temp != NULL ) {
				
				memcpy( names, temp->fileName, 16 );			/* Store filenames consecutively. */
				printf("%s, ", names);	
				
			temp = temp->next;						/* Points to the next element of Linked List. */
			}	

		}

	free(names);

return filenames;
}




/********************************************************************
 *
 * emptyI_Node() takes two arguments. The first is a pointer to 
 * the first i-Node of file and the second is the sequence number 
 * of this i-Node. 
 * This function initializes every i-Node and informs block and
 * i-Node Bit Map at the same time.
 * It uses a recurence until all needed structures informed.
 * 
 *******************************************************************/  

void emptyI_Node( struct i_Node *start, int x ) {
			
int temp = 0;		
int i;			
			
	struct i_Node *info = globalINFO->inodes;
			
	char *blockBM = globalINFO->Block_BitMap;
	char *inodeBM = globalINFO->iNode_BitMap;
			
	start->fileSize = 0;						/* Set file Size equal to zero. */
			
		inodeBM[x] = '0';					/* Inform inode Bit Map that this i-node is free now. */
			
		for( i=0; i<8; i++ ) {					/* This loop informs Block Bit Map for its new empty blocks. */
			
			temp = start->block[i];

			start->block[i] = -1;				/* Initializes i-Node's block. */

			if( temp >= 0 )					/* If this block was not available, make it. */					
				blockBM[temp] = '0';	
		}	
			
	if( start->nextNODE >= 0 ) { 					/* If file has size bigger than 8 blocks. */
			
		temp = start->nextNODE;

		start->nextNODE = -1;
			
		emptyI_Node( (struct i_Node *)&info[temp], temp );	/* Call recurcively the function until all i-Nodes have been empty. */
	}		
			

}			
			


