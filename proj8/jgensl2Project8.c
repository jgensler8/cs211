/*  Code for the user interface for Lab 8 for CS 211 Fall 2013  
 *
 *  Author: Pat Troy
 *  Date: 10/13/2013
 */

/* Project 8
 * EDITED BY: Jeff Gensler
 * Date:      10/15/2013
 * Email:     jgensl2@uic.edu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BEGIN FX ADDITIONS

typedef struct stack_node{
  struct stack_node *next;
  char val_ch;
  int val_i;
} Node;

typedef struct ll_stack_holder{
  Node *top;
  int size; //may not get used, here for pure functionality
} *Stack;

// BASIC STACK FXS
Stack stack_init(){
  Stack s = malloc( sizeof( struct ll_stack_holder) );
  s->top = NULL;
  s->size = 0;
  return s;
}

void stack_push_ch(Stack s, char val){
  Node* add = malloc(sizeof( Node) );
  add->val_ch = val;
  add->next = s->top;
  s->top = add;
  s->size++;
}
void stack_push_i(Stack s, int val){
  Node* add = malloc(sizeof( Node) );
  add->val_i = val;
  add->next = s->top;
  s->top = add;
  s->size++;
}

char stack_pop_ch(Stack s){
  if(s->top == NULL)
    return '\0';
  else{
    Node* temp = s->top;
    char ret =  temp->val_ch;
    s->top = temp->next;
    free( temp);
    s->size--;
    return ret;
  }
}
int stack_pop_i(Stack s){
  if(s->top == NULL)
    return '\0';
  else{
    Node* temp = s->top;
    int ret =  temp->val_i;
    s->top = temp->next;
    free( temp);
    s->size--;
    return ret;
  }
}

int stack_isEmpty(Stack s){
  return s->top == NULL ? 1 : 0;
}

//the 'top' function
char stack_peek_ch( Stack s){
  return s->top == NULL ? '\0' : s->top->val_ch;
}
int stack_peek_i( Stack s){
  return s->top == NULL ? -999 : s->top->val_i;
}

void stack_free(Stack s){
  Node* temp = s->top;
  Node* prev = s->top;
  while(temp != NULL){
    temp = temp->next;
    free(prev);
    prev = temp;
  }

  free(s);
}

// ADV STACK FXS

int int_op_int_eval( int num1, char op, int num2){
  switch( op){
    case '+': return num1+num2;
    case '-': return num1-num2;
    case '*': return num1*num2;
    case '/': return num2 == 0 ? -999 : num1/num2 ;
    //both of these cases would need to be added to the algorithm
    //case '%': return num1%num2; //optional modulous operation
    //case '^': return pow(num1,num2); //optional power operation
    default: printf("FATAL ERROR: passed wrong op!\n"); return -999;
  };
}

void stack_pop_eval_push(Stack ops, Stack nums){
  int num1 = stack_pop_i( nums);
  int num2 = stack_pop_i( nums);
  char op = stack_pop_ch( ops);
  //printf("num1:%d: num2:%d: op:%c:\n", num1, num2, op);
  //passes the nums in opposite order to reflect ACTUAL order in equation
  int fx = int_op_int_eval( num2, op, num1);
  stack_push_i( nums, fx); 
}

// GEN FXS
void gen_parse_args( int argc, char *argv[], int *debugMode){
  int i;
  char debug[] = "-d";
  for( i = 1; i < argc; i++){
    if( 0 == strcmp(argv[i], debug) )
      *debugMode = 1;
  }
}

// END FX ADDITIONS

typedef enum {FALSE = 0, TRUE, NO = 0, YES} boolean;

typedef enum {ERROR = 0, OPERATOR, VALUE, EOLN, QUIT, HELP} tokenType;

typedef struct tokenStruct
{
  tokenType type;
  char      op;
  int       val;
} token;
token getInputToken (FILE *in);

void processExpression (token inputToken, FILE *in, int d)
{
  /**********************************************/
  /* Declare both stack head pointers here      */
  Stack ops = stack_init( ops);
  Stack nums = stack_init( nums);
  /* Loop until the expression reaches its End */
  while (inputToken.type != EOLN)
  {
    /* The expression contains an OPERATOR */
    if (inputToken.type == OPERATOR)
    {
      /* make this a debugMode statement */
      if( d) printf ("OP:%c, " ,inputToken.op);
      // add code to perform this operation here
      //if the op is a open paren
      if( inputToken.op == '(' )
        stack_push_ch( ops, inputToken.op);
      //if the op is + or -
      else if( inputToken.op == '+' || inputToken.op == '-'){
        while( !stack_isEmpty(ops) && (stack_peek_ch(ops) == '+' 
                                  ||  stack_peek_ch(ops) == '-'
                                  ||  stack_peek_ch(ops) == '/'
                                  ||  stack_peek_ch(ops) == '*') ){
          stack_pop_eval_push( ops, nums);
        }
        stack_push_ch( ops,inputToken.op);
      }
      //if the op is * or /
      else if( inputToken.op == '*' || inputToken.op == '/'){
        while( !stack_isEmpty(ops) && (stack_peek_ch(ops) == '/' 
                                  ||  stack_peek_ch(ops) == '*') ){
          stack_pop_eval_push( ops, nums);
        }
        stack_push_ch( ops,inputToken.op);
      }
      //if the op is a closing paren
      else if( inputToken.op == ')' ){
        while( !stack_isEmpty(ops) && stack_peek_ch(ops) != '(' ){
          stack_pop_eval_push( ops, nums);
        }
        if( stack_isEmpty( ops)) printf("ERROR, no closing parenthesis!\n");
        else stack_pop_ch( ops); //pop the open parenthesis
      }
    }

    /* The expression contain a VALUE */
    else if (inputToken.type == VALUE)
    {
      /* make this a debugMode statement */
      if(d) printf ("Val: %d, ", inputToken.val);
      // add code to perform this operation here
      stack_push_i( nums, inputToken.val);
    }

    /* get next token from input */
    inputToken = getInputToken (in);
  } 

  /* The expression has reached its end */
  // add code to perform this operation here
  while( !stack_isEmpty( ops) ){
    stack_pop_eval_push( ops, nums);
  }
  printf("The top of the val stack: %d\n", stack_peek_i( nums) );
  stack_pop_i( nums);
  if( !stack_isEmpty( nums) ) printf("ERROR, leftover nums on num stack\n");
  
  stack_free( ops);
  stack_free( nums);
  printf ("\n");
}


/**************************************************************/
/*                                                            */
/*  The Code below this point should NOT need to be modified  */
/*  for this program.   If you feel you must modify the code  */
/*  below this point, you are probably trying to solve a      */
/*  more difficult problem that you are being asked to solve  */
/*                                                            */
/**************************************************************/

token getInputToken (FILE *in)
{
  token retToken;
  retToken.type = QUIT;

  int ch;
  ch = getc(in);
  if (ch == EOF)
    return retToken;
  while (('\n' != ch) && isspace (ch))
  {
    ch = getc(in);
    if (ch == EOF)
      return retToken;
  }

  /* check for a q for quit */
  if ('q' == ch)
  {
    retToken.type = QUIT;
    return retToken;
  }

  /* check for a ? for quit */
  if ('?' == ch)
  {
    retToken.type = HELP;
    return retToken;
  }

  /* check for the newline */
  if ('\n' == ch)
  {
    retToken.type = EOLN;
    return retToken;
  }

  /* check for an operator: + - * / ( ) */
  if ( ('+' == ch) || ('-' == ch) || ('*' == ch) ||
      ('/' == ch) || ('(' == ch) || (')' == ch) )
  {
    retToken.type = OPERATOR;
    retToken.op = ch;
    return retToken;
  }

  /* check for a number */
  if (isdigit(ch))
  {
    int value = ch - '0';
    ch = getc(in);
    while (isdigit(ch))
    {
      value = value * 10 + ch - '0';
      ch = getc(in);
    }
    ungetc (ch, in);  /* put the last read character back in input stream */
    retToken.type = VALUE;
    retToken.val = value;
    return retToken;
  }

  /* else token is invalid */
  retToken.type = ERROR;
  return retToken;
}

/* Clear input until next End of Line Character - \n */
void clearToEoln(FILE *in)
{
  int ch;

  do {
    ch = getc(in);
  }
  while ((ch != '\n') && (ch != EOF));
}

void printCommands()
{
  printf ("The commands for this program are:\n\n");
  printf ("q - to quit the program\n");
  printf ("? - to list the accepted commands\n");
  printf ("or any infix mathematical expression using operators of (), *, /, +, -\n");
}

int main (int argc, char **argv)
{

  char *input;
  token inputToken;

// ************START ADDITION
  int debug = 0;
  gen_parse_args( argc, argv, &debug);
// ************END ADDITON


  printf ("Starting Expression Evaluation Program\n\n");
  printf ("Enter Expression: ");

  inputToken = getInputToken (stdin);
  while (inputToken.type != QUIT)
  {
    /* check first Token on Line of input */
    if(inputToken.type == HELP)
    {
      printCommands();
      clearToEoln(stdin);
    }
    else if(inputToken.type == ERROR)
    {
      printf ("Invalid Input - For a list of valid commands, type ?\n");
      clearToEoln(stdin);
    }
    else if(inputToken.type == EOLN)
    {
      printf ("Blank Line - Do Nothing\n");
      /* blank line - do nothing */
    }
    else 
    {
      processExpression(inputToken, stdin, debug);
    } 

    printf ("\nEnter Expression: ");
    inputToken = getInputToken (stdin);
  }

  printf ("Quitting Program\n");
  return 1;
}
