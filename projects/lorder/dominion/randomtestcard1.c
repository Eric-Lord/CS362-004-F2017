
/* -----------------------------------------------------------------------
 * Demonstration of how to write random tests for dominion-base
 * Include the following lines in your makefile:
 *
 * randomtestcard1: randomtestcard1.c dominion.o rngs.o
 *      gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

/* Card Test for Smithy
   Smithy behavior:
     - draw three cards from the deck and add them to their hand
     - discard the smithy card
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "dominion.h"
#include "unitTest_helpers.h"

#define TEST_CARD smithy
#define NUMTESTS 1000
#define MIN_PLAYERS 2
#define MAX_DISCARD_COUNT 50
#define MAX_DECK_COUNT 50
#define MAX_HAND_COUNT 10
#define MAX_NUM_ACTIONS 5
#define MAX_NUM_BUYS 5
#define MAX_CARDS_PLAYED 10
#define MAX_NUM_COINS 5
#define TOTAL_KINGDOMCARDS 20
#define CUR_PLAYER 0
#define FALSE 0
#define TRUE 1

int seed;

void CreateRandomGameState(struct gameState * G) 
{
  int numPlayers, i;
  int randomDeckCount, randomHandCount, randomDiscardCount;
  int randomNumBuys, randomNumActions, randomCoins, randomPlayedCardCount;
  int k[10];      // smithy plus 9 other randomly selected kingdom cards
  int numKingdoms;
  int dupKingdom, randomKingdom;
  int randomCard;

  // setup random kingdom cards, kingdom cards must be unique, one should be smithy
  k[0] = smithy;
  numKingdoms = 1;
  while (numKingdoms <= 10)
  {
    dupKingdom = FALSE;
    randomKingdom = (rand() % TOTAL_KINGDOMCARDS) + adventurer;

    //check if this card has already been selected
    for (i=0; i < numKingdoms; i++) {
      if (k[i] == randomKingdom)
        dupKingdom = TRUE;
    }
    if (!dupKingdom)
    {
      k[i] = randomKingdom;
      numKingdoms++;
    }
  }
  // setup random number of players
  numPlayers = rand()%((MAX_PLAYERS - MIN_PLAYERS) + 1) + MIN_PLAYERS;

  // initialize a game state and player cards
  seed = rand();
  initializeGame(numPlayers, k, seed, G);
  
  randomNumBuys = rand() % MAX_NUM_BUYS;
  randomNumActions = 1 + rand() % MAX_NUM_ACTIONS;
  randomCoins = rand() % MAX_NUM_COINS;
  randomPlayedCardCount = rand() % MAX_CARDS_PLAYED;
  G->whoseTurn = 0;
  G->numActions = randomNumActions;
  G->numBuys = randomNumBuys;
  G->playedCardCount = randomPlayedCardCount;
  G->coins = randomCoins;

  // randomize game state to simulate smithy card play at some point during a game
  // get random counts for deck, hand, and discard
  randomDeckCount = 10 + rand() % (MAX_DECK_COUNT);
  randomHandCount = 2 + rand() % MAX_HAND_COUNT;
  randomDiscardCount = rand() % MAX_DISCARD_COUNT;
  //set player deck, hand and discard counts
  G->deckCount[CUR_PLAYER] = randomDeckCount;
  G->handCount[CUR_PLAYER] = randomHandCount;
  G->discardCount[CUR_PLAYER] = randomDiscardCount;
  
  //Set cards in players hand
  //Make smithy card be card in hand position zero
  G->hand[CUR_PLAYER][0] = smithy;
  //Fill in rest of players hand
  for (i = 1; i < G->handCount[CUR_PLAYER]; i++) 
  {
    //select random card from available cards
    randomCard = rand() % treasure_map + 1;
    //check supply for card availability
    if (G->supplyCount[randomCard] > -1){
      //enough in supply so add it
      G->hand[CUR_PLAYER][i] = randomCard;
    }
    else{
      //try again, not enough cards
      i--;
    }
  }
  
  //Setup players deck
  for (i = 0; i < G->deckCount[CUR_PLAYER]; i++) 
  {
    //select random card from available cards
    randomCard = rand() % treasure_map + 1;
    //check supply for card availability
    if (G->supplyCount[randomCard] > -1){
      //enough in supply so add it
      G->deck[CUR_PLAYER][i] = randomCard;
    }
    else{
      //try again, not enough cards
      i--;
    }
  }
  
  //Setup players discard
  for (i = 0; i < G->discardCount[CUR_PLAYER]; i++) {
    randomCard = rand() % treasure_map + 1;
    //check supply for card availability
    if (G->supplyCount[randomCard] > -1){
      //enough in supply so add it
      G->discard[CUR_PLAYER][i] = randomCard;
    }
    else{
      //try again, not enough cards
      i--;
    }
  }

}

void test_Smithy()
{
  struct gameState G, testG;
  int testFailed = 0;
  int numTestFailures = 0;
  int assertResult;
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int player, run;

  printf("\n\n------------------Smithy Start Random Tests-------------------------------------------\n");
  //iterate over number of test runs
  for (run=0; run < NUMTESTS; run++) {

    //create a random game state
    CreateRandomGameState(&G);
    
    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    //play the smithy card
    cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);


    //  test the results of card effect
    //  want to test the following:
    //    - players handcount increase by two => draw three and then discard one
    //    - the deck should decrease by three if deck contains three or more cards
    //    - smithy should be in playedCards
    //    - should not affect other players hand, deck or discard counts


    //print game state
    printf("------------------Smithy Start Random Test: %d seed: %d -------------------------------------------\n\n", run+1, seed);
    
    /* Players hand should get 2 additional card : draw three and subtract one because the Smithy card should be discarded */
    assertResult = unitTestAssert("Player's hand count should increase by two cards - drew three and discard one",
      (testG.handCount[CUR_PLAYER] == G.handCount[CUR_PLAYER] + 2), 0);
    if(assertResult == 0)
    {
      printf("Number of cards in hand before play: %d \n", G.handCount[CUR_PLAYER]);
      printf("Number of cards in hand after play: %d \n", testG.handCount[CUR_PLAYER]);
      printf("Player's hand count is %d and it should be %d: \n", testG.handCount[CUR_PLAYER], G.handCount[CUR_PLAYER] +2);
      numTestFailures++;
    }

    //the deck should decrease by three if the deck contains at least three cards
    if(G.deckCount[CUR_PLAYER] > 3)
    {
      assertResult = unitTestAssert("The deck should decrease by three if the players deck contains at least three cards",
      (testG.deckCount[CUR_PLAYER] == G.handCount[CUR_PLAYER] - 3), 0);
      if(assertResult == 0)
      {
        printf("Number of cards in deck before play: %d \n", G.deckCount[CUR_PLAYER]);
        printf("Number of cards in deck after play: %d \n", testG.deckCount[CUR_PLAYER]);
        printf("Player's deck count is %d and it should be %d: \n", testG.deckCount[CUR_PLAYER], G.deckCount[CUR_PLAYER] - 3);
        numTestFailures++;
      }
    }

    /* Should not affect other players hands*/
    for(player=1; player < testG.numPlayers; player++)
    {
      int deckChanged = 0;
      int handChanged = 0;
      int discardChanged = 0;
      int i;
      for (i = 0; i < MAX_DECK; i++)
      {
        if (testG.deck[player][i] != G.deck[player][i])
        {
          deckChanged = 1;
        }
      }

      for (i = 0; i < MAX_DECK; i++)
      {
        if (testG.discard[player][i] != G.discard[player][i])
        {
          discardChanged = 1;
        }
      }

      for (i = 0; i < MAX_HAND; i++)
      {
        if (testG.hand[player][i] != G.hand[player][i])
        {
          handChanged = 1;
        }
      }

      char buffer[100];
      snprintf(buffer, 100, "Affected player %d Should not affect other player", player);
      
      assertResult = unitTestAssert(buffer, (
        testG.handCount[player] == G.handCount[player]
        && testG.deckCount[player] == G.deckCount[player]
        && testG.discardCount[player] == G.discardCount[player]
        && handChanged == 0 && deckChanged == 0 && discardChanged == 0),0
      ); 
      if(assertResult == 0)
      {
        numTestFailures++;
      }
    }

    /* Should move played card to playedCards */
    assertResult = unitTestAssert("Should move smithy card to playedCards",
      hasCard(smithy, testG.playedCards, testG.playedCardCount), 0
    );
    if(assertResult == 0)
    {
      numTestFailures++;
    }

    printf("------------------Smithy End Random Test: %d, Number of Tests Failed: %d -------------------------------------------\n\n", run, numTestFailures);
  }


}

int main() {
  time_t t;
  int seedx = (unsigned) time(&t);
  srand(seedx);
  test_Smithy();
  return 0;
}

