#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINELEN 301
#define MAXERRLEN 321

typedef struct{
    char *elems;
    int maxSize;
    int topStack;
} Stack;

int push( Stack* , char, int);
char pop( Stack* , int);
void initStack( Stack* );
void resetStack( Stack* );
char invBrack( char );
void resetLine( char** );
int isEmpty( Stack);

main( int argc, char *argv[] ){
    Stack myStack;
    initStack( &myStack);
    int flag = 0;
    int numRead, sumRead;
    char stdCh, temp, *line, *lntemp, *errStr;
    char openSet[5] = "<{[(";
    line = (char*)malloc( sizeof(char)*MAXLINELEN);
    errStr = (char*)malloc( sizeof(char)*MAXERRLEN);

    if( argc >= 2 && argv[1][1] == 'd')
	flag = 1; // set the debug mode
    
    while( fgets( line, 300, stdin) != NULL){
	lntemp = line; // used to advance scanf
	sumRead = 0; // used to print to err message, if needed
	while( 0 < sscanf( lntemp, "%c%n", &stdCh, &numRead) ){
	    lntemp += numRead;
	    sumRead += numRead;
	    if( stdCh == 'q') // quit
		return 0;
	    else if( strchr( openSet, stdCh) != NULL) // open brack
		push( &myStack, stdCh, flag);
	    else if( strchr( openSet, invBrack(stdCh) ) != NULL){ // close brack
		if( isEmpty( myStack) ){
		    sprintf( errStr, "%*c:Missing Here", sumRead, invBrack( stdCh) );
		    break; //exit (parse line)
		}
		else{ // could match the stack elem, could not
		    temp = pop( &myStack, flag);
		    if( invBrack(stdCh) != temp){
		        sprintf( errStr, "%*c:Expected Here", sumRead, invBrack( temp));
			break; // exit (parse line)
		    }
		    //else the chars match! no other work needed
		}
	    }
        } //end while (parse line)
	printf("%s", line);
	if( *errStr != '\0')
	    printf("%s\n", errStr);
	else
	    printf("Expression is balanced\n");
	resetStack( &myStack);
	resetLine( &errStr);
	printf("\n");
    }//end while (line from stdin)
    

    free(errStr);
    free(line);
    return 0;
}

//contains flag for debugging
int push( Stack* S, char toPush, int flag){
    int i;
    char* newStack;
    if(flag) printf("PUSH\n");
    //if needed, grow the array ****GROW FUNCTION****
    if( S->maxSize == S->topStack){
	if(flag) printf("\tsize before push: %d\n", S->maxSize);
	
	newStack = (char*)malloc( sizeof(char)*((S->maxSize)+2) );
	for( i = 0 ; i < S->maxSize ; ++i)
	    newStack[i] = S->elems[i];
	free(S->elems);
        S->elems = newStack;
	S->maxSize += 2; //reflect the growth
	if(flag){
	    printf("\tsize after push: %d\n", S->maxSize);
	    printf("\tvalues copied: %d\n", i);
	}
    }
    if(flag) printf("\tvalue to be pushed: %c\n", toPush);
    S->elems[S->topStack] = toPush;
    ++(S->topStack);

    return 1;    
}

//contains flag for debugging
char pop(Stack* S, int flag){
    if( S->topStack <= 0)
	return '\0';

    S->topStack--;
    if(flag){
	printf("POP\tvalue popped: %c\n", S->elems[S->topStack]);
    }
    return S->elems[S->topStack];
}

//allocs space and sets vars to reflect
void initStack( Stack* S){
    S->maxSize = 2;
    S->topStack = 0;
    S->elems = malloc( sizeof(char)*2 );
}

//free then  allocs the size and sets the vars to reflect this
void resetStack( Stack* S){
    free(S->elems);
    initStack( &(*S) );
}

char invBrack( char stdCh){
    switch(stdCh){
	case '>': return '<';
	case '}': return '{';
	case ']': return '[';
	case ')': return '(';
	case '<': return '>';
	case '{': return '}';
	case '[': return ']';
	case '(': return ')';
    }
    return '\n';
}

//sets all spaces to null
void resetLine( char** line){
   int i;
   for( i = 0 ; i < MAXERRLEN; ++i)
	(*line)[i] = '\0';
}

//checks to see if the stack is empty
int isEmpty( Stack S){
    if( S.topStack == 0)
	return 1;
    else
	return 0;
}
