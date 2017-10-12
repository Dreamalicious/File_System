#ifndef SHELL_H
#define SHELL_H



	/*********************************
	 * * * * * * * * * * * * * * * * *                               
	 *	GLOBAL VARIABLES	 *
	 * * * * * * * * * * * * * * * * *				
	 *********************************/


	/**********************************************
	 *
	 * Variable commandTable is a struct its 
	 * members are the names of utilities that
	 * our File System supplies. 
	 *
	 **********************************************/

	struct commandTable {
        
        	char ls[3];
        	char cp[3];
        	char mv[3];
        	char rm[3];
        	char df[3];
        	char cat[4];
        	char echo[5];                                                                   
        	char quit[5];
        	char help[5];
        	char mkfs[5];
		char clear[6];
        	char mount[6];
        	char umount[7];                         
        	
	};                                                                      



	/*********************************
	 * * * * * * * * * * * * * * * * *
	 * 	FUNCTION PROTOTYPES	 *
	 * * * * * * * * * * * * * * * * *
	 *********************************/


	void shell();           
	void prompt();

	int numOfArgs( char * ,char * );

	struct commandTable *initializeCmds();                  




#endif



