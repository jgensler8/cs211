#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_TABLE_SIZE 10

typedef struct node_struct{
  struct node_struct *next;
  char *key;
  void *val;
} Node;

typedef struct hash_table_struct{
  Node **table;
  int tableSize;
  int numElems;
  int loadFactor;
  int (*hfunc)( struct hash_table_struct, int);
} *Htable;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ll fxs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* param: Node&* head of a row
 * param: int, key
 * param: int, val
 */
void node_append( Node** head, char* key, void* val){
  Node* temp = *head;
  Node* newest = malloc( sizeof( Node) );
  char* word = (char*)malloc( (1+strlen(key))*sizeof(char));
  strcpy( word, key);
  newest->key = word;
  newest->val = val;
  newest->next = NULL;
  if( temp == NULL) //list is empty
    *head = newest;
  else{
    while( temp->next != NULL)
      temp = temp->next;
    //temp points to the last node;
    temp->next = newest;
  }
}

/* param: Node&
 * func:  free the Node and contents
 */
void node_free( Node* node){
  free( node->key);
  free( node);
}

/* param: Node**
 * param: int
 * param: int
 * func:  delete a particular node from a linked list
 */
void node_delete( Node** head, char* key, void* val){
  Node* temp = *head;
  if( temp == NULL){
    fprintf( stderr, "ERROR, DELETE FROM EMPTY LIST\n");
  }
  else if( 0 == strcmp(temp->key,key) ){ //1st node
    *head = temp->next;
    node_free( temp);
  }
  else{ //2nd or onward
    while( temp->next != NULL && 0 != strcmp(temp->next->val, val) )
      temp = temp->next;
    if( temp->next != NULL && 0 == strcmp(temp->next->val, val) ){
      Node* holder = temp->next;
      temp->next = temp->next->next;
      free( holder);
    }
    else fprintf( stderr, "ERROR, NODE NOT FOUND\n");
    // else we didn't find it
  }
}

/* param: Node&*
 * func:  delete a "row" of a linked list
 */
void node_delete_row( Node** head){
  Node* temp = *head;
  Node* holder;
  if( temp != NULL){
    while( temp != NULL){
      holder = temp;
      temp = temp->next;
      node_free( holder);
    }
    *head = NULL;
  }
}

/* param: Node*
 * func:  print the node to stdout
 */
void node_print( Node* temp, void (*userPrint)(void*) ){
  fprintf( stdout, "KEY:%s:", temp->key);
  userPrint( temp->val);
}

/* param: Node**
 * func:  print a list of nodes to stdout
 */
void node_print_row( Node** head, void (*userPrint)(void*) ){
  Node* temp = *head;
  while( temp != NULL){
    node_print( temp, userPrint);
    temp = temp->next;
  }
  fprintf(stdout, "\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~htable fxs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* param: int*(*fx)(Htable, int)
 * func:  initialize the Htable and set its hash fx to the param
 * ret:   the initialized Htable
 */
Htable ht_init( int (*hash_fx)(Htable, char) ){
  Htable htable = malloc( sizeof( struct hash_table_struct) );
  htable->table = (Node**)malloc( INIT_TABLE_SIZE*sizeof( Node*) );
  int i;
  for( i=0; i < INIT_TABLE_SIZE; ++i)
    (htable->table)[i] = NULL;
  htable->tableSize = INIT_TABLE_SIZE;
  htable->numElems = 0;
  htable->loadFactor = 75;
  //htable->hfunc = hash_fx; //TODO incompatible pointer type
  return htable;
}

/* param: Htable
 * func:  erase the key value pairs of the Htable
 */
void ht_erase( Htable htable){
  int i;
  for( i=0; i < htable->tableSize; ++i){
    node_delete_row( &( (htable->table)[i] ) );
  }
  htable->numElems = 0;
}

/* param: Htable
 * func:  free the Htable's contents
 */
void ht_free( Htable htable){
  ht_erase( htable);
  free( htable->table);
  free( htable);
}

/* param: Htable
 * param: int, key
 * param: int, value
 * func:  add the given key value pair to the Htable
 *        will make a deep copy of the passed key
 */
void ht_add_helper( Htable htable, char* key, void* val){
  if( !ht_exists( htable, key) ){
    //fprintf( stdout, "ADDING A LEGAL VALUE\n");
    node_append( &(htable->table)[ ht_mod_hash( htable, key)], key, val);
  }
  else{
    fprintf( stderr, "ADDING A DUPLICATE VALUE\n");
    node_append( &(htable->table)[ ht_mod_hash( htable, key)], key, val);
  }
}

/* param: Htable
 * param: int
 * func:  resize the Htable's table so to hold more elements
 */
void ht_resize( Htable htable, int new_size){
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
      ht_add_helper( htable, temp->key, temp->val);
      temp = temp->next;
    }
    node_delete_row( &(old_table[i]) ); //delete "row"
  }
  free( old_table); //delete pointer to "row"
}

/* param: Htable
 * param: int, key
 * param: int, value
 * func:  check if the table needs to be grown
 *        add it to the table (ht_add_helper)
 */
void ht_add( Htable htable, char* key, void* val){
  ++htable->numElems;
  //printf("oldsize+1:%d:loadfactor*size:%d:\n", htable->numElems*100,
  //  (htable->loadFactor)*(htable->tableSize) );
  if( htable->numElems*100 > htable->loadFactor * htable->tableSize )
    ht_resize( htable, 2*htable->tableSize );
  ht_add_helper( htable, key, val);
}

/* param: Htable
 * param: int
 * func:  see if the key exists in the Htable
 * ret:
 *    0: the key doesn't exist
 *    1: the key exists
 */
int ht_exists( Htable htable, char* key){
  int val_expec = ht_mod_hash( htable, key);
  int flag = 0;
  Node* temp = (htable->table)[val_expec];
  while( temp != NULL){
    if( 0 == strcmp(temp->key,key) ){
      flag = 1;
      break;
    }
    temp = temp->next;
  }
  return flag == 0 ? 0 : 1;
}

/* param: Htable
 * func:  print the contents of the Htable
 */
void ht_list( Htable htable, void (*userPrint)(void*) ){
  fprintf( stdout, "Table Size: %d\n", htable->tableSize);
  fprintf( stdout, "Load Factor: %d\n", htable->loadFactor);
  int i;
  for( i=0; i<htable->tableSize; ++i){
    fprintf( stdout, "%d: ", i);
    node_print_row( &(  (htable->table)[i]  ), userPrint );
  }
}

//the hash function used to disribute the keys
int ht_mod_hash( Htable htable, char* name){
  int i, key = 0;
  for( i=0; i<strlen(name); ++i)
    key = key*2 + (int)(name[i]);
  return key % htable->tableSize;
}
