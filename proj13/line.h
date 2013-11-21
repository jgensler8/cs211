#include <stdio.h>
typedef struct line_struct *Line;

extern Line line_init(FILE*);
extern void line_free( Line);
extern void line_grow( Line);
extern int line_read_line( Line);
extern void set_cur_size( Line);
extern void line_print_line( Line );
extern char* get_line( Line);
