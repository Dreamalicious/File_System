#include "standard.h"
#include "init.h"
#include "directoryService.h"
#include "hashTable.h"


/*********************************************
 * 
 * create() takes as argument a filename
 * and creates a new file with this name.
 * Firstly, we search for an other file
 * with the same name, if does't exists 
 * we just create it, else we delete it
 * first and creates it then.
 * 
 *********************************************/

int create( char *fileName ) {

int index = 0;
int num   = 0;

	struct node *hashTable  = globalINFO->hashTable;	/* Points to hash Table where are stored all system's filenames with alphabetical order. */
	struct node *insertHere = NULL;


	struct node *temp = searchHashTable( hashTable, fileName );	/* Search for this filename in this list. Return NULL if filename does not exist. */

	
		if( temp == NULL ) {

			index = hashFunction( fileName );		
	
			insertHere = &hashTable[index];			

			insertHere = insertHere->next;			/* Points to the head of the Linked List. */

			num = linearSearch( globalINFO->iNode_BitMap );	/* Returns an emty i-Node. It defines in help.c */

			if( num < 0 ) {
				
				printf("Disk has no free space.\n");
			}
			else {
				insertList( insertHere, fileName, num );
				globalINFO->iNode_BitMap[num] = '1';
			}
		}
		else {							/* If filename does exists: */
	
			while( delete(fileName) != 0 );			/* Delete file from system. */
		
			printf("File: %s already exists.\n",   fileName);
			printf("File Size of %s is zero.\n\n", fileName);

			create(fileName);				/* Creates an another with the same name. */
		}

return 0;
}
 

	
	
/*************************************************
 *
 * files() counts the number of system's files
 * and argument's pointer, points to an array
 * which contains all filenames.
 * 
 *************************************************/

int files( char **filenames ) {

int num = 0;
int i;
	
	struct node *hashTable = globalINFO->hashTable;			/* Points to hash Table where are stored all system's filenames with alphabetical order. */
	struct node *temp      = NULL;
	struct node *pointer   = NULL;

	pointer = hashTable;						/* Points to the first node of hash Table. */

	for ( i=0; i<27; i++ ) {					/* This loop counts the number of our File System's filenames. */

		temp = (struct node *)(&pointer[i]) -> next -> next;	/* Points to the i-th node of hash Table. */

		while ( temp != NULL ) {

			temp = temp->next;
			num++;						/* Counter who counts the number of system's files. */
		}

	}
	

	filenames = (char **)walkHashTable( num );			/* Points to an array with all system's filenames */
									/* walkHashTable() is implemented in "help.c"     */


return num;
}
	

/*********************************************************
 *		
 * delete() has one arguments. This argument is the 
 * filename of we want to delete.
 * In its body, this function searches for filename
 * given in hash Table and if found it then initializes
 * to zero all bytes of Block and i-Node Bit Maps.
 * Then removes from hash Table this filename.
 *		
 *********************************************************/
		
int delete( char *filename ) {
		
int i;		
int index = 0;		
		
	struct node *hashTable = globalINFO->hashTable;		/* Points to hash Table where are stored all system's filenames. */
	struct node *temp      = NULL;
	struct node *help      = NULL;
		
	struct i_Node *pointer = globalINFO->inodes;		/* Points to system's i-Nodes. */
	struct i_Node *pt      = NULL;					
		
	temp = searchHashTable( hashTable, filename );          /* Search for this filename in this list. Return NULL if filename does not exist. */	
		
	if( temp != NULL ) { 					/* If there is file with that name. */
		
		pt = &pointer[temp->i_Node];			/* Points to the first i-Node of file. */
		
		emptyI_Node( pt, temp->i_Node );		/* Initializes file's i-Nodes and bit and block Bit Maps in those positions. */

		index = hashFunction(filename);			/* Finds where in the Hash Table is filename given. */		
		help  = &hashTable[index];
		help  = help->next->next;
 
		deleteList( help, filename );			/* Deletes this record from Hash Table. */
	}	
	else {	
		
		printf("DELETE FAILURE: There is no such file.\n");
		return -1;
	}	
		
		
return 0;	
}		
		
		
int Open( char *filename ) {

int place = 0;

	struct node *hashTable  = globalINFO->hashTable;        /* Points to hash Table where are stored all system's filenames. */

	struct node *temp = searchHashTable( hashTable, filename );
	struct FDtable *p = NULL;

	if( temp != NULL ) {

		place = linearSearch( GLOBAL->FD_CharMap );		/* Search for an empty space in File Descriptor Table. */

		GLOBAL->FD_CharMap[place] = '1';			/* Shows us that this place is no longer available.    */

		p = &FDTable[place];					/* Points to the right place of file Descriptor Table. */
		
		memcpy( p->filename, temp->fileName, 16 );

		p->iNode = temp->i_Node;
		p->ufid  = GLOBAL->fileDescriptor;

		GLOBAL->fileDescriptor++;
	}
	else {
		printf("OPEN FAILURE: File %s doen't exists.\n", filename);
		return -1;
	}

return p->ufid;
}	
	
 
