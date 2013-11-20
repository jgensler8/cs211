#include <stdio.h>
#include <stdlib.h>

#include "hmap.h"

int main(){
  Htable htable = ht_init( NULL);
  
  char* name = (char*)malloc( 100*sizeof(char));
  strcpy( name, "jeffrey\0");
  ht_add( htable, name, NULL);
  ht_add( htable, name, NULL);
  ht_erase( htable);
  ht_add( htable, name, NULL);
  ht_add( htable, name, NULL);
  printf(":%d:", ht_exists( htable, name) );
  printf(":%d:", ht_exists( htable, "nothere") );

  ht_list( htable);

  free( name);
  ht_free( htable);
  return 0;
}
