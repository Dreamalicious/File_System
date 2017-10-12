#include "standard.h"
#include "init.h"

/**********************************************************
 *
 * init_superBlock() allocates memory for a new struct
 * superBlock, initializes it's contents and returns
 * a pointer in this memory segment. 
 *
 **********************************************************/

struct superBlock * init_superBlock() {

int i;

	struct superBlock *temp = ( struct superBlock * )malloc( sizeof(struct superBlock) );

	for( i=0; i<5; i++ ) {

		temp->criticalInfo[i] = 0;
	}

return temp;
}


/***********************************************************
 *
 * init_iNode() takes as argument the number of i-Nodes 
 * we want to use, allocates memory for them and 
 * initializes them. 
 * Then returns a pointer to the starting struct of array.
 *
 ***********************************************************/

struct i_Node *init_iNode( int num ) {

int i, j;

	struct i_Node *temp  = NULL;
	struct i_Node *inode = malloc( num * sizeof( struct i_Node ) );

	for( j=0; j<num; j++ ) {

		temp = &inode[j];

		temp->fileSize = 0;

		for( i=0; i<8; i++ ) {
		
			temp->block[i] = -1;
		}

		temp->nextNODE = -1;
	}


return inode;
}



/****************************************************
 *
 * init_FDtable() returns an array with #argument
 * struct FDtable initialized variables.
 * 
 ****************************************************/

struct FDtable *init_FDtable( int num ) {

int i;
	struct FDtable *temp = (struct FDtable *)malloc( num * sizeof(struct FDtable) );
	struct FDtable *p    = NULL;

	for( i=0; i<num; i++ ) {

		p = &temp[i];

		memset( p->filename, '\0', 16 );

		p->iNode = -1;
		p->ufid  = -1;		
	}

return temp;
}

