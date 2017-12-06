/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include "unitTest_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/*
	buyCard behavior is as follows: as long as the prerequisites are met, the card the player indicates should be
	put in the discard pile.

	buyCard prerequisites:
		- has to be done during the buy phase
		- player has to have a number of coins greater than or equal to the cost of the card
		- the supply count of the card must be greater than or equal to one
		- player has to have a buy available
		- must be players turn

*/

#define CARDTOBUY smithy
#define Player1 0
#define Player2 1

int test_buyCard()
{
	int changed;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int i;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	int assertResult;
	int testFailed = 0;
	int result;
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing buyCard: ----------------\n");

	// TEST 1: numBuys = 0,  coins = 20, supplyCount = 2 ----------
	printf("TEST 1: numBuys = 0,  coins = 20, supplyCount = 2\n");

	//initialize variables
	G.numBuys = 0;
	G.coins = 20;
	G.supplyCount[CARDTOBUY] = 2;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//test that buy fails if player does not have a buy available
	result = buyCard(CARDTOBUY, &testG);

	//result should be -1 because numBuys < 1
	assertResult = unitTestAssert("buyCard should return -1 when numBuys < 1", (result == -1),2);
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	//the game state should not have changed
	changed = isGameStateChanged(&G, &testG);
	assertResult = unitTestAssert("Game state should not be modified when numBuys < 1", (changed == 0),2);
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 2: numBuys = 1,  coins = 0, supplyCount = 2 ----------
	printf("TEST 2: numBuys = 1,  coins = 0, supplyCount = 2\n");

	//initialize variables
	G.numBuys = 1;
	G.coins = 0;
	G.supplyCount[CARDTOBUY] = 2;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

  	result = buyCard(CARDTOBUY, &testG);

  	//the game state should not have changed
	changed = isGameStateChanged(&G, &testG);
	assertResult = unitTestAssert("Game state should not be modified when coins < cost", (changed == 0),2);
	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* Should return -1 when coins < cost */
	assertResult = unitTestAssert("buyCard should return -1 when coins < cost", (result == -1),2);
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 3: numBuys = 1,  coins = 20, supplyCount = 0 ----------
  	printf("TEST 3: numBuys = 1,  coins = 20, supplyCount = 0\n");

	//initialize variables
	G.numBuys = 1;
	G.coins = 20;
	G.supplyCount[CARDTOBUY] = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

  	result = buyCard(CARDTOBUY, &testG);

  	//the game state should not have changed
	changed = isGameStateChanged(&G, &testG);
	assertResult = unitTestAssert("Game state should not be modified when supplyCount < 1", (changed == 0),2);
	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* Should return -1 when coins < cost */
	assertResult = unitTestAssert("buyCard should return -1 when supplyCount < 1", (result == -1),2);
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 4: numBuys = 1,  coins = 20, supplyCount = 2 ----------
  	printf("TEST 4: numBuys = 1,  coins = 20, supplyCount = 2\n");

	//initialize variables
	G.numBuys = 1;
	G.coins = 20;
	G.supplyCount[CARDTOBUY] = 2;
	G.deck[Player2][0] = silver;
	G.deck[Player2][1] = smithy;
	G.deckCount[Player2] = 2;
	G.hand[Player2][0] = gardens;
	G.hand[Player2][1] = council_room;
	G.handCount[Player2] = 2;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	result = buyCard(CARDTOBUY, &testG);

	/* Should return 0 when a card is purchased */
  	assertResult = unitTestAssert("Should return 0 when a card is purchased", (result == 0),2);
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	/* Should reduce coins by purchased card cost */
  	assertResult = unitTestAssert("Should reduce player's coins by purchased card cost", (testG.coins == G.coins - getCost(CARDTOBUY)),2);
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* Should reduce card supplyCount by 1 */
  	assertResult = unitTestAssert("Should reduce card supplyCount by 1", (testG.supplyCount[CARDTOBUY] == G.supplyCount[CARDTOBUY] - 1),2);
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* Should reduce numBuys by 1 */
  	assertResult = unitTestAssert("Should reduce numBuys by 1", (testG.numBuys == G.numBuys - 1),2);
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	/* Should add purchased card to discard pile */
  	assertResult = unitTestAssert("Should add purchased card to discard pile", (hasCard(CARDTOBUY, testG.discard[Player1], testG.discardCount[Player1])),2);
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* Should increase increment discardCount */
  	assertResult = unitTestAssert("Should increase discardCount by 1", (testG.discardCount[Player1] == G.discardCount[Player1] + 1),2);
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
		&& handChanged == 0 && deckChanged == 0 && discardChanged == 0), 2
	); 
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	if(testFailed == 0)
	{
		printf("All tests passed for buyCard!\n");
	}
	return testFailed;
}

int main() {
    
  int testFailed;
  testFailed = test_buyCard();
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
