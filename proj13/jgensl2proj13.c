#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hmap.h"
#define HISTORY_LENGTH 4

// default to heads;
typedef struct result_struct{
  int h;
  int t;
} Result;

// initialize the struct
void result_init( Result *result){
  result->h = 0;
  result->t = 0;
}

// print the head
void result_print( void* toPrint){
  Result* r = (Result*)toPrint;
  printf("VAL:h%d:t%d; ", r->h, r->t);
}

//set the string to all 'h'
void string_init( char** str){
  int i;
  for( i=0 ; i<HISTORY_LENGTH; ++i)
    (*str)[i] = 'h';
  (*str)[HISTORY_LENGTH] = '\0';
}
//highest power of two that fits into target
int gen_get_highest_pow( int target){
  if( target <= 0) return -1;
  int highest = 0;
  while( pow(2,highest) <= target) ++highest;
  return highest != 0 ? highest-1 : 0;
}
//build the structure to store the moves in
void gen_build_table( Htable htable, Result** results){
  int permutations = pow(2,HISTORY_LENGTH), i, temp, highest;
  *results = (Result*)malloc( permutations*sizeof( Result) );
  char* headsTails = (char*)malloc( (1+HISTORY_LENGTH)*sizeof(char));
  for( i=0; i<permutations; ++i){
    result_init( & ((*results)[i]) );
    string_init( &headsTails);
    temp = i;
    while( temp > 0){
      highest = gen_get_highest_pow( temp);
      if(highest == -1) break;
      headsTails[ highest] = 't';
      temp -= pow(2,highest);
    }
    ht_add( htable, headsTails, & ((*results)[i]) );
  }
  free( headsTails);

}

// move the history "down one" and add the new char
void gen_add_char( char **history, char toAdd){
  int i;
  for( i=1; i<HISTORY_LENGTH; ++i)
    (*history)[i-1] = (*history)[i];
  (*history)[HISTORY_LENGTH-1] = toAdd;
}

//serach table and guess result
char get_prediction( Htable htable, char* history){
  // Result* r = (Result*)htable_get( htable, history);
  // if( r->h < r->t) return 't' //TODO
  return 'h';
}

int main(){
  Htable htable = ht_init( NULL);
  Result* results;
  gen_build_table( htable, &results);

  //pretty cool when you change the history size
  //ht_list( htable, result_print);
  char get, *history = (char*)malloc((1+HISTORY_LENGTH)*sizeof(char));
  char prediction;
  int userScore = 0, compScore = 0;
  string_init( &history);
  while( get != EOF){
    fprintf(stdout, "Enter a guess: ");
    get = (char)fgetc( stdin);
    if( get == 'h' || get == 't'){
      gen_add_char( &history, get);
      prediction = get_prediction( htable, history);
      if( prediction == get){
        fprintf(stdout, "COMPUTER GUESSED RIGHT!\n");
        ++compScore;
      }
      else{
        fprintf(stdout, "COMPUTER GUESSED WRONG!\n");
        ++userScore;
      }
      //log prediction TODO
      fprintf(stdout, "SCORE: Hu %d | Co %d\n", userScore, compScore);
    }
    get = (char)fgetc( stdin);
  }

  free( history);
  free( results);
  ht_free( htable);
  return 0;
}
