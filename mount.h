#ifndef MOUNT_H
#define MOUNT_H
 

	void mount();
	void inform( struct superBlock * , char * );
	void takeFilenames( struct node * , char *, int );
	void takeIndexes  ( struct i_Node * , char * , int );
	
	
#endif


