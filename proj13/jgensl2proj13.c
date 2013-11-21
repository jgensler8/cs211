#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hmap.h"
#include "line.h"
#define HISTORY_LENGTH 4
#define SCORE_LIMIT 25

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
    while( temp > 0){ //build a "hthth...t" out of powers of two
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
  printf("inside:%s:\n", history);
  Result* r = (Result*)ht_get( htable, history);
  printf("h:%d t:%d \n", r->h, r->t);
  if( r->h < r->t) return 't';
  return 'h';
}

// add the heuristic for the AI to use next time
log_prediction( htable, history, get){
  Result* r = (Result*)ht_get( htable, history);
  if( get == 'h') ++r->h;
  else if( get == 't') ++r->t;
}

int main(){
  Htable htable = ht_init( NULL);
  Result* results;
  gen_build_table( htable, &results);
  char *history = (char*)malloc((1+HISTORY_LENGTH)*sizeof(char));
  char prediction, *chunk;
  int userScore = 0, compScore = 0, gameTimer = 1;
  Line line = line_init( stdin);
  string_init( &history);

  while( userScore < SCORE_LIMIT && compScore < SCORE_LIMIT){
    //ht_list( htable, result_print );
    //printf("HISTORY:%s:\n", history);
    fprintf(stdout, "TIMER: %d\n", gameTimer);
    fprintf(stdout, "Enter a guess: ");
    line_read_line( line);
    chunk = get_line( line);
    switch( chunk[0]){
      case 'h':
      case 't':
        prediction = get_prediction( htable, history);
        if( prediction == chunk[0]){
          fprintf(stdout, "COMPUTER GUESSED RIGHT!\n");
          ++compScore;
        }
        else{
          fprintf(stdout, "COMPUTER GUESSED WRONG!\n");
          ++userScore;
        }
        log_prediction( htable, history, chunk[0]);
        char userChar = chunk[0];
        gen_add_char( &history, userChar);
        fprintf(stdout, "SCORE: Hu %d | Co %d\n", userScore, compScore);
        ++gameTimer;
        break;
      case '\n': break;
      default: printf("INVALID OPTION\n");
    }
    free( chunk);
  }

  free( history);
  free( results);
  line_free (line);
  ht_free( htable);
  return 0;
}
