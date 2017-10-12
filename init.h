#ifndef INIT_H
#define INIT_H



       /***********************************************************************************
	***************************				***************************
	***************************	GLOBAL VARIABLES	***************************
	***************************				***************************
	***********************************************************************************/



        /**********************************************
         *
         * Varible struct superBlock is a well
         * defined varible which takes the initial
         * information from HardDisk.dat when
         * user execute command "mount".
         *
         **********************************************/

        struct superBlock {
        
              int criticalInfo[5];		/* array which contains critical system informations in order:  */

				          	/* Contains the size of block in our File System.               */
                       				/* Contains the number of blocks needed for I-node Bit Map.     */
                  			    	/* Contains the number of blocks needed for Block  Bit Map.     */
                     				/* Contains the number of I-nodes of our File System.           */
                    				/* Contains the number of blocks needed for directory Table.    */
        };
		
/*
=========================================================================================================================================
*/		
		
	struct systemINFO {
		
		struct superBlock *sb;		/* Points to a struct which contains superblock's info. */
		struct node       *hashTable;	/* Points to a hash table which contains all filenames of our File System. */
		
		char *iNode_BitMap;
		char *Block_BitMap; 
		
		struct i_Node *inodes;		/* Points to an array of i-Nodes. */
		
	} *globalINFO;				/* A global variable which initialized in our main function */
						/* and contains all needed info for our File System.        */
		
		


	struct i_Node {

		int fileSize;		/* Contains the size of file that this is the starting i-Node.  */
		
		int block[8];		/* Contains the number of block, where file's data exists. */
					/* An i-Node points directly to 8 different Data Blocks,   */
					/* if our file needs more space, next integer points to    */
					/* the next avaiable i-Node (if exists).                   */

		int nextNODE;		/* Points to the next i-Node. */
	};


	
	struct FDtable {

		char filename[16];	/* Filename assosiated with this file descriptor. */

		int iNode;		/* Starting i-Node of file. */
		int ufid;		/* Current file descriptor, takes value from global variable fileDescriptor, defined in init.h */

	} *FDTable;


	struct variable {

	        int default_block_size;         /* Used as default block size to read the critical information from our hard disk. */
        	int fileDescriptor;             /* Used as starting value for file Descritptors, which are assosited with files    */
                	                        /* when open.                                                                      */
        	char *FD_CharMap;               /* Is a Map which informs as about the free spaces in File Descriptor Table.       */
                	                        /* Takes initial values in main.c                                                  */
	} *GLOBAL;


	char *nameOFdisk;


/*
================================================================================================================================================
*/

 
       /***********************************************************************************
        ***************************                             ***************************
        ***************************     FUNCTION PROTOTYPE      ***************************
        ***************************                             ***************************
        ***********************************************************************************/
 

        struct superBlock * init_superBlock();
	struct i_Node     * init_iNode  ( int );
	struct FDtable    * init_FDtable( int );

#endif

