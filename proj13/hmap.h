#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hash_table_struct *Htable;

//~~~~~~~~~~~~~~~~~~~~~~~~~~htable fxs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* param: int*(*fx)(Htable, int)
 * func:  initialize the Htable and set its hash fx to the param
 * ret:   the initialized Htable
 */
extern Htable ht_init( int (*hash_fx)(Htable, char) );

/* param: Htable
 * func:  erase the key value pairs of the Htable
 */
extern void ht_erase( Htable htable);

/* param: Htable
 * func:  free the Htable's contents
 */
extern void ht_free( Htable htable);

/* param: Htable
 * param: int, key
 * param: int, value
 * func:  add the given key value pair to the Htable
 *        will make a deep copy of the passed key
 */
extern void ht_add( Htable htable, char* key, void* val);

/* param: Htable
 * param: int
 * func:  resize the Htable's table so to hold more elements
 */
extern void ht_resize( Htable htable, int new_size);

/* param: Htable
 * param: int
 * func:  see if the key exists in the Htable
 * ret:
 *    0: the key doesn't exist
 *    1: the key exists
 */
extern int ht_exists( Htable htable, char* key);

/* param: Htable
 * func:  print the contents of the Htable
 */
extern void ht_list( Htable htable);

//the hash function used to disribute the keys
extern int ht_mod_hash( Htable htable, char* key);


