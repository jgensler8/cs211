#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

#define GRAPH_INIT_SIZE 10

typedef struct node_struct{
  struct node_struct *next;
  void *val;
} Node;

struct graph_struct{
  Node** adjList; //adjacency list
  int maxSize; //max size of adjList
  int (*cmp)(void*, void*); // how to compare the values stored
  int (*hash)(void* ); //tells the graph what index to look to for given void*
  void (*print)(void* ); // how to print the values stored
};

/* param: int *cmp, how to compare the values stored
 * param: void *val_to_index, tells the graph what index to look to for a val
 * param: void *print, how to print the values stored
 * func:  initialize a Graph struct
 * ret:   Graph struct
 */
Graph graph_init( int (*cmp)(void*,void*), int (*val_to_index)(void*)
    , void (*print)(void*) ){
  Graph g = malloc( sizeof( struct graph_struct) );
  g->adjList = (Node**)malloc( GRAPH_INIT_SIZE*sizeof(Node*) );
  g->maxSize = GRAPH_INIT_SIZE;
  int i;
  for( i=0; i<GRAPH_INIT_SIZE; ++i)
    (g->adjList)[i] = NULL;
  g->cmp = cmp;
  g->hash = val_to_index;
  g->print = print;
  return g;
}

/* param: Graph
 * param: int, new adjList size
 * func:  grow graphs adjList capacity to size int
 */
void graph_grow( Graph g, int n){
  if( n < g->maxSize ){
    fprintf( stderr, "ERROR, CANT REDUCE SIZE TO SIZE %d\n", n);
    return;
  }
  Node** newList = (Node**)malloc( n*g->maxSize*sizeof(Node*));
  int i;
  for( i=0; i<g->maxSize; ++i) //copy old contents
    newList[i] = (g->adjList)[i];
  for( i=g->maxSize; i<n*g->maxSize; ++i) //make new contents NULL
    newList[i] = NULL;
  free(g->adjList);
  g->adjList = newList;
  g->maxSize = n;
}

/* param: Graph
 * func:  free the "dangling" list of the adjList of the Graph
 */
void graph_erase( Graph g){
  int i;
  Node *temp, *holder;
  for( i=0; i<g->maxSize; ++i){
    temp = (g->adjList)[i];
    while( temp != NULL){
      holder = temp;
      temp = temp->next;
      free( holder);
    }
    (g->adjList)[i] = NULL;
  }
}

/* param: Graph
 * func:  free the dangling list, adjList, and graph struct
 */
void graph_free( Graph g){
  graph_erase( g);
  free( g->adjList);
  free( g);  
}

/* param: g
 * ret:   get the max 
 */
int graph_get_max_size( Graph g){
  return g->maxSize;
}

/* param: Graph
 * param: void*, from node
 * param: void*, check the dangling list's node's val for this
 * ret:
 *    -1: error lookup
 *    0:  does not exist
 *    1:  target exists
 *
 */
int graph_edge_exists( Graph g, void* from, void* to){
  int toPosition = g->hash( to);
  int fromPosition = g->hash( from);
  if( fromPosition < 0 || fromPosition >= g->maxSize
      || toPosition < 0 || toPosition >= g->maxSize) return -1;
  Node* temp = (g->adjList)[fromPosition];
  while( temp != NULL){
    if( 0 == g->cmp( to, temp->val) ) return 1;
    temp = temp->next;
  }
  return 0;
}

/* param: Graph
 * param: void*, from value
 * param: void*, to value
 * func: either the "from" or "to" could result in:
 *        1) a position < 0, bad 
 *        2) the need to grow the adjList, good
 *        3) nothing, both are valid, good
 */
void graph_add_edge( Graph g, void* from, void* to){
  int toPosition = g->hash( to);
  int fromPosition = g->hash( from);
  int largest = toPosition > fromPosition ? toPosition : fromPosition;
  int smallest = toPosition < fromPosition ? toPosition : fromPosition;
  if( smallest < 0){
    fprintf( stderr, "ERROR, EDGE POSITION BELOW ZERO (%d)\n", smallest);
    return;
  }  
  if( largest > g->maxSize) graph_grow( g, largest+1);  
  if( graph_edge_exists( g, from, to) ){
    fprintf( stderr, "ATTEMPTED DUPLICATE VALUE, NOT ADDING\n");
    return;
  }
  Node* addition = (Node*)malloc( sizeof(Node));
  addition->val = to;
  addition->next = (g->adjList)[fromPosition]; // could be NULL
  (g->adjList)[fromPosition] = addition; //place new node at from of list
}

/* param: Graph
 * param: void*, from
 * param: void*, to
 * func:  if an edge from 'from' to 'to' exists, delete it
 */
void graph_del_edge( Graph g, void* from, void* to){
  int toPosition = g->hash( to);
  int fromPosition = g->hash( from);
  int largest = toPosition > fromPosition ? toPosition : fromPosition;
  int smallest = toPosition < fromPosition ? toPosition : fromPosition;
  if( smallest < 0){
    fprintf( stderr, "ERROR, EDGE POSITION BELOW ZERO (%d)\n", smallest);
    return;
  }  
  if( largest > g->maxSize){
    fprintf( stderr, "ERROR, EDGE POSITION ABOVE MAX (%d)\n", largest);
    return;
  }
  Node* temp = (g->adjList)[fromPosition];
  if( temp == NULL){
    fprintf( stderr, "ERROR, CANT DELETE FROM EMPTY LIST\n");
    return;
  }
  else if( temp->next == NULL){
    if( g->cmp( temp->val, to) == 0){
      fprintf( stdout, "DELETING EDGE\n");
      free( temp);
      (g->adjList)[fromPosition] = NULL;
    }
    else fprintf( stdout, "SORRY, NODE DOESNT EXIST\n");
    return;
  }
  else{
    while( temp->next != NULL){
      if( 0 == g->cmp( temp->next->val , to) ){
        fprintf( stdout, "DELETING EDGE\n");
        Node* holder = temp->next;
        temp->next = temp->next->next;
        free( holder);
        return;
      }
      temp = temp->next;
    }
    fprintf( stdout, "SORRY, NODE DOESNT EXIST\n");
  }
}

/* param: Graph
 * param: int, new future size of the graph
 *        !!! this options frees all data from the structure
 */
void graph_resize( Graph g, int n){
  int i;
  graph_erase( g);
  free( g->adjList);
  Node** newList = (Node**)malloc( n*sizeof(Node*));
  for( i=0; i<n; ++i)
    newList[i] = NULL;
  g->maxSize = n;
  g->adjList = newList;
}

/* param: Graph
 * func:  print the adjList of the Graph
 */
void graph_list( Graph g){
  int i;
  Node* temp;
  for( i=0; i<g->maxSize; ++i){
    temp = (g->adjList)[i];
    fprintf( stdout, "%d:", i);
    while( temp != NULL){
      g->print( temp->val);
      temp = temp->next;
    }
    fprintf( stdout, "\n");
  }
}

/* param: Graph
 * param: void*, from (node val)
 * param: int&*, the status of each node
 * func: launch a dfs from void*
 *    any parent of a cycle will receive a -2 flag code
 *    flag codes:
 *    -2: discovered, completed, has cycle
 *    -1: discovered, expanding, has cycle
 *    0: never discovered
 *    1: discovered, expanding
 *    2: discovered, completed
 * ret:
 *    0: cycles, bad graph
 *    1: no cycles
 */
int graph_dfs_helper( Graph g, void* from, int** flags){
  int fromPosition = g->hash( from);
  if( (*flags)[ fromPosition] == -2){
    return 0; //same reason as flag code 2
  }
  else if( (*flags)[ fromPosition] == -1){
    return 0; //don't expand because its being expanded
  }
  else if( (*flags)[ fromPosition] == 0){
    (*flags)[ fromPosition] = 1;
    Node* toValNode = (g->adjList)[ fromPosition];
    while( toValNode != NULL){ //for every neighbor, launch DFS
      if( 0 == graph_dfs_helper( g, toValNode->val, flags))
        (*flags)[ fromPosition] = -1; //a child of mine has a cycle
      toValNode = toValNode->next;
    }
    if( (*flags)[ fromPosition] == 1){ //if i still don't have cycles...
      (*flags)[ fromPosition] = 2;
      return 1;
    }
    else{
      (*flags)[ fromPosition] = -2; // else i have cycles, dont expand later
      return 0;
    }
  }
  else if( (*flags)[ fromPosition] == 1){
    fprintf( stderr, "CYCLE DETECTED\n");
    (*flags)[ fromPosition] = -1; // mark ancestor as commiting a cycle
    return 0; 
  }
  else if( (*flags)[fromPosition] == 2) return 1;
}
/* param: Graph
 * param: void*, launch dfs from here
 * func:  launch dfs from the void*
 *        !!XXX!! DOES NOT WORK WITH SELF LOOPS !!XXX!!
 * ret:   an array containing  
 */
int* graph_dfs( Graph g, void* from){
  int* flags = (int*)malloc( g->maxSize*sizeof( int));
  int i;
  for( i=0; i<g->maxSize; ++i)  
    flags[i] = 0;
  graph_dfs_helper( g, from, &flags);
  for( i=0; i<g->maxSize; ++i)  
    printf("%d: %d\n", i, flags[i]);
  return flags;
}

/* param: Graph
 * param: void*, from
 * param: void*, to
 * func:  evaluate if there is a viable path from 'from' to 'to'
 * ret:
 *    -1: error in path criteria
 *    0:  no path found
 *    1:  path found
 *    2:  error, cycles in path but path found
 */
int graph_dfs_target( Graph g, void* from, void* to){
  int toPosition = g->hash( to);
  int fromPosition = g->hash( from);
  if( fromPosition < 0 || fromPosition >= g->maxSize
      || toPosition < 0 || toPosition >= g->maxSize) return -1;
  int* results = graph_dfs( g, from);
  if( results[toPosition] == 0){
    free(results); 
    return 0;
  }
  else if( results[toPosition] == 2){
    free(results); 
    return 1;
  }
  else if( results[toPosition] == -2){
    free(results); 
    return 2;
  }
}
