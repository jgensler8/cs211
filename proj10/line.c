#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "line.h"
#define DEFAULT_SIZE 10

typedef struct line_struct{
  char* line;
  int max_size;
  int cur_size;
  int tot_read;
  FILE* file;
} *Line;

/*func: initializes Line's members
*/
Line line_init( FILE* f){
  Line l = malloc(sizeof(struct line_struct) );
  l->line = malloc(DEFAULT_SIZE*sizeof(char) );
  l->max_size = DEFAULT_SIZE;
  l->cur_size = 0;
  l->tot_read = 0;
  l->file = f;
  return l;
}

/* param: Line
* func: frees the contents of Line and Line itself
*/
void line_free( Line l){
  free( l->line);
  free(l);
}

/* param: Line
* func: doubles the size of &Line's line and copys old contends over
*/
void line_grow( Line l){
  char* bigger = malloc(l->max_size*2); //double the size
  strcpy( bigger, l->line);
  free( l->line);
  l->line = bigger;
  l->max_size *= 2;
}

/* param: Line storage of input
* func: reads from passed file, stores the line in Line's line
* return:
* 0: EOF REACHED
* 1: succesful line read
*/
int line_read_line( Line l){
  char* line_chunk = malloc( DEFAULT_SIZE*sizeof(char) );
  char* prev;
  (l->line)[0] = '\0'; //make first char NULL
  l->tot_read = 0;
  while( NULL != fgets( line_chunk, DEFAULT_SIZE, l->file)){ //read D_S-1 (9chars)
    l->tot_read += DEFAULT_SIZE;
    if(l->tot_read > l->max_size) line_grow( l);
    prev = strchr( l->line, '\0'); //advance prev to the '\0' created by fgets
    strncpy( prev, line_chunk, DEFAULT_SIZE);
    if( NULL != strchr( line_chunk, '\n') ){
      free( line_chunk);
      set_cur_size( l);
      return 1;
    }
  }
  free( line_chunk);
  return 0;
}

/* param: Line
* func: updates the current size based on the length of Line's line
*/
void set_cur_size( Line l){
  l->cur_size = strlen( l->line);
}

/* param: Line's line to stdout
* func: print
*/
void line_print_line( Line l){
  fprintf(stdout,"%s", l->line);
}

/* param: Line
* func: returns a deep copy of the Line's line
*/
char* get_line( Line l){
  char* ret = malloc( (l->cur_size+1)*sizeof(char) );
  strcpy( ret, l->line);
  return ret;
}
