#include "standard.h"
#include "hashTable.h"	
#include "linkedList.h"	
#include "init.h"
#include "mount.h"
	
void mount() {
	
	
int fd  = 0;
int pos = 0;
	
	
	char *info = (char *)malloc( GLOBAL->default_block_size * sizeof(char) );
	
	memset( info, '\0', GLOBAL->default_block_size );
	
	
	if( (fd = open( "DISKname.txt", O_RDONLY )) < 0 ) {
	
			perror("System failed to mount.\n");
			exit(1);
	}

/*
===============
SUPER-BLOCK
===============
*/		
		lseek( fd, pos, SEEK_SET );				/* Set file's cursor at the start of the file.   */
		read ( fd, info, GLOBAL->default_block_size );		/* Read critical informations about file system  */
									/* using a default block size(defined in init.h) */

		inform( globalINFO->sb, info );				/* Inform superBlock about system's critical info*/
		
		pos = GLOBAL->default_block_size;			/* This is the last byte has beed read. */

/*
================
DIRECTORY TABLE
================
*/
		struct superBlock *sb = globalINFO->sb;							/* A local variable points to global infos. */

		char *filenames = (char *)malloc( (sb->criticalInfo[0]) * (sb->criticalInfo[4]) * sizeof(char) );
													/* Allocates space to read system's filenames. */
		
		memset( filenames, '\0', (sb->criticalInfo[0]) * (sb->criticalInfo[4]) );		/* Initializes this memory segment. */

		lseek( fd, pos, SEEK_SET );
		read ( fd, filenames, (sb->criticalInfo[0]) * (sb->criticalInfo[4]) );			/* Read all filenames from our Hard Disk, */
													/* with their starting i-Node.            */

		struct node *hashTable = createHashTable();						/* Creates a hash Table to store all file */
													/* data. That makes filename-searching    */
													/* faster.				  */

		takeFilenames( hashTable, filenames, strlen(filenames) );				/* Insert filenames and their starting    */
													/* i-Node into Hash Table.                */ 
		
		pos += strlen(filenames);								/* Cursor is in the end of Directory Table. */

/*
================
I-NODE BIT MAP
================
*/
		globalINFO->iNode_BitMap = (char *)malloc( sizeof(char) * (sb->criticalInfo[0]) * (sb->criticalInfo[1]) );
													/* Allocates memory to read i-Node Bit Map. */

		memset( globalINFO->iNode_BitMap, '\0', (sb->criticalInfo[0] * sb->criticalInfo[1]) );	/* Initializes this memory segment. */
		

		lseek( fd, pos, SEEK_SET );
		read ( fd, globalINFO->iNode_BitMap, strlen(globalINFO->iNode_BitMap) );		/* Read i-Node Bit Map from our Hard Disk. */

		pos += strlen(globalINFO->iNode_BitMap);						/* Cursor goes after i-Node Bit Map. */

/*
================
BLOCK BIT MAP
================
*/

		globalINFO->Block_BitMap = (char *)malloc( sizeof(char) * (sb->criticalInfo[0]) * (sb->criticalInfo[2]) );
													/* Allocates memory to read Block Bit Map. */
		
		memset( globalINFO->Block_BitMap, '\0', strlen(globalINFO->Block_BitMap) );		/* Initializes this memory segment. */
			
		lseek( fd, pos, SEEK_SET );
		read ( fd, globalINFO->Block_BitMap, strlen(globalINFO->Block_BitMap) );		/* Read Block Bit Map from our Hard Disk. */
		
		pos += strlen(globalINFO->Block_BitMap);						/* Cursor goes after block Bit Map. */
/*
==========
I-NODES
==========
*/
		struct i_Node *inode = NULL;
			
		char *InodeInfo = (char *)malloc( 10 * 16 * (sb->criticalInfo[3]) * sizeof(char) );
										/* Allocates memory to store in i-Node's informations. */
										/* Each i-Node has 10 integers and each integer needs  */
										/* 16 bytes in file. (We need less but we use just     */
										/* powers of 2.                                        */
		
		memset( InodeInfo, '\0', 160 * (sb->criticalInfo[3]) );		/* Initializes this memory segment. */

		inode = init_iNode( sb->criticalInfo[3] );			/* Points to an initialized array of struct #systemINODES i-Nodes. */

		lseek( fd, pos, SEEK_SET ); 
		read ( fd, InodeInfo, 160 * (sb->criticalInfo[3]) );		/* Read i-node's info from our Hard Disk. */

		takeIndexes( inode, InodeInfo, sb->criticalInfo[3] );		/* Inform every inode with last system's status. */

		pos += (160 * (sb->criticalInfo[3]) );			/* Cursor goes to the start position of the first data block. */


	close(fd);


	globalINFO->sb           = sb;						/* Informs global variable about system's starting status. */
	globalINFO->hashTable    = hashTable;
	globalINFO->inodes       = inode;


free(InodeInfo);
free(filenames);	
free(info);
	
}	


	
/**********************************************************************
 *
 * inform() takes two arguments. The first is a pointer to an 
 * initialized variable struct superBlock and the second is
 * a string with legth as system's default block size commands.
 * In function's body, superBlock take knowledge about system's
 * critical informations like:
 * block Size, #of blocks needed for system's bit Maps, #of blocks
 * needed to store system's filenames and #of system's i-Nodes.
 * 
 **********************************************************************/

void inform( struct superBlock *sb, char *info ) {

int i;

	char *buffer = (char *)malloc( 32 * sizeof(char) );


	for( i=0; i<5; i++ ) {

		memset( buffer, '\0', 32 );
                memcpy( buffer, info, 32 );

                sb->criticalInfo[i] = atoi( buffer );

		info += 32;					/* System need at most 32bytes for each integer. */
								/* (We no need so much bytes, but we choose to   */
								/*  allocates spaces which are power of 2.       */
	}


free(buffer);

}	
		

/*************************************************************************************
 *
 * takeFilenames() has three arguments. The first is a pointer to an initialized
 * Hash Table, the second is a string with length as big as needed to stored
 * all system's filenames and their i-Nodes and the third is the length of second
 * argument. 
 * In its body, function takes every 32 bytes of second argument and from them
 * takes the filename and the i-Node and stores and insert this information in 
 * hash Table. 
 *
 ************************************************************************************/
		
void takeFilenames( struct node *hashTable, char *filenames, int numOfBytes ) {
				
int i;
int numInode;
int index;

	struct node *pt = NULL;
		
	char *buffer = (char *)malloc( 32 * sizeof(char) );
	char *name   = (char *)malloc( 16 * sizeof(char) );
	char *num    = (char *)malloc( 16 * sizeof(char) );
		
	for( i=0; i < (numOfBytes/32); i++ ) {

		numInode = 0;
		index    = 0;
		
		memset( buffer, '\0', 32 );		
		memcpy( buffer, filenames, 32 );		/* Copy information for the file to variable buffer. */	
								
		memset( name, '\0',   16 );			
		memcpy( name, buffer, 16 );			/* Copy filename from buffer to variable name. */
								
		memset( num, '\0',      16 );			
		memcpy( num, buffer+16, 16 );			/* Copy number of file's starting i-Node from buffer to variable num. */
								
		numInode = atoi( num );				
								

		index = hashFunction( name );
			
		pt = &hashTable[index];				/* Points where hash Function decides to. */
		pt = pt->next->next;

		insertList( pt, name, numInode );		/* Insert to Hash Table a new node, which contains  */
								/* a name of a file and its first i-Node.           */
					
		filenames += 32;				/* From these 32 bytes, the first 16 are used to    */
								/* store a filename and the rest 16 to their first  */
								/* i-Node.					    */	
	}	
	



free(num);
free(name);		
free(buffer);		

}		
		

void takeIndexes( struct i_Node *inode, char *InodeInfo, int num ) {

int i, j;
int value;

	struct i_Node *temp = NULL;

	char *buffer = (char *)malloc( 10 * 16 * sizeof(char) );
	char *number = (char *)malloc(      16 * sizeof(char) );
	

	for( i=0; i<num; i++ ) {

		temp = &inode[i];			/* Points to the right i-Node. */

		memset( buffer, '\0', 160 );
		memset( number, '\0', 16  );
			
		memcpy( buffer, InodeInfo, 160 );
			
		memcpy( number, buffer, 16 );
				
		value = atoi(number);
		temp->fileSize = value;
			
		buffer += 16;
			
		for( j=0; j<8; j++ ) {			/* This loop informs i-Node's indexes. */
			
			value = 0;
			
			memcpy( number, buffer, 16 );
			memset( number, '\0',   16 );
			
			value = atoi( number );
			
			temp->block[i] = value;
			
			buffer += 16;
			
		}	

		
		memcpy( number, buffer, 16 );
		value = 0;
		value = atoi( number );

		temp->nextNODE = value;
			
	InodeInfo += 160;				/* Points to the next i-Node's data. */
	}

	
free(buffer);
free(number);
}
