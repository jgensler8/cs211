/*
 * **EDITED** BY: Jeff Gensler
 * Date: 11/06/2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DECKSIZE 52
#define VALUE 9
#define FACE 4

typedef struct{
int value;
char suit [10];
char name [10];
}Card;


void printDeck(Card cards[]){
  int i;

  for(i=0; i < DECKSIZE;i++){
    printf("%s of %s value=%d\n",cards[i].name,cards[i].suit,cards[i].value);
    if((i+1)%13==0 && i!=0)printf("------------------\n\n");
  }
}


void shuffle(Card cards[]){
  Card temp;
  int i;
  int this;
  int that;

  for(i=0;i<100;i++){
    this=rand()%(DECKSIZE);
    that=rand()%(DECKSIZE);
    /* printf("%d and %d\n", this, that); */
    temp = cards[this];
    cards[this]=cards[that];
    cards[that]=temp;

  }
}

void initDeck(Card cards[]){
  char faceName[13][10]={"two","three","four","five","six","seven","eight",
                         "nine","ten","jack","queen","king","ace"};

  char suitName[4][10]={"spades","diamonds","clubs","hearts"};

  int suitCount=0;
  int faceCount=0;
  int i;

  srand(time(NULL));

  for(i=0;i<DECKSIZE;i++){
    if(faceCount<9){
      cards[i].value=faceCount+2;
    }
    else{
      cards[i].value=10;
    }
    strcpy(cards[i].suit, suitName[suitCount]);
    strcpy(cards[i].name, faceName[faceCount++]);
    if(faceCount==13){
      cards[i].value=11;
      suitCount++;
      faceCount=0;
    }
  }

  /* printDeck(cards); */
  shuffle(cards);
  /* printDeck(cards); */
}

int evaluateHand(Card hand[], int numCards)
{
  int points = 0;
  int i;

  for (i = 0; i < numCards; i++)
  {
    points = points + hand[i].value;
  }

  for (i = 0; i < numCards; i++)
  {
    if (points > 21 && hand[i].value == 11)
       points = points - 10;
  }

  if (points > 21)
   points = -1;

  return points;  
}

void dealerBlackJack(Card player[], int playerCount, 
                     Card dealer[], int dealerCount)
{
  int i;

  printf ("Dealer\'s cards:\n");
  for ( i = 0; i < dealerCount ; i++)
    printf ("  %s of %s\n", dealer[i].name, dealer[i].suit); 
  printf ("  Points: %d\n", evaluateHand(dealer, dealerCount));

  if (evaluateHand(player, playerCount) == 21)
  {
    printf ("Both Dealer and Player have Blackjack.  Tie\n");
  }
  else
  {
    printf ("The Dealer has Blackjack.  You lose\n");
  }
}

void playHand (Card player[], int playerCount, 
               Card dealer[], int dealerCount,
               Card deck[], int cardsLeft)
{
  int playerPoints;
  int dealerPoints;
  int i;
  char input[100];

  do 
  {
    printf ("Do you want another card? (y/n):");
    gets(input);
    if (input[0] == 'y')
      player[playerCount++] = deck[--cardsLeft];

    printf ("Player\'s cards:\n");
    for ( i = 0; i < playerCount ; i++)
      printf ("  %s of %s\n", player[i].name, player[i].suit); 
    printf ("  Points: %d\n", evaluateHand(player, playerCount));
    printf ("Dealer shows: %s of %s\n", dealer[0].name, dealer[0].suit);
  }
  while (input[0] == 'y' && evaluateHand(player, playerCount) != -1);

  if(evaluateHand(player, playerCount) == -1)
  {
    printf ("Your point value went over 21.  You lose\n");
    return ;
  }

  printf ("\nDealer\'s cards:\n");
  for ( i = 0; i < dealerCount ; i++)
    printf ("  %s of %s\n", dealer[i].name, dealer[i].suit); 
  printf ("  Points: %d\n", evaluateHand(dealer, dealerCount));
  while (evaluateHand(dealer, dealerCount) < 17)
  {
    dealer[dealerCount++] = deck[--cardsLeft];
    printf ("  Newcard for dealer: %s of %s\n", 
            dealer[dealerCount - 1].name, dealer[dealerCount - 1].suit); 
    printf ("  Points: %d\n", evaluateHand(dealer, dealerCount));
    if (evaluateHand(dealer, dealerCount) == -1)
    {
      printf ("The dealer went over 21 points.  You win\n");
      return ;
    }
  }

  playerPoints = evaluateHand(player, playerCount);
  dealerPoints = evaluateHand(dealer, dealerCount);
  if (playerPoints == dealerPoints)
  {
    printf("Dealer and Player have the same point total.  Tie\n");
  }
  else if (playerPoints > dealerPoints)
  {
    printf("Player has the higher point total.  You win.\n");
  }
  else
  {
    printf("Dealer has the higher point total.  You lose.\n");
  }

}



int main(){

  Card deck[DECKSIZE];
  Card player[5];
  Card dealer[5];
  int cardsLeft;
  int playerCount;
  int dealerCount;
  int i;

  initDeck(deck);
  cardsLeft = 52;

  player[0] = deck[--cardsLeft];
  dealer[0] = deck[--cardsLeft];
  player[1] = deck[--cardsLeft];
  dealer[1] = deck[--cardsLeft];
  playerCount = 2;
  dealerCount = 2;

  printf ("Player\'s cards:\n");
  for ( i = 0; i < playerCount ; i++)
    printf ("  %s of %s\n", player[i].name, player[i].suit); 
  printf ("  Points: %d\n", evaluateHand(player, playerCount));
  printf ("Dealer shows: %s of %s\n", dealer[0].name, dealer[0].suit);
 
  if (evaluateHand(dealer, dealerCount) == 21)
  {
    dealerBlackJack(player, playerCount, dealer, dealerCount);
  }
  else if (evaluateHand(player, playerCount) == 21)
  {
    printf ("Player has Blackjack.  You win.\n");
  }
  else
  {
    playHand(player, playerCount, dealer, dealerCount, deck, cardsLeft);
  }

  return 0;
}
