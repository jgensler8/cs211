#include <stdio.h>
#include <stdlib.h>
#define WIDTH 8

typedef struct stack{
    int *Elems; //pointer to top of stack
    int num_elems; //count of elems
    int max_size; //max size
} *Stack;

void parseArgs( int argc, char* argv[], char** file_name){
    int i, found_files = 0;
    FILE *temp;
    for( i = 1; i < argc; i++){
        temp = fopen(argv[i], "r");
        if( temp != NULL){
          found_files++;
         *file_name = argv[i];
         fclose(temp);
        }
    }

    if(found_files == 0 || found_files > 1){
      fprintf( stderr, "ERROR, MULTIPLE FILE NAMES GIVEN, TERMINATING\n");
      abort();
    }
}

/*************** FUNCTIONS *****************************/
int push( Stack S, int toPush){
    int i;
    int* newStack;
    //if needed, grow the array ****GROW FUNCTION****
    if( S->max_size == S->num_elems){
        newStack = (int*)malloc( sizeof(int)*((S->max_size)*2) );
        for( i = 0 ; i < S->max_size ; ++i){
         newStack[i] = (S->Elems)[i];
        }
        free(S->Elems);
        S->Elems = newStack;
        S->max_size *= 2; //reflect the growth
        printf("\tsize after push: %d\n", S->max_size);
    }
    (S->Elems)[S->num_elems] = toPush;
    ++(S->num_elems);
    return 1;
}

int pop(Stack S){
    S->num_elems--;
    return (S->Elems)[S->num_elems];
}

//allocs space and sets vars to reflect
Stack initStack(){
    Stack S = malloc( sizeof( struct stack) );
    S->max_size = 5;
    S->num_elems = 0;
    S->Elems = (int*)malloc( sizeof(int)*5 );
    return S;
}

//checks to see if the stack is empty
int isEmpty( Stack S){
    if( S->num_elems == 0)
        return 1;
    else
        return 0;
}

int peek( Stack S){
    int temp;
    temp = pop( S);
    push(S, temp);
    return temp;
}

void read_from_file( Stack odd, Stack even, char* file_name){
  FILE* file = fopen( file_name, "r");
  int temp;
  while( EOF != fscanf( file, "%d", &temp)){
    //printf("%d\n", temp);
    if( temp % 2 == 0) push( even, temp);
    else push( odd, temp);
  }
  fclose( file);
}

void print_rev( Stack S, int counter){
  if( S->num_elems == 0) return;
  int temp = pop( S);
  print_rev( S, (counter+1)%WIDTH ); //turtles!
  printf("\t%d", temp); 
  if( counter%WIDTH == 0) printf("\n");
}

void stack_free( Stack S){
  free( S->Elems);
  free( S);
}

int main( int argc, char* argv[]){
  printf("\n\n\n\n~~~~~~~~~~~~~~~~~~~~~A NOTE~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("because my print function uses recursion, the FIRST line of\n");
  printf("my output will have a number between 1 and 8. this may look funny\n");
  printf("so I hope it is readable 'enough'\n");
  printf("enjoy! Jeff Gensler, cs211, lab 1pm to 2:50, jgensl2@uic.edu\n\n\n\n");

  char* file_name = NULL;
  parseArgs( argc, argv, &file_name);
  Stack odd = initStack();
  Stack even = initStack();
   
  read_from_file( odd, even, file_name);

  printf("even values:\n");
  print_rev( even, 0);
  printf("\n");
  
  printf("odd values:\n");
  print_rev( odd, 0);
  printf("\n");

  stack_free( odd);
  stack_free( even);

  return 0;
}




