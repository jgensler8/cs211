#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXINPUTLEN 81
#define GROWBY 10

int getInput(char **ln);
void setTarget(char **tar, char **rep, char *ln);
void processInput(char *tar, char *rep, char *ln);
int isSetFormat( char* ln);
void getLine(char ***line);

int main( int argc, char *argv[] ){
    char *target, *replacement, *line;
    target = (char*)malloc( sizeof(char));
    replacement = (char*)malloc( sizeof(char));
    *target = '\n';
    int flag = 0;

    do{
	printf("enter string:\n");
	int test = getInput( &line);
        switch( test  ){
	    case 1: processInput( target, replacement, line); break;
	    case 2: setTarget( &target, &replacement, line); break ;
	    case 3: flag = 1; break;
 	    case 4: ;
        }
	free(line);
    }while( flag != 1);
    
    free(target);
    free(replacement);
    return 0;
}

/* getInput sorts on small conditions
 * a quick quit if empty (user only hits enter)
 * the exit sequence ('###')
 * a setformat
 * else process the string
 */
int getInput( char **ln){
    getLine(&ln);
    int length;
    length = strlen( *ln);
    if( length <= 0)
	return 4;
    else if( 	length >= 3 &&
		ln[0][0] == '#' &&
		ln[0][1] == '#' &&
		ln[0][2] == '#')
	return 3;
    else if( isSetFormat( *ln) )
	return 2;
    else
	return 1;
}

/* setTarget accepts a target line and parses the values
 * it then stores the values from the line in a different memory location
 * to be used later
 */
void setTarget(char **tar, char **rep, char *ln){
    char *cpy, *tartemp, *reptemp;
    cpy = (char*)malloc(sizeof(char)*(strlen(ln)+1));
    strcpy( cpy, ln);

    tartemp = strtok( cpy, "#");
    reptemp = strtok( NULL, "#");

    *tar = (char*)realloc( *tar, sizeof(char)*(strlen(tartemp)+1) );
    strcpy(*tar, tartemp );

    if( reptemp != NULL){
	*rep = (char*)realloc( *rep, sizeof(char)*(strlen(reptemp)+1) );
	strcpy(*rep, reptemp );
    }
    else{
	*rep = (char*)realloc( *rep, sizeof(char) );
	*rep = '\0';
    }
	

    free( cpy);
}

/* processInput use the target string and replces every instance of 'tar'
 * with the contents of 'rep'
 */
void processInput(char *tar, char *rep, char *ln){
    if( *tar == '\n'){
	fputs( "too early to process: no format set;\n", stderr);
	return;
    }

    //count times the target exists
    int times = 0;
    char *temp = ln;
    do{
	temp = strstr( temp, tar);
	if(temp != NULL){
	    ++times;
	    temp += strlen(tar);
	}
    }while( temp != NULL);
    
    char* final;
    int i;
    char *before; //before and 'temp' move on the line
    char *finaltemp; //final temp moves on the "final" cstring
    
    if( rep == '\0'){
	final = (char*)malloc( sizeof(char)*
			(strlen(ln) + 1 - strlen(tar)*times) );
	finaltemp = final; //final temp moves on the "final" cstring
	for( i = 0, temp = ln, before = ln; i < times; ++i ){
	    temp = strstr( temp, tar); //find the next target
	    strncpy( finaltemp, before, temp-before); //copy from the end of the last tar to new
	    finaltemp += (temp-before); //move finaltemp over the range copied
	    before += (temp-before) + strlen(tar); // move temp over range copied and tar
	    temp += strlen(tar); //move temp over the target
        }
        strcpy( finaltemp, before);
    }
    else{    
    	final = (char*)malloc( sizeof(char)*
			(strlen(ln) + 1 + (strlen(rep)-strlen(tar))*times) );
	finaltemp = final; //final temp moves on the "final" cstring
	for( i = 0, temp = ln, before = ln; i < times; ++i ){
	    temp = strstr( temp, tar);
	    strncpy( finaltemp, before, temp-before);
	    finaltemp += (temp-before); 
	    strncpy( finaltemp, rep, strlen(rep)); //**place the replacement on the end
	    finaltemp += strlen(rep); //**move over the replacement
	    before += (temp-before) + strlen(tar);
	    temp += strlen(tar);
	}
	strcpy( finaltemp, before);
    }

    //print the final solution
    printf("\n##########\nBEFORE:%s; #SUBS:%d; FINAL:%s\n\n", ln, times, final);
    free(final);
}

/* isSetFormat returns 0 if the line sent is not in the format of
 * "#[text]#[text or no text]#..."
 * if there are trailing chars after the 3rd '#' this input is still counted
 * as a format string (because it can be parsed as one)
 */
int isSetFormat(char* ln){
    int length, i, count;
    length = strlen( ln);
    if( length <= 3) //after this we know the length is at least 4
	return 0;
    else if( 	ln[0] == '#' &&
		ln[1] == '#'){
	fputs("Invalid format string: try again;\n", stderr);
	return 0;
    }

    for(i = 0, count = 0; i < length; ++i){
	if( ln[i] == '#')
	    ++count;
    }
    
    if( count >= 3)
	return 1;
    else
	return 0;
}

/* getLine reads in a line (of any length!) and keeps this value in the passed
 * char * value
 */
void getLine(char*** line){
    char* temp;
    int flag = 0;
    int i, k;
    temp = (char*)malloc( sizeof(char)*GROWBY );
    for(i = 1; flag != 1; i++){
        /* because fgets writes a null, we must overwrite this null
           if we want the string to "look" the same as it was given
           to stdin
        */
        temp = (char*)realloc(temp , sizeof(char)*(i*GROWBY) );
        fgets( temp+((GROWBY-1)*(i-1)), GROWBY, stdin);
        //scan the last part of the string to see if we are done
        for( k = 0 ; k < strlen(temp) ; k++){
            if( temp[k] == '\n'){
                flag = 1;
                break;
            }
        }
	temp[k] = '\0';
    }
    temp = (char*)realloc(temp, sizeof(char)*(strlen(temp)+1) );
    **line = temp;
}
