#include "standard.h"
#include "fileService.h"
#include "hashTable.h"
#include "init.h"



/****************************************************
 *
 * Close takes as argument an integer which is 
 * the ufid of opened file and we can find it 
 * in FDTable, if file is opened. So we search for 
 * it and if found we initializes this record of 
 * table, so the file is closed. 
 *
 ****************************************************/

int Close( int ufid ) {

int i = 0;

        struct FDtable *p = &FDTable[0];				/* Points to the start of File Descriptor Table. */
		

		while( (p->ufid != ufid) && i <= 10 ) {			/* This loop finds the ufid in FDT */

			i++;
	                p = &FDTable[i];			
		}

		if(i == 10) {

			printf("CLOSE FAILURE: you try to close a file who is not open.\n");
			return -1;
		}
		else {

	                GLOBAL->FD_CharMap[i] = '0';			/* Shows us that this place is now available. */

        	        memset( p->filename, '\0', 16 );		/* Now we initialize all clues of this record */

                	p->iNode = -1;
                	p->ufid  = -1;
		}

return 0;
}



/***********************************************
 *
 * file_size() takes as argument the ufid of 
 * an opened file. We search, using it as key, 
 * for its filename in FDTable and if founds it
 * we use its matching filename to search in 
 * hash Table, from this record we found the 
 * starting i-Node of file and in this i-Node
 * the artibute : Size of file.
 * We just returns it...
 *
 ***********************************************/

int file_size( int ufid ) {

int i = 0;

        struct node *hashTable  = globalINFO->hashTable;                /* Points to hash Table where are stored all system's filenames. */
	

        struct FDtable *p = &FDTable[0];				/* Points to the start of File Descriptor Table. */

		for( i=0; i<10; i++ ) {					/* This loop finds the ufid in FDT */

                        p = &FDTable[i];

			if( p->ufid == ufid )
				break;
                }

                if(i == 10) {

                        printf("CLOSE FAILURE: you try to close a file who is not open.\n");
                        return -1;
                }
		else {
			
			struct node *temp = searchHashTable( hashTable, p->filename );

			if( temp == NULL ) {

				printf("Error: You remove an open file...\n%s there is no longer exists.\n\n", p->filename );
				return -1;
			}

	
			struct i_Node *in = globalINFO->inodes;
			struct i_Node *pt = &in[temp->i_Node];

		return pt->fileSize;
		}

return 0;	/* This command never executed. */
}



/*******************************************************************
 *
 * Read() has four arguments. Using the first one, we found file's
 * matching info in FDTable. Then we use the filename to find the
 * first i-Node of the file and from there we read which blocks
 * this file has. Then we just store this info in third argument
 * and that's it.
 *
 *******************************************************************/

int Read( int ufid, char *buf, int num, int pos ) {

int i  = 0;
int fd = 0;

        struct node   *hashTable  = globalINFO->hashTable;		/* Points to hash Table where are stored all system's filenames. */
	struct i_Node *iNoDe      = globalINFO->inodes;			/* Points to the start of an array which contains i-Node's info. */
	struct superBlock *s      = globalINFO->sb;			/* Points to system's critical info.                             */


        struct FDtable *p = &FDTable[0];                                /* Points to the start of File Descriptor Table. */
		
		
                for( i=0; i<10; i++ ) {                                 /* This loop finds the ufid in FDT */
		
                        p = &FDTable[i];
				
			if( p->ufid == ufid )
				break;

                }
		
                	if(i == 10) {
		
                        	printf("READ FAILURE: This file is not opened.\n");
                        	return -1;
                	}
			
			
		struct node *temp = searchHashTable( hashTable, p->filename );
		struct i_Node *pt = &iNoDe[temp->i_Node];			/* Points to the first i-Node. */
	
	
	
int cursor = 0;								/* We define some useful local variables */
int rv     = 0;
int start  = 0;

int bs  = s->criticalInfo[0];
int ibm = s->criticalInfo[1];
int bbm = s->criticalInfo[2];
int in  = s->criticalInfo[3];
int bdt = s->criticalInfo[4]; 
  		
	cursor = GLOBAL->default_block_size + (bs * ibm) + (bs * bbm) + (bs * bdt) + (16*10*in);	/* Cursor goes after all starting info. */
	start  = cursor;  		
		
	if( (fd = open( "DISKname.txt", O_RDONLY)) < 0 ) {
		
		perror("Can't read from Hard Drive.\n");
		return -1;
	}	
		
	
	for( i=0; i<8; i++ ) {		/* This loop check the number of bytes to be read and decides what part of code will execute. */
					/* If is bigger than one block, then it reads it using as step block Size and store its part  */
					/* to the third argument variable. 							      */
					/* Else it just read and stores, so then we can print it out in screen.                       */

	if( pt->block[i] >= 0) {

		if( (num <= bs) && (num > 0) ) {		/* If number of bytes to read is less than block-Size. */
		
			cursor += (pt->block[i] * bs);		/* Cursor points to the first data Block. */
		
			lseek( fd, cursor + pos, SEEK_SET );
		
			rv = read ( fd, buf, num );
		
		cursor = start;
		break;
		}
		else if( num > bs ) {
												
		      	cursor += (pt->block[i] * bs);		/* Cursor goes to the data block pointed by file's i-Node. */
								
			lseek( fd, cursor+pos, SEEK_SET );	
			rv  += read( fd, buf, bs );		
			num -= bs;				/* Reduces the number of bytes have read. */
			
			buf += bs;
			
		cursor = start;					/* Cursor goes to the fist data Block again. */
		}
		else
			break;
	}
	
	} 
		
	close(fd);

return rv;
}



/***************************************************************************
 *
 * Write() has four arguments. With the first we found the matching
 * record to FDTable. We use those informations(filename and i-Node) to 
 * inform others (like file Size). Then we just write in the right blocks
 * the given information( which is stored in third argument ).
 *
 ***************************************************************************/

int Write( int ufid, char *buf, int num, int pos ) {


int i;
int fd = 0;

        struct node   *hashTable  = globalINFO->hashTable;              /* Points to hash Table where are stored all system's filenames. */
        struct i_Node *iNoDe      = globalINFO->inodes;                 /* Points to the start of an array which contains i-Node's info. */
        struct superBlock *s      = globalINFO->sb;                     /* Points to system's critical info.                             */


        struct FDtable *p = &FDTable[0];                                /* Points to the start of File Descriptor Table. */


                for( i=0; i<10; i++) {			                /* This loop finds the ufid in FDT */

                        p = &FDTable[i];

			if( p->ufid == ufid )
				break;
                }

                        if(i == 10) {

                                printf("WRITE FAILURE: This file is not opened.\n");
                                return -1;
                        }

	
        struct node *temp = searchHashTable( hashTable, p->filename );
        struct i_Node *pt = &iNoDe[temp->i_Node];			/* Points to the first i-Node. */
	
	pt->fileSize += num;						/* Informs attribute fileSize about its new size. */
	


int cursor = 0;								/* Define some useful local variables. */
int rv     = 0;
int start  = 0;

int bs  = s->criticalInfo[0];						/* And initialize them. */
int ibm = s->criticalInfo[1];
int bbm = s->criticalInfo[2];
int in  = s->criticalInfo[3];
int bdt = s->criticalInfo[4];


        cursor = GLOBAL->default_block_size + (bs * ibm) + (bs * bbm) + (bs * bdt) + (16*10*in);        /* Cursor goes after all starting info. */
        start  = cursor;



        if( (fd = open( "DISKname.txt", O_WRONLY)) < 0 ) {

                perror("Can't write to Hard Drive.\n");
                return -1;
        }


						
		for( i=0; i<8; i++ ) {				/* This loop check for the number of bytes that we want to write:    */
								/* If this number is less than blockSize we allocate a block and     */
								/* write directly to it.					     */
								/* Else if is bigger than block size, we allocate one we write it    */
								/* we reduces the number of bytes to be writen by #blockSize         */
								/* because we write them and do the same for all 8 available blocks. */	

		if( pt->block[i] == -1 ) {

       			if( num > bs ) {
   				
				pt->block[i] = linearSearch(globalINFO->Block_BitMap);
		
				if( pt->block[i] >= 0 ) {

					cursor += (pt->block[i] * bs);          /* Cursor goes to the data block pointed by file's i-Node. */
						
                                	lseek( fd, cursor+pos, SEEK_SET );
                                	rv  += write( fd, buf, bs );
                                	num -= bs;                              /* Reduces the number of bytes have read. */
						
                                	buf += bs;				/* The new starting address of buffer to insert the next string. */

				cursor = start;
				globalINFO->Block_BitMap[pt->block[i]] = '1';	/* Informs Block Bit Map that this block is not free anymore. */
				}
			}		
			else if( (num > 0) && (num <= bs) ) {			/* If this is the last block to write and its size is < blockSize */

                                pt->block[i] = linearSearch(globalINFO->Block_BitMap);

                                if( pt->block[i] >= 0 ) {

                                        cursor += (pt->block[i] * bs);          /* Cursor goes to the data block pointed by file's i-Node. */

                                        lseek( fd, cursor+pos, SEEK_SET );
                                        rv  += write( fd, buf, bs );
                                        num -= bs;                              /* Reduces the number of bytes have read. */	

				cursor = start;				
                                globalINFO->Block_BitMap[pt->block[i]] = '1';   /* Informs Block Bit Map that this block is not free anymore. */
                                }
			break;
			}
			else		
				break;

		}	

		}

	close(fd);

return rv;
}
