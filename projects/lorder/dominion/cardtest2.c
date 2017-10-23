/* Test the Adventurer card
   Adventurer behavior:
     - draw cards from deck until two treasure cards are drawn
     - discard all other cards that are not adventurer
     - discard the adventurer card
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "dominion.h"
#include "unitTest_helpers.h"

#define Player1 0
#define Player2 1

int test_Adventurer() {
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  struct gameState G, testG;
  int i;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
  int assertResult;
  int testFailed = 0;
  int choice1 = 0;
  int choice2 = 0;
  int choice3 = 0;
  int handpos = 0;
  int bonus = 0;
  int result;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----- Testing Adventurer card -----\n");

  /* Test with only one treasure card */
  printf("\n----- Test: deckCount = 6, deck has gold, discardCount = 0\n");
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testG.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  testG.hand[Player1][0] = adventurer;
  testG.hand[Player1][1] = curse;
  testG.handCount[Player1] = 2;
  testG.deck[Player1][0] = great_hall;
  testG.deck[Player1][1] = gold;
  testG.deck[Player1][2] = mine;
  testG.deck[Player1][3] = gardens;
  testG.deck[Player1][4] = village;
  testG.deck[Player1][5] = council_room;
  testG.deckCount[Player1] = 6;
  testG.discardCount[Player1] = 0;

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* With deck containing only 1 treasure card hand should only get 1 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing only 1 treasure card, player's hand should only get the single treasure card",
    (testG.handCount[Player1] == G.handCount[Player1] + 1 - 1 && hasCard(gold, testG.hand[Player1], testG.handCount[Player1])));
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should stop drawing cards when two treasure card have been drawn */
  assertResult = unitTestAssert("Should stop drawing cards when 2 treasure cards have been drawn",
    (testG.deckCount[Player1] == G.deckCount[Player1] - 6)
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should discard all other cards drawn */
  assertResult = unitTestAssert("Should discard all other drawn cards",
    (testG.discardCount[Player1] == G.discardCount[Player1] + 5)
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount))
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

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

  /* Test with two treasure cards */
  printf("\n----- Test: deckCount = 6, deck has gold and silver, discardCount = 0\n");
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testG.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  testG.hand[Player1][0] = adventurer;
  testG.hand[Player1][1] = curse;
  testG.handCount[Player1] = 2;
  testG.deck[Player1][0] = great_hall;
  testG.deck[Player1][1] = gold;
  testG.deck[Player1][2] = silver;
  testG.deck[Player1][3] = gardens;
  testG.deck[Player1][4] = village;
  testG.deck[Player1][5] = council_room;
  testG.deckCount[Player1] = 6;
  testG.discardCount[Player1] = 0;

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* With deck containing two treasure card hand should get 2 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing two treasure cards, player's hand should get two treasure card",
    (testG.handCount[Player1] == G.handCount[Player1] + 2 - 1 && hasCard(gold, testG.hand[Player1], testG.handCount[Player1])));
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should discard all other cards drawn */
  assertResult = unitTestAssert("Should discard all other drawn cards",
    (testG.discardCount[Player1] == G.discardCount[Player1] + 3)
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount))
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should stop drawing cards when two treasure cards have been drawn */
  assertResult = unitTestAssert("Should stop drawing cards when 2 treasure cards have been drawn",
    (testG.deckCount[Player1] == G.deckCount[Player1] - 5)
  );

  /* Should not affect other players */
    deckChanged = 0;
    handChanged = 0;
    discardChanged = 0;
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


/* Test with three treasure cards */
  printf("\n----- Test: deckCount = 6, deck has gold, copper and silver, discardCount = 0\n");
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testG.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  testG.hand[Player1][0] = adventurer;
  testG.hand[Player1][1] = curse;
  testG.handCount[Player1] = 2;
  testG.deck[Player1][0] = great_hall;
  testG.deck[Player1][1] = gold;
  testG.deck[Player1][2] = silver;
  testG.deck[Player1][3] = gardens;
  testG.deck[Player1][4] = village;
  testG.deck[Player1][5] = copper;
  testG.deckCount[Player1] = 6;
  testG.discardCount[Player1] = 0;

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* With deck containing three treasure card hand should get 2 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing three treasure card, player's hand should get two treasure cards",
    (testG.handCount[Player1] == G.handCount[Player1] + 2 - 1 && hasCard(gold, testG.hand[Player1], testG.handCount[Player1])));
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should discard all other cards drawn */
  assertResult = unitTestAssert("Should discard all other drawn cards",
    (testG.discardCount[Player1] == G.discardCount[Player1] + 2)
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount))
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should stop drawing cards when two treasure cards have been drawn */
  assertResult = unitTestAssert("Should stop drawing cards when 2 treasure cards have been drawn",
    (testG.deckCount[Player1] == G.deckCount[Player1] - 4)
  );

  /* Should not affect other players */
    deckChanged = 0;
    handChanged = 0;
    discardChanged = 0;
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

  /* Test with an empty deck and a populated discard pile */
  printf("\n-------TEST: deckCount = 0, discardCount = 4, discard has gold");
  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));
  testG.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  testG.hand[Player1][0] = adventurer;
  testG.hand[Player1][1] = curse;
  testG.handCount[Player1] = 2;
  testG.deckCount[Player1] = 0;
  testG.discard[Player1][0] = estate;
  testG.discard[Player1][1] = gold;
  testG.discard[Player1][2] = mine;
  testG.discard[Player1][3] = embargo;
  testG.discard[Player1][4] = village;
  testG.discardCount[Player1] = 5;

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* Should draw treasure cards from deck that has been created from shuffled discard pile */
  assertResult = unitTestAssert("Should add discard to deck and shuffle, so one treasure cards is drawn",
    (testG.handCount[Player1] == G.handCount[Player1] + 1 - 1
    && hasCard(gold, testG.hand[Player1], testG.handCount[Player1]))
  );

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount))
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should not affect other players */
  deckChanged = 0;
  handChanged = 0;
  discardChanged = 0;
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
  test_Adventurer();
  return 0;
}