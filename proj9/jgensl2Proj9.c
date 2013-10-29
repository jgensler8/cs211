#include <stdio.h>
#include <stdlib.h>

void gen_parse_args( int, char**, int*);

int factorial( int n, int);
int fibonacci( int n, int);
int isPrime( int n, int);
int GCD( int m, int n, int);
int expo( int m, int n, int);

int main( int arc, char* argv[] ){
  int debug = 0, numa, numb;
  gen_parse_args( arc, argv, &debug);

  //factorial a >= 0
  printf("factorial: enter num:");
  scanf("%d", &numa);
  if( numa >= 0) printf("return: %d\n\n", factorial( numa, debug) );
  else printf("INVALID NUMBER\n");

  //fibonacci a >= 0
  printf("fibonacci: enter num:");
  scanf("%d", &numa);
  if(numa >= 0) printf("return: %d\n\n", fibonacci( numa, debug) );
  else printf("INVALID NUMBER\n");

  //isprime none
  printf("is prime: enter num:");
  scanf("%d", &numa);
  printf("return: %d\n\n", isPrime( numa, debug) );

  //GCD a >= 0
  printf("gcd: enter num 1:");
  scanf("%d", &numa);
  printf("gcd: enter num 2:");
  scanf("%d", &numb);
  if( numa >= 0 && numb >= 0)
    printf("return: %d\n\n", GCD( numa, numb, debug) );
  else printf("INVALID NUMBER(s)\n") ;

  //exponentiation a anything b >=0
  printf("exponentiation: enter num 1:");
  scanf("%d", &numa);
  printf("exponentiation: enter num 2:");
  scanf("%d", &numb);
  if( numb >= 0) printf("return: %d\n\n", expo( numa, numb, debug) );
  else printf("INVLID NUMER");

  return 0;
}

void gen_parse_args( int argc, char **argv, int *d){
  int i;
  for( i=0; i < argc; ++i){
    if( 0 == strcmp( argv[i], "-d")) *d = 1;
  }
}

/* param: int, the base number, assumed >= 0
 * param: int, debug flag
 * func: simple factorial problem
 * return: int, the product of all integers <= n
 */
int factorial( int n, int d){
  if(d) printf("FACTORIAL: n:%d:\n", n);
  if( n == 0){
    if(d) printf("FACTORIAL: RETURN: %d\n", 1);
    return 1;
  }
  else{
    if(d) printf("FACTORIAL: RETURN: %d*factorial( %d, %d)\n",n,n-1,d);
    return n*factorial( n-1, d);
  }
}

/* param: int, the nth fibonacci number
 * param: int, debug flag
 * func: calculate the nth fibonacci number
 * return: the nth fibonacci number
 */
int fibonacci( int n, int d){\
  if(d) printf("FIBONACCI: n:%d:\n", n);
  if( n == 0){
    if(d) printf("FIBONACCI: RETURN: %d\n", 0);
    return 0;
  }
  else if( n == 1){
    if(d) printf("FIBONACCI: RETURN: %d\n", 1);
    return 1;
  }
  else{
    if(d) printf("FIBONACCI: RETURN: FIB(%d, %d) + FIB( %d, %d)\n",n-1,d,n-2,d);
    return fibonacci( n-1, d) + fibonacci( n-2, d);
  }
}

/* param: int, to decide if its prime
 * param: int, debug flag
 * func:
 * return:
 *  0: param passed is not prime
 *  1: param passed is prime
 */
int isPrime( int n, int d){
  if(d) printf("isPrime: n:%d:\n", n);
  if( n < 2){
    if(d) printf("isPrime: RETURN: %d\n", 0);
    return 0;
  }
  else if ( 0 == isPrime_factorInRange( 2, n, d) ){
    printf("isPrime: RETURN: %d\n", 1);
    return 1; //no factor in range
  }
  else{
    printf("isPrime: RETURN: %d\n", 0);
    return 0;
  }
}

/* param: int, 
 * param, int debug flag
 * func: see if any number divides into the passed number
 * return:
 *  0: no common divisors (IS prime)
 *  1: arbitrary common divisor (Is NOT prime)
 */
int isPrime_factorInRange( int m, int n, int d){
  if(d) printf("isPrime_factorInRange: m:%d:n:%d\n", m, n);
  if( m >= n){
    if(d) printf("isPrime_factorInRange: RETURN: %d\n", 0);
    return 0;
  }
  else if( n % m == 0){ 
    if(d) printf("isPrime_factorInRange: RETURN: %d\n", 1);
    return 1;\
  }
  else if( m == 2){
    if(d) printf("isPrime_factorInRange: RETURN: IP_FIR(%d,%d,%d)\n",m+1,n,d);
    return isPrime_factorInRange( m+1, n, d);
  }
  else if( m != 2){
    if(d) printf("isPrime_factorInRange: RETURN: IP_FIR(%d,%d,%d)\n",m+2,n,d);
    return isPrime_factorInRange( m+2, n, d);
  }
}

/* param: int,
 * param: int,
 * param: int, debug flag
 * func: find which one has a
 * return: the greatest common denominator
 */
int GCD( int m, int n, int d){
  if(d) printf("GCD: m:%d:n:%d\n", m, n);
  if( n == m){
    if(d) printf("GCD: RETURN: %d\n", m);
    return m;
  }
  else if( m > n){
    if(d) printf("GCD: RETURN: GCD(%d,%d,%d)\n",m-n,n,d);
    return GCD( m-n, n, d);
  }
  else if( m < n){
    if(d) printf("GCD: RETURN: GCD(%d,%d,%d)\n",m-n,n,d);
    return GCD( m, n-m, d);
  }
  
}

/* param: int, the base
 * param: int, the power of the base
 * param: int, debug flag
 * func: raise the first param to the second param
 * return: first param to the second param
 */
int expo( int m, int n, int d){
  if(d) printf("EXP: m:%d:n:%d:\n", m, n);

  if( n == 0){
    if(d) printf("EXP: RETURN: %d\n", 1); 
    return 1;
  }
  else if( n == 1){
    if(d) printf("EXP: RETURN: %d\n", m); 
    return m;
  }
  else if( n%2 == 0){
    int half = expo( m, n/2, d);
    if(d) printf("EXP: RETURN: EXPO(%d,%d,%d)*EXPO(\",\",\")\n", m,n/2,d);
    if(d) printf("EXP:\t\t(%d*%d)\n", half, half);
    return half*half;
  }
  else{
    if(d) printf("EXP: RETURN: %d*EXPO(%d,%d,%d)\n",m,m,n-1,d);
    return m*expo( m, n-1, d);
  }
}

