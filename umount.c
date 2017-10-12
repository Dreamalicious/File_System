#include "standard.h"
#include "init.h"
#include "hashTable.h"



void umount() {

int fd  = 0;
int pos = 0;

int bs  = 0;
int ibm = 0;
int bbm = 0;
int in  = 0;
int bdt = 0;


/*
===============
SUPER-BLOCK
===============
*/

	bs  = globalINFO->sb->criticalInfo[0];
	ibm = globalINFO->sb->criticalInfo[1];
	bbm = globalINFO->sb->criticalInfo[2];
	in  = globalINFO->sb->criticalInfo[3];
	bdt = globalINFO->sb->criticalInfo[4];


        if( (fd = open( "DISKname.txt", O_RDONLY )) < 0 ) {
	
                        perror("System failed to umount.\n");
                        perror("Try again...\a\n");
			return;
        }	
				
		
		char *buffer  = (char *)malloc( 32 * sizeof(char) );
		char *buffer1 = (char *)malloc( GLOBAL->default_block_size * sizeof(char) );

                memset( buffer, '\0', 32 );
		memset( buffer1, '0', GLOBAL->default_block_size );

				
			itoa( bs, buffer );
			memcpy( buffer1, buffer, 32);
			memset( buffer , '\0', 32  );
			buffer1+=32;
		
			itoa( ibm, buffer );
			memcpy( buffer1, buffer, 32);
			memset( buffer , '\0',  32 );
			buffer1+=32;
		
			itoa( bbm, buffer );
                	memcpy( buffer1, buffer, 32);
			memset( buffer, '\0', 32 );
                	buffer1+=32;
		
			itoa( in, buffer );
                	memcpy( buffer1, buffer, 32);
			memset( buffer, '\0', 32 );
                	buffer+=32;
		
			itoa( bdt, buffer );
                	memcpy( buffer1, buffer, 32);
			memset( buffer, '\0', 32 );
                	buffer+=32;

		lseek( fd, pos, SEEK_SET );
		write( fd, buffer1, strlen(buffer1) );
		pos+=strlen(buffer1);

		free(buffer1);
		free(buffer);
printf("SuperBlock OK\n");		
/*
================
DIRECTORY TABLE
================
*/		

int x;
	char *FileNames = (char *)malloc( bdt * bs * sizeof(char) );
	char *buf       = (char *)malloc( 16 * sizeof(char) );

	memset( FileNames, '0' , sizeof(bdt * bs) );
	memset( buf      , '\0', 16 );


        struct node *hashTable = globalINFO->hashTable;
        struct node *pt        = NULL;
			
			
                for( x=0; x<27; x++ ) {
			
                        pt = &hashTable[x];
                        pt = pt->next->next;						/* Points to the top of i-th linked list. */
			
                        while( pt != NULL ) {
			
                                memcpy( FileNames, pt->fileName, 16 );			/* Store filenames consecutively. */
                                FileNames+=16;
			
				itoa  ( pt->i_Node, buffer );
				memcpy( FileNames, buf, 16 );
				memset( buffer, '\0', 16   );
				FileNames+=16;
			
                        pt = pt->next;							/* Points to the next element of Linked List. */
                        }
			
                }
	
		lseek( fd, pos, SEEK_SET );
		write( fd, FileNames, strlen(FileNames) );
		pos+=strlen(FileNames);

	printf("Diractory Table OK.\n");	
/*	free(FileNames);
	free(buf);	*/
/*			
================
I-NODE BIT MAP
================
*/	
	
	char *buffer2 = (char *)malloc( bs * ibm * sizeof(char) );
	memset( buffer2, '0', bs * ibm );	
	
		memcpy( buffer2, globalINFO->iNode_BitMap, strlen(globalINFO->iNode_BitMap) );
	
		lseek( fd, pos, SEEK_SET );
		write( fd, buffer2, strlen(buffer2) );
		pos+=strlen(buffer2);
		
/*	free(buffer2);*/
	
/*
================
BLOCK BIT MAP
================
*/
	
	char *buffer3 = (char *)malloc( bs * bbm );
	memset( buffer3, '0', bs * bbm );
        
	        memcpy( buffer3, globalINFO->Block_BitMap, strlen(globalINFO->Block_BitMap) );

		lseek( fd, pos, SEEK_SET );
		write( fd, buffer, strlen(buffer3) );
		pos+=strlen(buffer3);

/*	free(buffer3);*/
printf("Bit MAPS OK\n");
/*
==========
I-NODES
==========
*/
		
int i, j;
		
	char *buffer4 = (char *)malloc( 160 * in * sizeof(char) );		
	char *buff    = (char *)malloc( 16 * sizeof(char) );

	memset( buff   , '\0', 16 );
	memset( buffer4, '0' , 160 * in );


		struct i_Node *inodes = globalINFO->inodes;
		struct i_Node *temp   = NULL;


		for( i=0; i<in; i++ ) {

			temp = &inodes[i];
			
			itoa( temp->fileSize, buff );
			memcpy( buffer4, buff, 16 );
			memset( buff   , '\0', 16 );
			buffer4+=16;
	
			for( j=0; j<8; j++ ) {
				
				itoa  ( temp->block[j], buff );
				memcpy( buffer4, buff, 16 );
				memset( buff   , '\0', 16 );
				buffer4+=16;				
			}
			
			itoa  ( temp->nextNODE, buff );
                        memcpy( buffer4, buff , 16 );
			memset( buff   , '\0' , 16 );
			buffer4+=16;
		}

	lseek( fd, pos, SEEK_SET );
	write( fd, buffer4, strlen(buffer4) );
	
/*	free(buffer4);
	free(buff);
*/
printf("Back up successfully.\n");

}		
		
