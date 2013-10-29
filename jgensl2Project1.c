/*
a sample "cp" command

takes two arguements:
	1) input file
	2) destination file
the contents of the first file are copied char by char (256 max)

~~~~~~~~only the input file must exist at runtime~~~~~~~~ 

Author: Jeff Gensler
email: jgensl2@uic.edu
project: project 1, week 1
*/

#include <stdio.h>

int main( int argc, char *argv[] ){
	FILE *toOpen;
	FILE *toSave;

	//Show the args
	int c;
	for( c = 0; c < argc; c++){
		printf("%s\n", argv[c]);
	}

	//check the amount of argv supplied
	if( argc != 3){
		printf("INVALID ARGUMENTS. TERMINATING\n");
		return -1;
	}
	
	//check if the toOpen file is in the directory
	if( (toOpen = fopen( argv[1], "r") ) == NULL){
		printf("TARGET FILE DOES NOT EXIST. TERMINATING\n");
		return -1;
	}

	//open the destination file; the file will be created if needed
	toSave = fopen( argv[2], "w");

	while( (c = fgetc( toOpen)) != EOF){
		//printf("%c\n", c);
		fputc( c, toSave);
	}

	fclose( toOpen);
	fclose( toSave);

	return 0;
}
