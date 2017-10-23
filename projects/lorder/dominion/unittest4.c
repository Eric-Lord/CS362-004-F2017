/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * unittest1: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
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
	scoreFor will return the one point for the following cards contained in the player's hand, deck and discard:
		- curse
		- estate
		- duchy
		- province
		- great_hall
		- gardens
*/

#define Player1 0
#define Player2 1

void test_scoreFor()
{

    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int i;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	int assertResult;
	int testFailed = 0;
	int score;

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing scoreFor: ----------------\n");

	// TEST 1: testing scoreFor for Player1 with victory cards in deck, hand and discard and cards in deck, hand and discard for Player 2----------
	printf("TEST 1: testing scoreFor for Player1\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize deck, hand and discard for player 1
	testG.hand[Player1][0] = adventurer;
	testG.hand[Player1][1] = gardens;
	testG.handCount[Player1] = 2;
	testG.deck[Player1][0] = great_hall;
	testG.deck[Player1][1] = copper;
	testG.deck[Player1][2] = mine;
	testG.deck[Player1][3] = gardens;
	testG.deck[Player1][4] = silver;
	testG.deck[Player1][5] = smithy;
	testG.deck[Player1][6] = gold;
	testG.deckCount[Player1] = 7;
	testG.discard[Player1][0] = estate;
	testG.discard[Player1][1] = council_room;
	testG.discardCount[Player1] = 2;
	//initialize deck, hand and discard for player 2
	testG.hand[Player2][0] = adventurer;
	testG.handCount[Player2] = 1;
	testG.deck[Player2][0] = copper;
	testG.deck[Player2][1] = adventurer;
	testG.deck[Player2][2] = mine;
	testG.deck[Player2][3] = gardens;
	testG.deck[Player2][4] = silver;
	testG.deck[Player2][5] = baron;
	testG.deckCount[Player2] = 6;
	testG.discard[Player2][0] = gardens;
	testG.discard[Player2][1] = council_room;
	testG.discardCount[Player2] = 2;

	//test that scoreFor returns  5 for player 1
	score = scoreFor(Player1, &testG);

	//score should be 5 because the deck, hand and discard contain 5 victory cards
	assertResult = unitTestAssert("score should be 5 because the deck, hand and discard contain 5 victory cards", (score == 5));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 2: testing scoreFor for Player1 with no cards in deck, hand and discard and cards in deck, hand and discard for Player 2----------
	printf("TEST 2: testing scoreFor for Player1\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize deck, hand and discard for player 1
	testG.handCount[Player1] = 0;
	testG.deckCount[Player1] = 0;
	testG.discardCount[Player1] = 0;
	//initialize deck, hand and discard for player 2
	testG.hand[Player2][0] = adventurer;
	testG.handCount[Player2] = 1;
	testG.deck[Player2][0] = copper;
	testG.deck[Player2][1] = adventurer;
	testG.deck[Player2][2] = mine;
	testG.deck[Player2][3] = gardens;
	testG.deck[Player2][4] = silver;
	testG.deck[Player2][5] = baron;
	testG.deckCount[Player2] = 6;
	testG.discard[Player2][0] = gardens;
	testG.discard[Player2][1] = council_room;
	testG.discardCount[Player2] = 2;

	//test that scoreFor returns 0 for player 1
	score = scoreFor(Player1, &testG);

	//score should be 0 when deck, hand, and discard are empty
	assertResult = unitTestAssert("scoreFor should return 0 when deck, hand, and discard are empty", (score == 0));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 3: testing scoreFor for Player1 with cards in hand only ----------
	printf("TEST 3: testing scoreFor for Player1 with victory cards in hand only\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize deck, hand and discard for player 1
	testG.hand[Player1][0] = duchy;
	testG.hand[Player1][1] = adventurer;
	testG.hand[Player1][2] = gardens;
	testG.handCount[Player1] = 3;
	testG.deck[Player1][0] = adventurer;
	testG.deckCount[Player1] = 1;
	testG.discardCount[Player1] = 0;

	//test that scoreFor returns   for player 1
	score = scoreFor(Player1, &testG);

	//result should be 0 because supplyCount[province] > 0
	assertResult = unitTestAssert("scoreFor should return 2 because only hand contains two victory card", (score == 2));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 4: testing scoreFor for Player1 with cards in deck only ----------
	printf("TEST 4: testing scoreFor for Player1 with victory cards in deck only\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize deck, hand and discard for player 1
	testG.handCount[Player1] = 0;
	testG.deck[Player1][0] = adventurer;
	testG.deck[Player1][1] = estate;
	testG.deck[Player1][2] = smithy;
	testG.deckCount[Player1] = 3;
	testG.discard[Player1][0] = embargo;
	testG.discardCount[Player1] = 1;

	//test that scoreFor returns 1  for player 1
	score = scoreFor(Player1, &testG);

	//score should be 1 because deck contains a victory
	assertResult = unitTestAssert("scoreFor should return 1 because only deck contains one victory card", (score == 1));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 5: testing scoreFor for Player1 with cards in discard only ----------
	printf("TEST 5: testing scoreFor for Player1 with victory cards in discard only\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize deck, hand and discard for player 1
	testG.hand[Player1][0] = feast;
	testG.handCount[Player1] = 1;
	testG.deck[Player1][0] = baron;
	testG.deck[Player1][1] = village;
	testG.deck[Player1][2] = smithy;
	testG.deckCount[Player1] = 3;
	testG.discard[Player1][0] = gardens;
	testG.discard[Player1][1] = great_hall;
	testG.discard[Player1][2] = tribute;
	testG.discard[Player1][3] = curse;
	testG.discard[Player1][4] = council_room;
	testG.discardCount[Player1] = 5;

	//test that scoreFor returns 2 for player 1
	score = scoreFor(Player1, &testG);

	//score should be 3 because discard contains two victory cards
	assertResult = unitTestAssert("scoreFor should return 3 because only discard contains three victory cards", (score == 3));
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
    test_scoreFor();
    return 0;
}
