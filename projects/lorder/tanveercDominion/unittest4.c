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
	scoreFor will return for the following cards contained in the player's hand, deck and discard:
		- curse -1
		- estate +1
		- duchy +3
		- province +6
		- great_hall +1
		- gardens +1 for every 10 cards
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

	//initialize deck, hand and discard for player 1
	G.hand[Player1][0] = adventurer;
	G.hand[Player1][1] = curse;
	G.handCount[Player1] = 2;
	G.deck[Player1][0] = great_hall;
	G.deck[Player1][1] = copper;
	G.deck[Player1][2] = mine;
	G.deck[Player1][3] = duchy;
	G.deck[Player1][4] = silver;
	G.deck[Player1][5] = smithy;
	G.deck[Player1][6] = gold;
	G.deckCount[Player1] = 7;
	G.discard[Player1][0] = estate;
	G.discard[Player1][1] = council_room;
	G.discardCount[Player1] = 2;
	//initialize deck, hand and discard for player 2
	G.hand[Player2][0] = adventurer;
	G.handCount[Player2] = 1;
	G.deck[Player2][0] = copper;
	G.deck[Player2][1] = adventurer;
	G.deck[Player2][2] = mine;
	G.deck[Player2][3] = gardens;
	G.deck[Player2][4] = silver;
	G.deck[Player2][5] = baron;
	G.deckCount[Player2] = 6;
	G.discard[Player2][0] = gardens;
	G.discard[Player2][1] = council_room;
	G.discardCount[Player2] = 2;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//test that scoreFor returns  5 for player 1
	score = scoreFor(Player1, &testG);

	//score should be 5 because the deck, hand and discard contain 5 victory cards
	assertResult = unitTestAssert("score should be 4 because the deck, hand and discard contain victory cards adding to 4", (score == 4),2);
	if(assertResult == 0)
	{
		printf("score %d\n", score);
		testFailed = 1;
	}

	// TEST 2: testing scoreFor for Player1 with no cards in deck, hand and discard and cards in deck, hand and discard for Player 2----------
	printf("TEST 2: testing scoreFor for Player1\n");

	
	//initialize deck, hand and discard for player 1
	G.handCount[Player1] = 0;
	G.deckCount[Player1] = 0;
	G.discardCount[Player1] = 0;
	//initialize deck, hand and discard for player 2
	G.hand[Player2][0] = adventurer;
	G.handCount[Player2] = 1;
	G.deck[Player2][0] = copper;
	G.deck[Player2][1] = adventurer;
	G.deck[Player2][2] = mine;
	G.deck[Player2][3] = gardens;
	G.deck[Player2][4] = silver;
	G.deck[Player2][5] = baron;
	G.deckCount[Player2] = 6;
	G.discard[Player2][0] = gardens;
	G.discard[Player2][1] = council_room;
	G.discardCount[Player2] = 2;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//test that scoreFor returns 0 for player 1
	score = scoreFor(Player1, &testG);

	//score should be 0 when deck, hand, and discard are empty
	assertResult = unitTestAssert("scoreFor should return 0 when deck, hand, and discard are empty", (score == 0),2);
	if(assertResult == 0)
	{
		printf("score %d\n", score);
		testFailed = 1;
	}

	// TEST 3: testing scoreFor for Player1 with cards in hand only ----------
	printf("TEST 3: testing scoreFor for Player1 with victory cards in hand only\n");

	//initialize deck, hand and discard for player 1
	G.hand[Player1][0] = duchy;
	G.hand[Player1][1] = adventurer;
	G.hand[Player1][2] = gardens;
	G.handCount[Player1] = 3;
	G.deck[Player1][0] = adventurer;
	G.deckCount[Player1] = 1;
	G.discardCount[Player1] = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//test that scoreFor returns   for player 1
	score = scoreFor(Player1, &testG);

	//result should be 0 because supplyCount[province] > 0
	assertResult = unitTestAssert("scoreFor should return 3 because only hand contains victory cards", (score == 3),2);
	if(assertResult == 0)
	{
		printf("score %d\n", score);
		testFailed = 1;
	}

	// TEST 4: testing scoreFor for Player1 with cards in deck only ----------
	printf("TEST 4: testing scoreFor for Player1 with victory cards in deck only\n");

	//initialize deck, hand and discard for player 1
	G.handCount[Player1] = 0;
	G.deck[Player1][0] = adventurer;
	G.deck[Player1][1] = estate;
	G.deck[Player1][2] = smithy;
	G.deckCount[Player1] = 3;
	G.discard[Player1][0] = embargo;
	G.discardCount[Player1] = 1;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//test that scoreFor returns 1  for player 1
	score = scoreFor(Player1, &testG);

	//score should be 1 because deck contains a victory
	assertResult = unitTestAssert("scoreFor should return 1 because only deck contains one victory card", (score == 1),2);
	if(assertResult == 0)
	{
		printf("score %d\n", score);
		testFailed = 1;
	}

	// TEST 5: testing scoreFor for Player1 with cards in discard only ----------
	printf("TEST 5: testing scoreFor for Player1 with victory cards in discard only\n");

	//initialize deck, hand and discard for player 1
	G.hand[Player1][0] = feast;
	G.hand[Player1][1] = smithy;
	G.handCount[Player1] = 2;
	G.deck[Player1][0] = baron;
	G.deck[Player1][1] = village;
	G.deck[Player1][2] = smithy;
	G.deckCount[Player1] = 3;
	G.discard[Player1][0] = gardens;
	G.discard[Player1][1] = great_hall;
	G.discard[Player1][2] = tribute;
	G.discard[Player1][3] = curse;
	G.discard[Player1][4] = council_room;
	G.discardCount[Player1] = 5;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//test that scoreFor returns 2 for player 1
	score = scoreFor(Player1, &testG);

	//score should be 3 because discard contains two victory cards
	assertResult = unitTestAssert("scoreFor should return 1 because only discard contains victory cards", (score == 1),2);
	if(assertResult == 0)
	{
		printf("score %d\n", score);
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
