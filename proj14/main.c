#include <stdlib.h>
#include <stdio.h>
#include "line.h"
#include "graph.h"


typedef struct command_struct{
  char command;
  int num1;
  int num2;
  char* fileName;
} *Command;

typedef struct data_struct{
  int *indicies;
  int maxSize;
} Data;

/* func:  init a command struct
 * ret:   command struct
 */
Command com_init(){
  Command c = malloc( sizeof( struct command_struct) );
  c->command = '\0';
  c->num1 = -1;
  c->num2 = -1;
  c->fileName = NULL;
  return c;
}

/* param: char*, file name
 * ret:
 *    0: not a valid name
 *    1: a valid file name
 */
int gen_is_valid_fname( char* name){
  FILE* tempFile;
  tempFile = fopen( name, "r") ;
  if( tempFile == NULL) return 0;
  else fclose( tempFile);
  return 1;
}

/* param: line to parse
 * ret: initialize command with "instructions"
 */
Command com_parse(char* line){
  Command c = com_init();
  int numRead = 0;
  sscanf(line, " %c%n", &c->command, &numRead);
  line += numRead;
  switch( c->command){
    case 'q': break; //CHECK FOR NEGATIVES TODO
    case 't': sscanf( line," %d%n", &c->num1, &numRead); 
              line += numRead;
              sscanf( line," %d%n", &c->num2, &numRead); 
              break;
    case 'r': sscanf( line," %d%n", &c->num1, &numRead);
              break;
    case 'i': sscanf( line," %d%n", &c->num1, &numRead); 
              line += numRead;
              sscanf( line," %d%n", &c->num2, &numRead);
              break;
    case 'd': sscanf( line," %d%n", &c->num1, &numRead); 
              line += numRead;
              sscanf( line," %d%n", &c->num2, &numRead);
              break;
    case 'l': break;
    case 'f': sscanf( line, " %s", &c->fileName);
              if( !gen_is_valid_fname( c->fileName) )
                fprintf( stdout, "CAN'T OPEN GIVEN FILE\n");
    default:
      fprintf( stdout, "INVALID COMMAND\n");
  };
  return c;
}

/*
 *
 */
void gen_file_parse(FILE* file){

}

/* param: int, argc
 * param: char**, argv
 * param: FILE&*
 * ret:
 *    -1:         a valid filename has not been found
 *    otherwise:  a valid file has been found at this index
 */
int gen_parse_args( int argc, char** argv){
  int i, flag = -1;
  FILE* tempFile;
  for( i=1; i<argc; ++i){
    if( gen_is_valid_fname( argv[i]) ) flag = i;
  }
  return flag;
}

//**************** CMD FXS *********************

void cmd_can_travel( Graph g, Command c){
  int val = graph_dfs_target( g, &c->num1, &c->num2);
  switch( val){
    case 0: fprintf( stdout, "SORRY, no path found\n"); break;
    case 1: fprintf( stdout, "SUCCESS, path found!\n"); break;
    case 2: fprintf( stdout, "ERROR, path found but cycles detected\n"); 
            break;
    default: fprintf( stderr, "ERROR, invald path criteria\n");
  }
}

void cmd_remove_all( Graph g, Command c){
  if( c->num1 > 0) graph_resize( g, c->num1);
  else fprintf( stdout, "SORRY, VALUE NOT BIG ENOUGH\n");
}

void cmd_insert( Graph g, Command c, Data* d){
  printf("cmd insert:%d,%d\n", c->num1, c->num2);
  graph_add_edge( g, &(c->num1), &((d->indicies)[c->num2]) );
}

void cmd_delete( Graph g, Command c, Data* d){
  //graph_delete_edge( g, c->num1, c->num2);
}

void cmd_list( Graph g){
  graph_list( g);
}

void cmd_exec_file( Graph g, Command c){

}

// **************** GRAPH FXS ***************

int graph_cmp(void* a, void* b){
  int *A = (int*)a;
  int *B = (int*)b;
  fprintf( stdout, "comparing %d with %d\n", *A, *B);
  if( *A == *B) return 0;
  return *A < *B ? -1 : 1;
}

int graph_val_to_index( void* a){
  int *val = (int*)a;
  return *val;
}

void graph_print( void* a){
  int *val = (int*)a;
  fprintf( stdout, " %d", *val);
}

//********** DATA FXS ********************

void data_try_grow( Data* d, Command c){
  int largest = (c->num1 >= c->num2) ? c->num1 : c->num2;
  if( largest >= d->maxSize){  
    free( d->indicies);
    d->maxSize = (largest+1)*c->num1;
    int* newArr = (int*)malloc( d->maxSize*sizeof(int));
    int i;
    for( i=0; i<d->maxSize; ++i)
      newArr[i] = i;
    d->indicies = newArr;
  }
}

void data_init( Data *d){
  d->maxSize = 100;
  d->indicies = (int*)malloc( d->maxSize*sizeof(int));
  int i;
  for( i=0; i<d->maxSize; ++i) (d->indicies)[i] = i;
}

//***************** MAIN ********************

int main( int argc, char** argv){
  int exit = 1;
  Line line = line_init( stdin);
  Graph g = graph_init( graph_cmp, graph_val_to_index, graph_print);
  Data d; data_init( &d); 
  while( exit){
    line_read_line( line);
    char* chunk = get_line( line);
    Command c = com_parse( chunk);
    switch( c->command){
      case 'q': exit = 0; break;
      case 't': cmd_can_travel( g, c); break;
      case 'r': cmd_remove_all( g, c); break;
      case 'i': data_try_grow( &d, c); cmd_insert( g, c, &d); break;
      case 'd': cmd_delete( g, c, &d); break;
      case 'l': cmd_list( g); break;
      case 'f': cmd_exec_file(g, c); break;
      default: break;
    }
    free( c);
    free( chunk);
  }
  free( d.indicies);
  graph_free(g);
  line_free( line);
  return 0;
}
