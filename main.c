#include "standard.h"
#include "init.h"
#include "shell.h"

void newDisk( char * );


int main( int argc, char **argv ) {


int fd   = 0;
int test = 0;


	globalINFO            = (struct systemINFO *)malloc( sizeof(struct systemINFO) );
	globalINFO->sb        = init_superBlock();
	globalINFO->hashTable = (struct node *)createHashTable();

	GLOBAL         = ( struct variable * )malloc( 1 * sizeof(struct variable) );

	GLOBAL->default_block_size = 0;
	GLOBAL->fileDescriptor     = 0;
        GLOBAL->default_block_size = 256;
        GLOBAL->fileDescriptor     = 1;
 

	FDTable = init_FDtable( 10 );						/* Global variable points to an array of structs. */
										/* We choose to let user manipulate 10 at most    */
										/* files at the same time.                        */
	GLOBAL->FD_CharMap = (char *)malloc( 10 * sizeof(char) );		/* Global Variable points to 10 bytes.            */

		memset( GLOBAL->FD_CharMap, '0', 10 );	

	system("clear");


	newDisk( "DISKname.txt" );
		
	shell();							


free(globalINFO->sb);
free(globalINFO->hashTable);
free(globalINFO->iNode_BitMap);
free(globalINFO->Block_BitMap);
free(globalINFO->inodes);

return 0;
}




/**************************************************************
 *
 * newDisk() creates a file which is our virtual Hard Disk
 * for our File System.
 * The name of Disk is the (only) argument of this function.
 *
 **************************************************************/


void newDisk( char *fileName ) {
     
int fd = 0;
int df = 0;

       if ( ( fd = open(fileName, O_RDONLY) ) < 0 ) {
            
                printf("Creating file: %s\n", fileName);
                df = open( fileName, O_CREAT );
 
                if( df < 0 ) {
               
                    printf("File %s could not be created.\n", fileName);
                    exit(1);
               }
               
               close(df); 
        }
        else
                close(fd);


}

