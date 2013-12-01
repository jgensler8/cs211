#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph_struct *Graph;

Graph graph_init( int (*cmp)(void*,void*), int (*val_to_index)(void*), void (*print)(void*) );
void graph_free( Graph);
int graph_edge_exists( Graph, void*, void*);
void graph_add_edge( Graph, void*, void*);
void graph_erase( Graph);
void graph_resize( Graph, int);
void graph_list( Graph );
int graph_dfs_target( Graph, void*, void*);
#endif
