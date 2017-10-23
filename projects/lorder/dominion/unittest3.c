/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest1: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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
	endTurn behavior is as follows: 
		- discard hand
		- draw five new cards
		- initialize player variables
		- switch players
*/

#define Player1 0
#define Player2 1

void test_endTurn()
{
	int changed;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int i;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	int assertResult;
	int testFailed = 0;
	int result;

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing endTurn: ----------------\n");

	// TEST 1: players hand has four cards, numBuys=0, coins=2, numActions=0, playedCardCount = 1 ----------
	printf("TEST 1: \n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize variables
	testG.hand[Player1][0] = copper;
	testG.hand[Player1][1] = remodel;
	testG.hand[Player1][2] = feast;
	testG.hand[Player1][3] = adventurer;
	testG.handCount[Player1] = 4;
	testG.deck[Player1][0] = silver;
	testG.deck[Player1][1] = smithy;
	testG.deckCount[Player1] = 2;
	testG.numBuys = 0;
	testG.coins = 2;
	testG.numActions = 0;
	testG.playedCardCount = 1;
	testG.phase = 2;
	testG.outpostPlayed = 1;
	
	//test endTurn with player having 4 cards in hand, numBuys=0, coins=2, numActions=0, playedCardCount = 1
	result = endTurn(&testG);

	//result should be 0
	assertResult = unitTestAssert("endTurn should always 0", (result == 0));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	//state->whoseTurn should now be Player2
	assertResult = unitTestAssert("whoseTurn should now be player 2", (testG.whoseTurn == 1));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* numActions should now be 1 */
  	assertResult = unitTestAssert("numActions should now be 1", (testG.numActions == 1));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

  	/* numBuys should now be 1 */
  	assertResult = unitTestAssert("numBuys should now be 1", (testG.numBuys == 1));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	/* playedCardCount should now be 0 */
  	assertResult = unitTestAssert("playedCardCount should now be 0", (testG.playedCardCount == 0));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	/* output should now be 0 */
  	assertResult = unitTestAssert("outpostPlayed should now be 0", (testG.outpostPlayed == 0));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	/* coins should now be 0 */
  	assertResult = unitTestAssert("coins should now be 0", (testG.coins == 0));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	/* phase should now be 0 */
  	assertResult = unitTestAssert("phase should now be 0", (testG.phase == 0));
  	if(assertResult == 0)
	{
		testFailed = 1;
	}

	if(testG.deckCount[Player1] > 4 )
	{
		/* Should add cards in hand to discard pile */
	  	assertResult = unitTestAssert("Should add cards in hand to discard pile", (
	  		hasCard(copper, testG.discard[Player1], testG.discardCount[Player1])
	  		&& hasCard(remodel, testG.discard[Player1], testG.discardCount[Player1])
	  		&& hasCard(feast, testG.discard[Player1], testG.discardCount[Player1])
	  		&& hasCard(adventurer, testG.discard[Player1], testG.discardCount[Player1])
	  		)
	  	);
	  	if(assertResult == 0)
		{
			testFailed = 1;
		}

	  	/* Should increment discardCount */
	  	assertResult = unitTestAssert("Should increase discardCount by 4", (testG.discardCount[Player1] == G.discardCount[Player1] + 4));
		if(assertResult == 0)
		{
			testFailed = 1;
		}

		/* Should decrement deck */
	  	assertResult = unitTestAssert("Should decrement deckCount by 5", (testG.deckCount[Player1] == G.deckCount[Player1] - 5));
		if(assertResult == 0)
		{
			testFailed = 1;
		}

	}
	else
	{

		/* Discard count should be 0 */
	  	assertResult = unitTestAssert("Discard count should be 0", (testG.discardCount[Player1] == 0));
		if(assertResult == 0)
		{
			testFailed = 1;
		}
	}

	/* Hand count should be five */
  	assertResult = unitTestAssert("Hand count should be 5", (testG.handCount[Player1] == 5));
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

	if(testFailed == 0)
	{
		printf("All tests passed for isGameOver!\n");
	}

}

int main() {
    test_endTurn();
    return 0;
}
