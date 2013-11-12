#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "line.h"
#define PRINT_FORMAT ":%d: "

typedef struct node_struct{
  struct node_struct *next;
  int val;
} Node;

typedef struct ll_struct{
  Node *head;
  int size; //may not get used, here for pure functionality
} *Ll;

// BASIC LL  FXS
Ll ll_init(){
  Ll l = malloc( sizeof( struct ll_struct) );
  l->head = NULL;
  l->size = 0;
  return l;
}

//insert the node into the list 
// smaller numbers are closer to head and bigger ones are farther
void ll_insert_helper( Node* n, Node* add, int d){
  if(d) printf("entering ll_insert_helper\n");  
  if( n->next == NULL)
    n->next = add;
  else if( n->next->val >= add->val){
    add->next = n->next;
    n->next = add;
  }
  else ll_insert_helper( n->next, add, d);
  if(d) printf("returning from ll_insert_helper\n");  
}
void ll_insert(Ll l, int val, int d){
  if(d) printf("entering ll_insert\n");
  Node* add = (Node*)malloc( sizeof( Node) );
  add->next = NULL;
  add->val = val;
  if( l->head == NULL) l->head = add;
  else if( l->head->val >= add->val){
    add->next = l->head;
    l->head = add;
  }
  else ll_insert_helper( l->head, add, d);
  if(d) printf("returing from ll_insert\n");
}

//delete a node in ll
void ll_delete_helper( Node* n, int val, int d){
  if(d) printf("entering ll_delete_helper\n");
  if( n->next == NULL){
    printf("Sorry, value doesn't exist\n");
  }
  else if( n->next->val == val){
    printf("Found it!\n");
    Node* temp = n->next;
    n->next = n->next->next; //could be another node or NULL
    free( temp);
  }
  else ll_delete_helper( n->next, val, d);
  if(d) printf("returning from ll_delete_helper\n");
}
void ll_delete(Ll l, int val, int d){
  if(d) printf("entering ll_delete\n");
  if( l->head == NULL) fprintf( stderr, "DELETE FROM EMPTY LIST\n");
  else if( l->head->val = val){
    Node* temp = l->head;
    l->head = temp->next;
    free( temp);
  }
  else ll_delete_helper( l->head, val, d);
  if(d) printf("returning from ll_delete\n");
}

//free the ll struct and contents
void ll_free_helper( Node* n, int d){
  if(d) printf("entering ll_free_helper\n");
  if( n != NULL){
    Node* temp = n;
    ll_free_helper( n->next, d);
    free( n);
  }
  if(d) printf("returning from ll_free_helper\n");
}
void ll_free( Ll l, int d){
  if(d) printf("starting ll_free\n");
  Node* temp = l->head;
  ll_free_helper( temp, d);
  free(l);
  if(d) printf("returning form ll_free\n");
}

//empty the ll struct's noded
void ll_empty( Ll l, int d){
  ll_free_helper( l->head, d);
  l->head = NULL;
}

//return if the given value is contained
int ll_contains_helper( Node* n, int val, int d){
  if(d) printf("entering ll_contains\n");
  if( n == NULL){
    if(d) printf("returning from ll_contains\n");
    return 0;
  }
  else if( n->val == val){
    if(d) printf("returning from ll_contains\n");
    return 1;
  }
  else ll_contains_helper( n->next, val, d);
  if(d) printf("returning from ll_contains\n");
}
int ll_contains( Ll l, int val, int d){
  return ll_contains_helper( l->head, val, d);
}

//list Ll's items
void ll_print_helper( Node* n, int d){
  if(d) printf("entering ll_print\n");
  if( n == NULL){
    if(d) printf("returning from ll_print\n");
    return;
  }
  else{
    printf( PRINT_FORMAT, n->val);
    ll_print_helper( n->next, d); 
  }
  if(d) printf("returning from ll_print\n");
}
void ll_print( Ll l, int d){
  ll_print_helper( l->head, d);  
  printf("\n");  
}

//print ll's items in reverse order
void ll_print_rev_helper( Node* n, int d){
  if(d) printf("entering ll_print_rev\n");
  if( n == NULL){
      if(d) printf("returning from ll_print_rev\n");
      return;
  }
  else{
    ll_print_rev_helper( n->next, d); 
    printf( PRINT_FORMAT, n->val);
  }
  if(d) printf("returning from ll_print_rev\n");
}
void ll_print_rev( Ll l, int d){
  ll_print_rev_helper( l->head, d);
  printf("\n");  
}

// parse cmd lnine args
void gen_parse_args( int argc, char **argv, int *d){
  int i;
  for( i=0; i < argc; ++i){
    if( 0 == strcmp( argv[i], "-d")) *d = 1;
  }
}

//print help screen
void gen_print_help(){
  printf("q: quit the program\n");
  printf("i <int>: insert value\n");
  printf("d <int>: delete value\n");
  printf("c <int>: is value contained?\n");
  printf("e: empty the list\n");
  printf("l: print the list\n");
  printf("r: print the list (reverse)\n");
  printf("?: help\n");
  printf("h: help\n");
}

// see if a digit exists in a c-string
int gen_exists_num( char* line){
  int i, len = strlen( line), flag = 0;
  for( i=0; i<len; ++i)
    if( isdigit(line[i]) ) flag = 1;
  return flag;
}

int main( int argc, char** argv){
  int d = 0, quit = 1, commandNum, numRead;
  char *input, command;
  Line l = line_init(stdin);
  Ll ll = ll_init();

  gen_parse_args( argc, argv, &d);

  while( quit){
    printf("Command: ");
    line_read_line( l);
    input = get_line( l);
    sscanf(input, " %c%n", &command, &numRead);
    switch( command){
      case 'q': quit = 0; break;
      case 'i': if( gen_exists_num( input+numRead) ){
                sscanf(input+numRead, "%d", &commandNum);
                ll_insert( ll, commandNum , d);
              }
              else printf("Sorry, you didn't enter a number!\n");
              break;
      case 'd': if( gen_exists_num( input+numRead) ){
                sscanf( input+numRead, "%d", &commandNum);
                ll_delete( ll, commandNum, d);
              }
              else printf("Sorry, you didn't enter a number!\n");
              break;
      case 'c': if( gen_exists_num( input+numRead) ){
                  sscanf( input+numRead, "%d", &commandNum);
                  if( ll_contains( ll, commandNum, d) )
                    printf("LIST DOES CONTAIN %d\n", commandNum);
                  else printf("LIST DOES NOT CONATAIN %d\n", commandNum);
                }
                else printf("Sorry, you didn't enter a number!\n");
                break;
      case 'e': ll_empty( ll, d); break;
      case 'l': ll_print( ll, d); break;
      case 'r': ll_print_rev( ll, d); break;
      case '?': gen_print_help(); break;
      case 'h': gen_print_help(); break;
      case '\n': break;
      default: printf("Invalid Command\n");
    }
    free( input);    
  }

  ll_free( ll, d);
  line_free( l);
  return 0;
}
