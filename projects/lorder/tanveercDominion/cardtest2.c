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
  int numTreasuresInHand = 0;

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----- Testing Adventurer card -----\n");

  /* Test with only one treasure card */
  printf("\n----- Test: deckCount = 6, deck has gold, discardCount = 0\n");
  
  G.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  G.hand[Player1][0] = adventurer;
  G.hand[Player1][1] = curse;
  G.handCount[Player1] = 2;
  G.deck[Player1][0] = great_hall;
  G.deck[Player1][1] = gold;
  G.deck[Player1][2] = mine;
  G.deck[Player1][3] = gardens;
  G.deck[Player1][4] = village;
  G.deck[Player1][5] = council_room;
  G.deckCount[Player1] = 6;
  G.discardCount[Player1] = 0;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  numTreasuresInHand = numCardTreasure(testG.hand[Player1], testG.handCount[Player1]);
  /* With deck containing only 1 treasure card hand should only get 1 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing only 1 treasure card, player's hand should contain the single treasure card",
    (numTreasuresInHand==1),2);
  if(assertResult == 0)
  {
    printf("Number of in hand: %d , should be 1\n", numTreasuresInHand);
    testFailed = 1;
  }

  /* With deck containing only 1 treasure card hand should only get 1 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing only 1 treasure card, player's hand count should remain the same",
    (testG.handCount[Player1] == G.handCount[Player1] + 1 - 1),2);
  if(assertResult == 0)
  {
    printf("Number of cards in hand before play: %d \n", G.handCount[Player1]);
    printf("Number of cards in hand after play: %d \n", testG.handCount[Player1]);
    testFailed = 1;
  }

  /* Should stop drawing cards when two treasure card have been drawn */
  assertResult = unitTestAssert("Looking for 2 treasure cards but only one in the deck so will exhaust the deck",
    (testG.deckCount[Player1] == G.deckCount[Player1] - 6),2
  );
  if(assertResult == 0)
  {
    printf("Number of cards in deck before play: %d \n", G.deckCount[Player1]);
    printf("Number of cards in deck after play: %d \n", testG.deckCount[Player1]);
    testFailed = 1;
  }

  /* Should discard all other cards drawn */
  assertResult = unitTestAssert("Should discard all other drawn cards, but keep the treasures",
    (testG.discardCount[Player1] == G.discardCount[Player1] + 5),2
  );
  if(assertResult == 0)
  {
    printf("Number of in discard: %d , should be 5\n", numTreasuresInHand);
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount)),2
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
    && handChanged == 0 && deckChanged == 0 && discardChanged == 0),2
  ); 
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Test with two treasure cards */
  printf("\n----- Test: deckCount = 6, deck has gold and silver, discardCount = 0\n");
  
  G.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  G.hand[Player1][0] = adventurer;
  G.hand[Player1][1] = curse;
  G.handCount[Player1] = 2;
  G.deck[Player1][0] = great_hall;
  G.deck[Player1][1] = gold;
  G.deck[Player1][2] = silver;
  G.deck[Player1][3] = gardens;
  G.deck[Player1][4] = village;
  G.deck[Player1][5] = council_room;
  G.deckCount[Player1] = 6;
  G.discardCount[Player1] = 0;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  numTreasuresInHand = numCardTreasure(testG.hand[Player1], testG.handCount[Player1]);
  /* With deck containing two treasure card hand should get 2 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing two treasure cards, player's hand should get two treasure card",
    (numTreasuresInHand==2),2);
  if(assertResult == 0)
  {
    printf("Number of in hand: %d , should be 2\n", numTreasuresInHand);
    testFailed = 1;
  }

  /* With deck containing two treasure card hand should get 2 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing two treasure cards, player's hand should increase by one",
    (testG.handCount[Player1] == G.handCount[Player1] + 2 - 1),2);
  if(assertResult == 0)
  {
    printf("Number of cards in hand before play: %d \n", G.handCount[Player1]);
    printf("Number of cards in hand after play: %d \n", testG.handCount[Player1]);
    testFailed = 1;
  }

  /* Should discard all other cards drawn */
  assertResult = unitTestAssert("Should discard all other drawn cards, but treasures.",
    (testG.discardCount[Player1] == G.discardCount[Player1] + 3),2
  );
  if(assertResult == 0)
  {
    printf("Number of in discard: %d , should be 3\n", numTreasuresInHand);
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount)),2
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should stop drawing cards when two treasure cards have been drawn */
  assertResult = unitTestAssert("Should stop drawing cards when 2 treasure cards have been drawn",
    (testG.deckCount[Player1] == G.deckCount[Player1] - 5),2
  );
  if(assertResult == 0)
  {
    printf("Number of cards in deck before play: %d \n", G.deckCount[Player1]);
    printf("Number of cards in deck after play: %d \n", testG.deckCount[Player1]);
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
    && handChanged == 0 && deckChanged == 0 && discardChanged == 0),2
  ); 
  if(assertResult == 0)
  {
    testFailed = 1;
  }


/* Test with three treasure cards */
  printf("\n----- Test: deckCount = 6, deck has gold, copper and silver, discardCount = 0\n");
  
  G.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  G.hand[Player1][0] = adventurer;
  G.hand[Player1][1] = curse;
  G.handCount[Player1] = 2;
  G.deck[Player1][0] = great_hall;
  G.deck[Player1][1] = gold;
  G.deck[Player1][2] = silver;
  G.deck[Player1][3] = gardens;
  G.deck[Player1][4] = village;
  G.deck[Player1][5] = copper;
  G.deckCount[Player1] = 6;
  G.discardCount[Player1] = 0;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  numTreasuresInHand = numCardTreasure(testG.hand[Player1], testG.handCount[Player1]);
  /* With deck containing three treasure card hand should get 2 additional card and subtract one because the Adventurer card should be discarded */
  assertResult = unitTestAssert("With deck containing three treasure card, player's hand should contain two treasure cards",
    (numTreasuresInHand==2),2);
  if(assertResult == 0)
  {
    printf("Number of in hand: %d , should be 2\n", numTreasuresInHand);
    testFailed = 1;
  }

  assertResult = unitTestAssert("With deck containing three treasure card, player's hand should increase by one",
    (testG.handCount[Player1] == G.handCount[Player1] + 2 - 1),2);
  if(assertResult == 0)
  {
    printf("Number of cards in hand before play: %d \n", G.handCount[Player1]);
    printf("Number of cards in hand after play: %d \n", testG.handCount[Player1]);
    testFailed = 1;
  }

  /* Should discard all other cards drawn */
  assertResult = unitTestAssert("Should discard all other drawn cards",
    (testG.discardCount[Player1] == G.discardCount[Player1] + 2),2
  );
  if(assertResult == 0)
  {
    printf("Number of in discard: %d , should be 2\n", numTreasuresInHand);
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount)),2
  );
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Should stop drawing cards when two treasure cards have been drawn */
  assertResult = unitTestAssert("Should stop drawing cards when 2 treasure cards have been drawn",
    (testG.deckCount[Player1] == G.deckCount[Player1] - 4),2
  );
  if(assertResult == 0)
  {
    printf("Number of cards in deck before play: %d \n", G.deckCount[Player1]);
    printf("Number of cards in deck after play: %d \n", testG.deckCount[Player1]);
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
    && handChanged == 0 && deckChanged == 0 && discardChanged == 0),2
  ); 
  if(assertResult == 0)
  {
    testFailed = 1;
  }

  /* Test with an empty deck and a populated discard pile */
  printf("\n-------TEST: deckCount = 0, discardCount = 4, discard has gold\n");
  
  G.whoseTurn = Player1;
  //initialize deck, hand and discard for player 1
  G.hand[Player1][0] = adventurer;
  G.hand[Player1][1] = curse;
  G.handCount[Player1] = 2;
  G.deckCount[Player1] = 0;
  G.discard[Player1][0] = estate;
  G.discard[Player1][1] = gold;
  G.discard[Player1][2] = mine;
  G.discard[Player1][3] = embargo;
  G.discard[Player1][4] = village;
  G.discardCount[Player1] = 5;

  // copy the game state to a test case
  memcpy(&testG, &G, sizeof(struct gameState));

  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

  /* Should draw treasure cards from deck that has been created from shuffled discard pile */
  assertResult = unitTestAssert("Should add discard to deck and shuffle, so one treasure cards is drawn - hand count should not change",
    (testG.handCount[Player1] == G.handCount[Player1] + 1 - 1),2
  );
  if(assertResult == 0)
  {
    printf("Number of cards in hand before play: %d \n", G.handCount[Player1]);
    printf("Number of cards in hand after play: %d \n", testG.handCount[Player1]);
    testFailed = 1;
  }

  numTreasuresInHand = numCardTreasure(testG.hand[Player1], testG.handCount[Player1]);
  /* Should draw treasure cards from deck that has been created from shuffled discard pile */
  assertResult = unitTestAssert("Should add discard to deck and shuffle, so one treasure cards is drawn",
    (numTreasuresInHand==1),2
  );
  if(assertResult == 0)
  {
    printf("Number of in hand: %d , should be 1\n", numTreasuresInHand);
    testFailed = 1;
  }

  /* Should move played card to playedCards */
  assertResult = unitTestAssert("Should move adventurer card to playedCards",
    (hasCard(adventurer, testG.playedCards, testG.playedCardCount)),2
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
    && handChanged == 0 && deckChanged == 0 && discardChanged == 0),2
  ); 
  if(assertResult == 0)
  {
    testFailed = 1;
  }
  
  
  if(testFailed == 0)
  {
    printf("All tests passed for Adventurer!\n");
  }

  return 0;
}

int main() {
  test_Adventurer();
  return 0;
}