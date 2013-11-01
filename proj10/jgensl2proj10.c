#include <stdio.h>
#include <stdlib.h>
#include "line.h"

typedef struct node_struct{
  struct node_struct *next;
  int key;
  int val;
} Node;

typedef struct hash_table_struct{
  Node **table;
  int tableSize;
  int loadFactor;
  int (*hfunc)( struct hash_table_struct, int);
} *Htable;


//~~~general fxs
void gen_parse_args( int, char**, int*);
//~~~ll fxs
void node_append( Node**, int, int, int);
void node_delete( Node**, int, int, int);
void node_delete_row( Node**);
void node_print( Node*);
void node_print_row( Node**);
//~~~htable fxs
Htable ht_init( int (*hashfx)( Htable, int) );
void ht_free(Htable);
void ht_resize( Htable, int, int);
int  ht_exists( Htable, int, int);
void ht_add( Htable, int, int, int);
void ht_erase( Htable);
void ht_list( Htable);

int  ht_mod_hash( Htable, int);

//~~~~~~~~~~~~~~~~~MAIN~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char* argv[]){
  int debug = 0, flag = 1, arg, numRead = 0;
  char command, *stream;
  Line line = line_init( stdin);
  gen_parse_args( argc, argv, &debug);
  Htable htable = ht_init( ht_mod_hash);

  while(flag){
    arg = -1;
    fprintf( stdout, "\ncommand: ");
    line_read_line( line);
    stream = get_line( line);
    sscanf( stream, " %c%n", &command, &numRead);
    switch( command){
      case 'q': flag = 0; break;
      case 'i': sscanf( stream+numRead, "%d", &arg);
                if( arg < 0) fprintf( stderr, "MUST BE >= 0\n");
                else ht_add( htable, arg, arg, debug);
                break;
      case 'd': sscanf( stream+numRead, "%d", &arg);
                if( arg < 0) fprintf( stderr, "MUST BE >= 0\n");
                else{
                  int pos = ht_mod_hash( htable, arg);
                  node_delete( &((htable->table)[pos]), arg, arg, debug); 
                }
                break;
      case 'c': sscanf( stream+numRead, "%d", &arg);
                if( arg < 0) fprintf( stderr, "MUST BE >= 0\n");
                else{
                  if( ht_exists( htable, arg, debug) )
                    fprintf( stdout, "The value, %d, exists!\n", arg);
                  else
                    fprintf( stdout, "The value, %d, does NOT exist\n", arg);
                }
                break;
      case 'e': ht_erase( htable); break;
      case 'r': sscanf( stream+numRead, "%d", &arg);
                if( arg >= 1) ht_resize( htable, arg, debug);
                else fprintf( stderr, "Error, size must be >= 1\n");
                break;
      case 'l': ht_list( htable); break;
      case '\n': break;
      default: fprintf( stdout, "Sorry, Invalid command\n"); break;
    }
    free(stream);
  }
  line_free( line);
  ht_free( htable);
  return 0;
}

//~~~general fxs
void gen_parse_args( int argc, char** argv, int* debugMode){
  int i;
  FILE *temp;
  char debug[] = "-d";
  for( i = 1; i < argc; i++){
    if( 0 == strcmp(argv[i], debug) )
    *debugMode = 1;
  }
}

//~~~ll fxs

//append node to end of chain of nodes
//set val to val 
void node_append( Node** head, int key, int val, int d){
  Node* temp = *head;
  Node* newest = malloc( sizeof( Node) );
  newest->key = key;
  newest->val = val;
  newest->next = NULL;
  if( temp == NULL){ //list is empty
    *head = newest;
  }
  else{
    while( temp->next != NULL){
      if(d) node_print( temp); 
      temp = temp->next;
    }
    //temp points to the last node;
    temp->next = newest;
  }
}

//delete a node in a chaing of nodes
//if a node's val == passed val, delete it and fix the list
void node_delete( Node** head, int key, int val, int d){
  Node* temp = *head;
  if( temp == NULL){
    fprintf( stderr, "ERROR, DELETE FROM EMPTY LIST)");
  }
  else if( temp->val == val){
    if(d) node_print( temp);
    *head = temp->next;
    free( temp);
  }
  else{
    while( temp->next != NULL && temp->next->val != val){
      if(d) node_print( temp);
      temp = temp->next;
    }
    if( temp->next != NULL && temp->next->val == val){
      Node* holder = temp->next;
      temp->next = temp->next->next;
      free( holder);
    }
  }
}

//delete the list of nodes under the passed head;
void node_delete_row( Node** head){
  Node* temp = *head;
  Node* holder;
  if( temp != NULL){
    while( temp != NULL){
      holder = temp;
      temp = temp->next;
      free( holder);
    }
    *head = NULL;
  }
}

void node_print( Node* temp){
  fprintf( stdout, " (%d,%d) ", temp->key, temp->val);
}

//print the key,val pairs of a given row
void node_print_row( Node** head){
  Node* temp = *head;
  while( temp != NULL){
    node_print( temp);
    temp = temp->next;
  }
  fprintf(stdout, "\n");
}

//~~~htable fxs
Htable ht_init( int (*hash_fx)(Htable, int) ){
  Htable htable = malloc( sizeof( struct hash_table_struct) );
  htable->table = (Node**)malloc( 1*sizeof( Node*) );
  (htable->table)[0] = NULL;
  htable->tableSize = 1;
  htable->loadFactor = -999;
  //htable->hfunc = hash_fx; //TODO incompatible pointer type
  return htable;
}

//deallocate the htable's table and the table's key+val
void ht_free( Htable htable){
  ht_erase( htable);
  free( htable->table);
  free( htable);
}

//resize and copy old contets to new htable->table
void ht_resize( Htable htable, int new_size, int d){
  Node** new_table = (Node**)malloc( new_size*sizeof(Node*) );
  int i;
  for(i = 0; i < new_size; ++i){
    new_table[i] = NULL;
  }
  Node** old_table = htable->table;
  int old_size = htable->tableSize;
  htable->table = new_table;
  htable->tableSize = new_size;

  for( i = 0; i < old_size; ++i){
    Node* temp = old_table[i];
    while( temp != NULL){
      ht_add( htable, temp->key, temp->val, d);
      temp = temp->next;
    }
    node_delete_row( &(old_table[i]) ); //delete "row"
  }
  free( old_table); //delete pointer to "row"
}

// return 1 if key exists, 0 if not;
int ht_exists( Htable htable, int key, int d){
  int val_expec = ht_mod_hash( htable, key);
  int flag = 0;
  Node* temp = (htable->table)[val_expec];
  while( temp != NULL && !flag){
    if(d) node_print( temp);
    if(temp->key == key)
      flag = 1;
    temp = temp->next;
  }
  return flag == 0 ? 0 : 1;
}

// add a key value pait to the htable
void ht_add( Htable htable, int key, int val, int d){
  if( !ht_exists( htable, key, d) ){
    //fprintf( stdout, "ADDING A LEGAL VALUE\n");
    node_append( &(htable->table)[ ht_mod_hash( htable, key)], key, val, d);
  }
  else{
    fprintf( stderr, "ADDING A DUPLICATE VALUE\n");
    node_append( &(htable->table)[ ht_mod_hash( htable, key)], key, val, d);
  }
}

// delete all linked list nodes
void ht_erase( Htable htable){
  int i;
  for( i=0; i < htable->tableSize; ++i){
    node_delete_row( &( (htable->table)[i] ) );
  }
}

//print out the htable
void ht_list( Htable htable){
  fprintf( stdout, "Table Size: %d\n", htable->tableSize);
  fprintf( stdout, "Load Factor: %d\n", htable->loadFactor);
  int i;
  for( i=0; i<htable->tableSize; ++i){
    fprintf( stdout, "%d: ", i);
    node_print_row( &(  (htable->table)[i]  ) );
  }
}

//the hash function used to disribute the keys
int ht_mod_hash( Htable htable, int key){
  return key % htable->tableSize;
}


