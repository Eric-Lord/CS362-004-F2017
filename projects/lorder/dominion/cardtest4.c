
/* Card Test for Village
   Village behavior:
     - allows the player to draw a card
     - adds 2 to the actions
*/

#include <stdio.h>
#include <string.h>
#include "dominion.h"
#include "unitTest_helpers.h"

#define Player1 0
#define Player2 1

int test_Village() 
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

  printf("----- Testing Village card -----\n");

  /* Set up state for the players */
  printf("Test: Player 1 discardCount=0, deckCount=7, and handCount=2\n");
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testG.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  testG.hand[Player1][0] = outpost;
  testG.hand[Player1][1] = gardens;
  testG.handCount[Player1] = 2;
  testG.deck[Player1][0] = great_hall;
  testG.deck[Player1][1] = copper;
  testG.deck[Player1][2] = mine;
  testG.deck[Player1][3] = gardens;
  testG.deck[Player1][4] = silver;
  testG.deck[Player1][5] = village;
  testG.deck[Player1][6] = gold;
  testG.deckCount[Player1] = 7;
  testG.discardCount[Player1] = 0;
  testG.outpostPlayed = 0;

  cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* Should add a card to the players hand */
  assertResult = unitTestAssert("Should add a card to the players hand", (testG.handCount[Player1] == G.handCount[Player1] + 1));
  if(assertResult == 0)
  {
    testFailed = 1;
  }


  /* Should increment the actions by 2 */
  assertResult = unitTestAssert("Should increment the actions by 2", (testG.numActions == G.numActions + 2));
  if(assertResult == 0)
  {
    testFailed = 1;
  }
  
  /* Should move played card to playedCards - playedCards should have outpost */
  assertResult = unitTestAssert("Should move played card to playedCards - playedCards should have village",
    (hasCard(village, testG.playedCards, testG.playedCardCount)));

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
    && handChanged == 0 && deckChanged == 0 && discardChanged == 0)
  ); 
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  if(testFailed == 0)
  {
    printf("All tests passed for isGameOver!\n");
  }

  return 0;
}

int main() {
  test_Village();
  return 0;
}