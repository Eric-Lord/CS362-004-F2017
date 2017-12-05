
/* Card Test for Smithy
   Smithy behavior:
     - draw three cards from the deck and add them to their hand
     - discard the smithy card
*/

#include <stdio.h>
#include <string.h>
#include "dominion.h"
#include "unitTest_helpers.h"

#define Player1 0
#define Player2 1
#define HANDPOSITION 2

int test_Smithy() 
{
  
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  struct gameState G, testG;
  int i;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
  int assertResult;
  int testFailed = 0;
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  
  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----- Testing Smithy card -----\n");

  /* Set up state for the players */
  printf("Test: Player 1 discardCount=0, deckCount=7, and handCount=2\n");
  
  G.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  G.hand[Player1][0] = smithy;
  G.hand[Player1][1] = gardens;
  G.handCount[Player1] = 2;
  G.deck[Player1][0] = great_hall;
  G.deck[Player1][1] = copper;
  G.deck[Player1][2] = mine;
  G.deck[Player1][3] = gardens;
  G.deck[Player1][4] = silver;
  G.deck[Player1][5] = village;
  G.deck[Player1][6] = gold;
  G.deckCount[Player1] = 7;
  G.discardCount[Player1] = 0;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* Should add three cards to player 1 hand and subtract one because the Smithy card should be discarded */
  assertResult = unitTestAssert("Should add 3 cards to player 1 hand and remove 1", (testG.handCount[Player1] == G.handCount[Player1] + 3 - 1),2);
  if(assertResult == 0)
  {
    printf("Hand count should increase by two. \n", G.handCount[Player1]);
    printf("Number of cards in hand before play: %d \n", G.handCount[Player1]);
    printf("Number of cards in hand after play: %d \n", testG.handCount[Player1]);
    testFailed = 1;
  }

  /* Should draw three cards from player 1 deck */
  assertResult = unitTestAssert("Should draw 3 cards from player 1 deck, so top 3 cards from deck should now be in player 1 hand",
    (hasCard(gold, testG.hand[Player1], testG.handCount[Player1])
    && hasCard(village, testG.hand[Player1], testG.handCount[Player1])
    && hasCard(silver, testG.hand[Player1], testG.handCount[Player1])),2
  );

  /* Should move played card to playedCards - playedCards should have smithy */
  assertResult = unitTestAssert("Should move played card to playedCards - playedCards should have smithy",
    (hasCard(smithy, testG.playedCards, testG.playedCardCount)),2);

  /* Should not affect other players */
  int deckChanged = 0;
  int handChanged = 0;
  int discardChanged = 0;
  for (i = 0; i < MAX_DECK; i++)
  {
    if (testG.deck[Player2][i] != G.deck[Player2][i])
    {
      deckChanged = 1;
    }
  }

  for (i = 0; i < MAX_DECK; i++)
  {
    if (testG.discard[Player2][i] != G.discard[Player2][i])
    {
      discardChanged = 1;
    }
  }

  for (i = 0; i < MAX_HAND; i++)
  {
    if (testG.hand[Player2][i] != G.hand[Player2][i])
    {
      handChanged = 1;
    }
  }

  assertResult = unitTestAssert("Should not affect other player", (
    testG.handCount[Player2] == G.handCount[Player2]
    && testG.deckCount[Player2] == G.deckCount[Player2]
    && testG.discardCount[Player2] == G.discardCount[Player2]
    && handChanged == 0 && deckChanged == 0 && discardChanged == 0),2
  ); 
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  if(testFailed == 0)
  {
    printf("All tests passed for Smithy!\n");
  }

  return testFailed;
}

int main() {
  int testFailed;
  testFailed = test_Smithy();
  FILE *f = fopen("testOutput.txt", "w");
  if (f == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }

  char *text;
  if (testFailed == 1)
  {
    const char *textF = "FAILED";
    fprintf(f, "%s", textF);
  }
  else
  {
    const char *textP = "PASSED";
    fprintf(f, "%s", textP);
  }
  fclose(f);
  
  return 0;
}



