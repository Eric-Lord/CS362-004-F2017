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
	isGameOver will return 1: 
		- if stack of Province cards is empty or
		- if three supply pile are at 0
	isGameOver will return 0:
		- if stack of Province cards is not empty and
		- any of the three supply pile are > 0
*/

#define Player1 0
#define Player2 1
#define CARD1 smithy
#define CARD2 steward
#define CARD3 embargo

void test_isGameOver()
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

	printf("----------------- Testing isGameOver: ----------------\n");

	// TEST 1: supplyCount[province] = 2 ----------
	printf("TEST 1: supplyCount[province] = 2\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize variables
	testG.supplyCount[province] = 2;

	//test that isGameOver returns 0 because still have province cards
	result = isGameOver(&testG);

	//result should be 0 because supplyCount[province] > 0
	assertResult = unitTestAssert("isGameOver should return 0 when supplyCount[province] > 0", (result == 0));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 2: supplyCount[province] = 2 and one supply pile = 0 ----------
	printf("TEST 2: supplyCount[province] = 2 and one supply pile = 0\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize variables
	testG.supplyCount[province] = 2;
	testG.supplyCount[CARD1] = 0;

	//test that isGameOver returns 0 because still have province cards and only one supply pile is empty
	result = isGameOver(&testG);

	//result should be 0 because supplyCount[province] > 0 and only one supply pile is empty
	assertResult = unitTestAssert("isGameOver should return 0 when supplyCount[province] > 0 and only one supply pile is empty", (result == 0));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 3: supplyCount[province] = 2 and two supply piles = 0 ----------
	printf("TEST 3: supplyCount[province] = 2 and two supply piles = 0\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize variables
	testG.supplyCount[province] = 2;
	testG.supplyCount[CARD1] = 0;
	testG.supplyCount[CARD2] = 0;

	//test that isGameOver returns 0 because still have province cards and only two supply piles are empty
	result = isGameOver(&testG);

	//result should be 0 because supplyCount[province] > 0 and only two supply piles are empty
	assertResult = unitTestAssert("isGameOver should return 0 when supplyCount[province] > 0 and only two supply piles are empty", (result == 0));
	if(assertResult == 0)
	{
		testFailed = 1;
	}

	// TEST 4: supplyCount[province] = 2 and three supply piles = 0 ----------
	printf("TEST 4: supplyCount[province] = 2 and three supply piles = 0\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	//initialize variables
	testG.supplyCount[province] = 2;
	testG.supplyCount[CARD1] = 0;
	testG.supplyCount[CARD2] = 0;
	testG.supplyCount[CARD3] = 0;

	//test that isGameOver returns 1 because supplyCount[province] > 0 but three supply piles are empty
	result = isGameOver(&testG);

	//result should be 1 because supplyCount[province] > 0 but three supply piles are empty
	assertResult = unitTestAssert("isGameOver should return 1 when supplyCount[province] > 0 but three supply piles are empty", (result == 0));
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
    test_isGameOver();
    return 0;
}
