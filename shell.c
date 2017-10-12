#include "standard.h"
#include "shell.h"
#include "util.h"


struct commandTable *initializeCmds() {

	struct commandTable *command = (struct commandTable *)calloc( 1, sizeof(struct commandTable) ); 


	strcpy( command->ls, "ls" );
	strcpy( command->cp, "cp" );
	strcpy( command->mv, "mv" );
	strcpy( command->df, "df" );
	strcpy( command->rm ,"rm" );	

	strcpy( command->cat, "cat" );
	
	strcpy( command->echo, "echo" );
	strcpy( command->quit, "quit" );
	strcpy( command->help, "help" );
	strcpy( command->mkfs, "mkfs" );	

        strcpy( command->clear, "clear" );
	strcpy( command->mount, "mount" );

        strcpy( command->umount, "umount" );


return command;
}


void shell() {


	int num;  
	int choice;

	int i, j; 

	char *buffer     = (char *) malloc ( 128 * sizeof(char) );
	char *command    = (char *) malloc ( 128 * sizeof(char) );
	char *argument1  = (char *) malloc ( 128 * sizeof(char) );
        char *argument2  = (char *) malloc ( 128 * sizeof(char) );

	struct commandTable *cmd = initializeCmds();



		while(1) {

			prompt();
			
			memset( command   , '\0', 128 );				/* Initializes all local variables which used to */
			memset( argument1 , '\0', 128 );				/* manipulate the interaction with the user.     */
			memset( argument2 , '\0', 128 );
			memset( buffer    , '\0', 128 );
			
			
			choice = 1;
			num    = 0;
			j      = 0;
			
			fgets( buffer, 128, stdin );					/* gets user's command from keyboard */
			
			
			
			
		        while( choice != 4 ) {						/* separates command and argument(s) if exists */
											/* and stores them in different variables to   */
											/* be easily manipulated.                      */
			
                		switch( (int)choice ) {
			
			
                                		case 1:
                                        		for( i=0; (buffer[i] != ' ') && (i < strlen(buffer)); i++ ) 
                                                			command[i] = buffer[i];
			
							j = i+1;
			
		                                        while( buffer[j] == ' ' )
                		                                j++;
			
                                		        choice = 2;
							break;
			
						case 2:
							for( i=j;  buffer[i] != ' ' && i<strlen(buffer); i++ ) 
                                                			argument1[i-j] = buffer[i];
			
							j = i+1;
			
							while( buffer[j] == ' ' )
								j++;

							choice = 3;                                        
							break;

						case 3:
							for( i = j; buffer[i] != ' ' && i<strlen(buffer); i++ ) 
                                                			argument2[i-j] = buffer[i];


		                                        while( buffer[j] == ' ' )
                		                                j++;

							choice = 4;
							break;

						default:
							printf("Error: on communucation with the user.\n");
							break;
				}

        	}



		num = numOfArgs( argument1, argument2 );		/* Counts the number of command's argument(s) */




				if( num == 0 ) {					/* Commands with no arguments executed here */

                                        if( memcmp( cmd->ls, command, 2 ) == 0 )
                                                ls();
                                        else if( memcmp( cmd->mount, command, 5 ) == 0 )
                                                mount();
                                        else if( memcmp( cmd->umount, command, 6 ) == 0 )
                                                umount();
                                        else if( memcmp( cmd->help, command, 4 ) == 0 )
                                                help();
                                        else if( memcmp( cmd->quit, command, 4 ) == 0 )
                                                break;
                                        else if( memcmp( cmd->df, command, 2 ) == 0 )
                                                df();
					else if( memcmp( cmd->clear, command, 5 ) == 0 )
						system("clear");
                                        else
                                                printf("Command -%s not found.\n", command);
                                }
				if( num == 1 ) {					/* Commands with one argument executed here */
											
					if( memcmp( cmd->cat, command, 3 ) == 0 )
						cat( argument1 );
					else if( memcmp( cmd->rm, command, 2 ) == 0 )
						rm( argument1 );
					else
						printf("Command -%s not found.\n", command);					
				}	
				if( num == 2 ) {					/* Commands with two arguments executed here */

					if( memcmp( cmd->mkfs, command, 4 ) == 0 )
						mkfs( atoi(argument1), atoi(argument2) );
					else if( memcmp( cmd->cp, command, 2 ) == 0 )
						cp( argument1, argument2 );
					else if( memcmp( cmd->mv, command, 2 ) == 0 )
						mv( argument1, argument2 );
					else if( memcmp( cmd->echo, command, 4 ) == 0 )
						echo( argument1, argument2 );
					else
						printf("Command -%s not found.\n", command);
				}

			
			}
	
	


}


void prompt() {

	printf("\n>> ");
}


int numOfArgs( char *arg1, char *arg2 ) {

int num = 0;

	if( (arg1[0] != ' ') && (arg1[0] != '\n') && (arg1[0] != '\0') )
		num++;
	if( (arg2[0] != ' ') && (arg2[0] != '\n') && (arg2[0] != '\0') )
		num++;

return num;
}

