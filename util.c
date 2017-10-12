#include "standard.h"
#include "init.h"	
#include "util.h"
#include "directoryService.h"
#include "fileService.h"
#include "hashTable.h"
	


/*******************************************************************
 *
 * mkfs() has two arguments. The first is the number of system's
 * blocks and the second is the size of each block. Both arguments
 * are given by the user, when he creates a new File System.
 * In function's body, using those two arguments we initialize
 * the other important informations about the system and makes them
 * visible from other functions through a global function.
 * 
 *******************************************************************/

void mkfs( int blocks, int blockSize ) {
	
int inodes   = 0;
int ibitMap  = 0;
int bbitMap  = 0;
int dirTable = 0;
	
	inodes   = blocks;					/* Number of i-Nodes are equal to number of system's blocks. */
								/* This decision taken because: 			     */
								/* Supose that every file of our system needs space less     */
								/* than one block. Then we need #inodes equal to #blocks.    */
								/* Of course, in the most of the times, there are too many   */
								/* i-Nodes, but like this we can manipulate worst case.      */
	
	ibitMap  = (inodes / blockSize) + 1;			/* Supose that every character of i-Node Bit Map is an i-Node */
								/* we need (#inodes / blockSize) blocks to manage the info.   */
								/* We add 1 for cases where the result is a float number.     */
	
	bbitMap  = (blocks / blockSize ) + 1;			/* Every character of Block Bit Map is a block. So we need    */
								/* number of char as number of blocks are.                    */
	
	dirTable = ((inodes*32) / blockSize) + 1;		/* Diractory Table can have at most #i-Nodes filenames, */ 
								/* because each filename points to its starting         */
								/* i-Node, so supose we need 32 bytes for every		*/ 
								/* different file we need (32 * #i-Nodes) bytes info.   */
							 	/* Finally we need (32 * #i-Nodes) / blockSize bytes.   */


	
	globalINFO->sb->criticalInfo[0] = blockSize;		/* Informs our new File Sysyem about its atributes. */
	globalINFO->sb->criticalInfo[1] = ibitMap;
	globalINFO->sb->criticalInfo[2] = bbitMap;
	globalINFO->sb->criticalInfo[3] = inodes; 
	globalINFO->sb->criticalInfo[4] = dirTable;
	

	globalINFO->iNode_BitMap = (char *)malloc( inodes  * sizeof(char) );		/* Makes those local memory segments global. */
	globalINFO->Block_BitMap = (char *)malloc( blocks  * sizeof(char) );
	globalINFO->inodes       = init_iNode( inodes );

	memset( globalINFO->iNode_BitMap, '0', inodes  );
	memset( globalINFO->Block_BitMap, '0', blocks  );
}	



/*************************************************************
 *
 * ls() prints on the screen all system's filenames. This 
 * function use utilities that provides directory service
 * and more specific, function files().
 *
 *************************************************************/
	
	
void ls( ) {

char **filenames;

int i;
int num = 0;
	

printf("\n");

        for(i=0; i<20; i++) 
                printf("=");

printf("\n\n");

        num = files( filenames );                       /* A function call from directory Service. */

printf("\n\n");

	for(i=0; i<20; i++) 
		printf("=");

printf("\n\n");

}	



/************************************
 *
 * rm() deletes a file. It uses
 * utilities provided by file
 * Service, and more specific
 * function delete().
 *
 ************************************/

void rm( char *filename ) {


	
	delete( filename ); 		/* delete( char * ) is a fileService's function.  */

}
	
	
	
/***************************************************
 *	
 * df() prints to the screen the number of data 
 * blocks that are available to the user. 
 * In function's body the global variable block
 * bit map is used to counts how many blocks are 
 * still available.
 *	
 **************************************************/
	
void df() {
	
int i;	
int counter = 0;
int num     = 0;
	
	num = globalINFO->sb->criticalInfo[3];	 	 /* #inodes equal to #blocks. */

	for( i=0; i < num; i++ ) {			 /* This loop does all the work... */ 
		
		if( globalINFO->Block_BitMap[i] == '0' )
			counter++;
	}	
		
	printf("\nYou have %d free blocks available from %d\n\n", counter, globalINFO->sb->criticalInfo[3] );

}		
	
	
	
/********************************************************************************
 *	
 * mv() provides to the user the ability to change a filename.
 * In its body: searches in hash Table, where all filenames exists,
 * for the filename and if founds, change its name. 
 * 	
 ********************************************************************************/
	

void mv( char *source, char *destination ) {
	
int index = 0;

	struct node *hashTable = globalINFO->hashTable;				/* Points to the hash Table, where all filenames exists.   */
	struct node *temp      = NULL;
	struct node *help      = NULL;	


        char *buffer = (char *)malloc(16*sizeof(char));                         /* Allocates a local-needed memory segment. */
        memset( buffer, '\0' , 16 );                                            /* Initialize local variable. */


	temp = searchHashTable( hashTable, source );				 /* Searches for filename given, and returns its structure. */ 
	
	if( temp != NULL ) {							 /* If filename given exists, change it. */ 
		
		index = hashFunction(destination);

		help  = &hashTable[index];	
		help  = help->next;

		insertList( help, destination, temp->i_Node );		 

		memset( temp->fileName, '\0', 16 );
		temp->i_Node = 0;

	}
	else {
		printf("MV FAILURE: <%s>: There is no such filename.\n", source);
	}

free(buffer);
}	



/**************************************************************************
 * 
 * 
 * 
 **************************************************************************/

void echo( char *operation, char *filename ) {
	
int ufid = 0;

        struct node *hashTable = globalINFO->hashTable;
        struct node *temp = searchHashTable( hashTable, filename );

        char *buffer = (char *)malloc( 1024 * sizeof(char) );            /*Allocates memory segment with size equal to max block-Size.*/
        memset( buffer, '\0', 1024 );


if( memcmp( operation, ">>", 2 ) == 0 ) {


        if( temp != NULL ) {

                int size = 0;


                printf("Append a string into %s\n", filename);
                sleep(1);
                system("clear");

                ufid = Open(filename);

                        size = file_size(ufid);

                        printf("Press Ctrl^D to exit from file %s\n", filename);
                        printf("===============================================\n\n");

                        while ( fgets( buffer, 1024, stdin ) ) {

                                Write( ufid, buffer, strlen(buffer), size-1 );
                        }

                Close(ufid);

        }
        else
                printf("<%s> : There is no such file.\n", filename );

free(buffer);
}
else if( memcmp( operation, ">", 1 ) == 0 ) {
	
	if( temp == NULL ) {
		
		ufid = create(filename);

		printf("\nFile %s created.\n", filename);	
		Close(ufid);
	
	ufid = 0;
	}
											
	
		printf("Write in file %s: ", filename);
		sleep(1);											
		system("clear");

	ufid = Open(filename);								

		printf("Press Ctrl^D to exit from file %s\n", filename);
		printf("===============================================\n\n");

		while ( fgets( buffer, 1024, stdin ) ) {		/*Read from keyboard string with at most #blockSize chars.*/
					
			Write( ufid, buffer, strlen(buffer), 0 );
		}
	
	Close(ufid);									

free(buffer);
}
else
	printf("Wrong operator.\n");

}


/*********************************************************
 * 
 * 
 * 
 *********************************************************/

void cat( char *filename ) {

int ufid = 0;
int size = 0;

	struct node *hashTable = globalINFO->hashTable;
	struct node *temp      = searchHashTable( hashTable, filename );

	if( temp != NULL ) {

		ufid = Open(filename);

			size = file_size(ufid);					/* Use utility from fileService.c */

                	char *buffer = (char *)malloc( size * sizeof(char) );	/* Allocates memory equal to file-Size and initialize it. */
                	memset( buffer, '\0', size );

			Read( ufid, buffer, size, 0 );				/* Use Read from fileService.c  */

		Close(ufid);

	system("clear");

        printf("===================================================\n\n");
	printf("\t\t%s(Size: %d bytes)\n", filename, size);
        printf("===================================================\n\n");


	printf("%s\n\n", buffer);

	free(buffer);
	}
	else
		printf("There is no such file.\n");


}


void cp( char *source, char *destination ) {

int i;
int size        = 0;
int source_ufid = 0;
int destin_ufid = 0;
	
	struct node *hashTable = globalINFO->hashTable;
	struct node *temp      = NULL;


	temp = searchHashTable( hashTable, source );

	if( temp != NULL ) {

		source_ufid = Open( source );

			size = file_size( source_ufid );
			create(destination);		
				
		char *buffer = (char *)malloc( size * sizeof(char) );
		memset( buffer, '\0', size );
					
			destin_ufid = Open(destination);

				Read ( source_ufid, buffer, size, 0 );
				Write( destin_ufid, buffer, size, 0 );			

			Close(destin_ufid);

		Close(source_ufid);

	free(buffer);
	}
	else {

		printf("CP FAILURE: file: <%s> does not exist.\n", source);
	}

}


void help() {

int i;
	
	for( i=0; i<40; i++ )
		printf("=");
        
	printf("\n\tCATALOGUE OF COMMANDS\n\n");


	printf("mkfs <BLOCKS> <BLOCK-SIZE> :");
	printf("You can define a new file system and choose its characteristics.\n");

	printf("clear \t\t\t   : ");
	printf("Clears command window.\n");

	printf("mount \t\t\t   : ");
	printf("Initializes and informs some data structures about system's last status.\n");
	
	printf("umount\t\t\t   : ");
	printf("File System's back up.\n");

	printf("ls    \t\t\t   : ");
	printf("Print to the screen all system's filenames.\n");

	printf("quit  \t\t\t   : ");
	printf("Exit from File System.\n");

	printf("help  \t\t\t   : ");
	printf("This is me and that's what i can do.\n");
	
	printf("cp <SOURCE> <DESTINATION>  : ");
	printf("Copies filename <SOURCE> to <DESTINATION>.\n");

	printf("mv <SOURCE> <DESTINATION>  : ");
	printf("Changes name of <SOURCE> to <DESTINATION>.\n");

	printf("df    \t\t\t   : ");
	printf("Prints out the number of data blocks that are still available to the user.\n");

        printf("rm  <FILENAME>  \t   : ");
	printf("Removes this file from file system.\n");

	printf("cat <FILENAME>  \t   : ");
	printf("Prints out the contents of file.\n");

	printf("echo <OPERATOR> <FILENAME> : ");
	printf("Writes to file or appends to it, depends to operator.\n");
	printf("                             ");
	printf("If operator is '>' then: until user do Ctrl^D, he writes in the file.\n");
        printf("                             ");
	printf("If operator is '>>' then until user do Ctrl^D, he appends to the file.\n");

printf("\n");

        for( i=0; i<40; i++ )
                printf("=");
printf("\n");
}



