#include <stdio.h>
#include <stdlib.h>
#define VISITED 'V'

typedef struct point{
    int row, col;
} Point;

typedef struct maze{
    int rows, cols; //size of maze
    Point Start; //starting pos
    Point End; //ending pos
    char **head; //point to board array
} Maze;

typedef struct stack{
    Point *Elems; //pointer to top of stack
    int numE; //count of elems
    int mSize; //max size
} Stack;

//maze fxs
void parseArgs( int argc, char** argv , char**, int*);
void gatherMaze( Maze*, int, FILE*);
void solveMaze( Maze*, int);
int readPoint( FILE*, Point* , Maze);
void freeMaze( Maze*);
void markMaze( Maze*, Point, char);
char getMazeChar( Maze*, Point);
int isValidMovement( Maze* , Point );
//Point
int isValidPos( Point, Maze);
int areEqual( Point, Point);
//stack fx
void initStack( Stack* );
int isEmpty( Stack* );
int push( Stack* , Point, int);
Point pop( Stack* , int);
Point peek( Stack*, int);

int main( int argc, char* argv[]){
    int debugMode = 0;
    char* toOpen;
    FILE *file;
    parseArgs( argc, argv, &toOpen, &debugMode);
    file = fopen( toOpen, "r");

    Maze mMaze;
    gatherMaze( &mMaze, debugMode, file); //read from input and assumble maze
    printf("	\nRows:%d Cols:%d\nStart row:%d Start col:%d\nEnd row:%d End col:%d:\n", 
		mMaze.rows, mMaze.cols, mMaze.Start.row, mMaze.Start.col, mMaze.End.row, mMaze.End.col);

    int i,j;
    for( i = 0; i < mMaze.rows; i++){
	for(j = 0; j < mMaze.cols; j++){
	    printf("%c", mMaze.head[i][j]);
   	}
   	printf("\n");
    }

    solveMaze( &mMaze, debugMode);

    for( i = 0; i < mMaze.rows; i++){
	for(j = 0; j < mMaze.cols; j++){
	    printf("%c", mMaze.head[i][j]);
   	}
   	printf("\n");
    }

    freeMaze( &mMaze);
    fclose( file);
    return 0;
}

/* parseArgs will set the a file to read from into "file"
 * and set debug mode to true, if the arg is supplied
 */
void parseArgs( int argc, char* argv[], char** file, int *debugMode){
    int i;
    FILE *temp;
    char debug[] = "-d";
    for( i = 1; i < argc; i++){
	if( 0 == strcmp(argv[i], debug) )
	   *debugMode = 1;
	
	temp = fopen(argv[i], "r");
	if( temp != NULL){
	    *file = argv[i];
	    fclose(temp);
	} 
    }
}

/* checks if the point is out of bounds
 * This function is a little tricky because I record the cols as
 * 10, for exampl. There are 10 malloc-ed but I only want to 
 * check 0 to 10.
 */
int isValidPos( Point temp, Maze zMaze){
    if( temp.row < 0 || 
	temp.row >= zMaze.rows ||
	temp.col < 0 ||
	temp.col >= zMaze.cols)
	return 0;
    else
	return 1;
}

char getMazeChar( Maze* mMaze, Point P){
    return (mMaze->head)[P.row][P.col];
}

/* gatherMaze reads the rows, cols, point start, point end, and the
 * possible places of the walls for the maze
 * also malloc's the board
 */
void gatherMaze( Maze *mMaze, int d, FILE *file){
    //READ THE ROWS AND COLS
    int flag = 1, x, y;
    Point temp;
    while( flag){
	fscanf( file, "%d%d", &temp.row , &temp.col);
	if( temp.row > 0 && temp.col > 0){
	    mMaze->rows = temp.row;
	    mMaze->cols = temp.col;
	    flag = 0;
	}
	else
	    fprintf( stderr, "INVALID size of the board\n");
    }
    //malloc the board
    mMaze->head = (char**)malloc( (mMaze->rows)*sizeof(char*) );
    for( x = 0; x < mMaze->rows ; ++x){
	(mMaze->head)[x] = (char*)malloc( (mMaze->cols)*sizeof(char) );
	for( y = 0; y < mMaze->cols; ++y)
	    (mMaze->head)[x][y] = '.';
    }
    //read start
    readPoint( file, &temp, *mMaze);
    mMaze->Start.row = temp.row;
    mMaze->Start.col = temp.col;
    (mMaze->head)[ mMaze->Start.row ][ mMaze->Start.col] = 's';
    //read end
    readPoint( file, &temp, *mMaze);
    mMaze->End.row = temp.row;
    mMaze->End.col = temp.col;
    (mMaze->head)[ mMaze->End.row][ mMaze->End.col] = 'e';
    //assign "walls" if possible
    while( readPoint( file, &temp, *mMaze) == 1
		&& 's' != getMazeChar( mMaze, temp)
		&& 'e' != getMazeChar( mMaze, temp) )
        (mMaze->head)[ temp.row][ temp.col] = '*';
}

// reads a point from passed file, returns 0 if the file is empty
// EX: if a point 9,1 is read, 8,0 is returned
int readPoint( FILE* file, Point* p, Maze zMaze){
    int flag = 1;
    do{
	if( EOF == fscanf( file, "%d%d", &p->row , &p->col))
	    return 0;
	--(p->row);//adjust from user input
	--(p->col);
	if( isValidPos( *p, zMaze) )
	    flag = 0;
	else
	    fprintf( stderr, "ERROR Reading Point\n");
    }while( flag);
    return 1;
}

void freeMaze( Maze* mMaze){
    int x;
    for( x = 0; x < mMaze->rows; ++x){
	free( (mMaze->head)[x]);
    }
    free(mMaze->head);
}

/* algorithm to solve the maze
 * Depth search: finds the first path and prints it
 * note: this done not find the BEST path
 */
void solveMaze( Maze *mMaze, int d){
    int endFound = 0;
    Stack mStack;
    Point up, down, left, right;
    initStack( &mStack);
    push( &mStack, mMaze->Start, d); //push start point
    markMaze( mMaze, mMaze->Start, VISITED);//mark start as visited
    while( !isEmpty( &mStack) && endFound == 0){ //not empty; not found
	if( areEqual( peek(&mStack, d), mMaze->End) )//if top is end, done
	    endFound = 1;
	up.row = peek(&mStack, d).row + 1;
	up.col = peek(&mStack, d).col;
	down.row = peek(&mStack, d).row - 1;
	down.col = peek(&mStack, d).col;
	left.row = peek(&mStack, d).row;
	left.col = peek(&mStack, d).col - 1;
	right.row = peek(&mStack, d).row;
	right.col = peek(&mStack, d).col + 1;
	if( isValidMovement( mMaze, up) ){
	    push( &mStack, up, d);
	    markMaze( mMaze, up, VISITED);
	}
	else if( isValidMovement( mMaze, down) ){
	    push( &mStack, down, d);
	    markMaze( mMaze, down, VISITED);
	}
	else if( isValidMovement( mMaze, left) ){
	    push( &mStack, left, d);
	    markMaze( mMaze, left, VISITED);
	}
	else if( isValidMovement( mMaze, right) ){
	    push( &mStack, right, d);
	    markMaze( mMaze, right, VISITED);
	}
	else
	    pop( &mStack, d); //remove the top of the stack
    }
    if( isEmpty( &mStack) )
	printf("NO SOLUTION\n");
    else{
	printf("HERE IS THE SOLUTION (bottom is start)\n");
	printf("(row,col)\n");
	while( !isEmpty( &mStack) ){
	    up = pop ( &mStack, d);
	    printf("(%d,%d)\n", up.row, up.col);
	}
    }

    free( mStack.Elems);
}

int isValidMovement( Maze* mMaze, Point p){
    if( isValidPos( p, *mMaze) // either empty OR exit
	&& ('.' == getMazeChar(mMaze, p) || 'e' == getMazeChar(mMaze, p)))
	return 1;
    else return 0; 
}


/****************STACK FUNCTIONS *****************************/
//contains flag for debugging
int push( Stack* S, Point toPush, int flag){
    int i;
    Point* newStack;
    if(flag) printf("PUSH");
    //if needed, grow the array ****GROW FUNCTION****
    if( S->mSize == S->numE){
	if(flag) printf("\tsize before push: %d\n", S->mSize);
	
	newStack = (Point*)malloc( sizeof(Point)*((S->mSize)*2) );
	for( i = 0 ; i < S->mSize ; ++i){
	    newStack[i].row = (S->Elems)[i].row;
	    newStack[i].col = (S->Elems)[i].col;
	}
	free(S->Elems);
        S->Elems = newStack;
	S->mSize *= 2; //reflect the growth
	if(flag){
	    printf("\tsize after push: %d;", S->mSize);
	    printf("\tvalues copied: %d\n", i);
	}
    }
    if(flag) printf("\tvalue to be pushed: X:%d Y:%d\n", toPush.row, toPush.col);
    (S->Elems)[S->numE].row = toPush.row;
    (S->Elems)[S->numE].col = toPush.col;
    ++(S->numE);

    return 1;    
}

//contains flag for debugging
Point pop(Stack* S, int flag){
    Point garbage;
    garbage.row = -1; garbage.col = -1;
    if( S->numE <= 0)
	return garbage;

    S->numE--;
    if(flag){
	printf("POP\tvalue popped: X:%d Y:%d\n"	, (S->Elems)[S->numE].row
						, (S->Elems)[S->numE].col);
    }
    return (S->Elems)[S->numE];
}

//allocs space and sets vars to reflect
void initStack( Stack* S){
    S->mSize = 16;
    S->numE = 0;
    S->Elems = malloc( sizeof(Point)*16 );
}

//checks to see if the stack is empty
int isEmpty( Stack *S){
    if( S->numE == 0)
	return 1;
    else
	return 0;
}

Point peek( Stack* S, int d){
    Point temp;
    if(d) printf("PEEK (POP+PUSH)\n");
    temp = pop( S, d);
    push(S, temp, d);
    return temp;
}


void markMaze( Maze* mMaze, Point p, char c){
    mMaze->head[p.row][p.col] = c;
}

int areEqual( Point a, Point b){
    return (a.row == b.row && a.col == b.col)?1:0;
}
