
/* -----------------------------------------------------------------------
 * Demonstration of how to write random tests for dominion-base
 * Include the following lines in your makefile:
 *
 * randomtesradventurer: randomtestadventurer.c dominion.o rngs.o
 *      gcc -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

/* Test the Adventurer card
   Adventurer behavior:
     - draw cards from deck until two treasure cards are drawn
     - discard all other cards that are not adventurer
     - discard the adventurer card
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "unitTest_helpers.h"

#define NUMTESTS 1000
#define MIN_PLAYERS 2
#define MAX_DISCARD_COUNT 50
#define MAX_DECK_COUNT 50
#define MAX_HAND_COUNT 10
#define MAX_NUM_ACTIONS 5
#define MAX_NUM_BUYS 5
#define MAX_CARDS_PLAYED 10
#define MAX_NUM_COINS 5
#define MIN_ADDED_TREASURES 0
#define MAX_ADDED_TREASURES 3
#define TOTAL_KINGDOMCARDS 20
#define CUR_PLAYER 0
#define FALSE 0
#define TRUE 1
#define KRED  "\x1B[31m"

int seed;

int CreateRandomGameState(struct gameState * G) 
{
	int numPlayers, i;
	
	int randomDeckCount, randomHandCount, randomDiscardCount;
	int randomNumBuys, randomNumActions, randomCoins, randomPlayedCardCount;
	int k[10];			// adventurer plus 9 other randomly selected kingdom cards
	int numKingdoms;
	int dupKingdom, randomKingdom;
	int randomCard;
	int minNumTreasures;
	int numTreasures = 0;

	// setup random kingdom cards, kingdom cards must be unique, one should be adventurer
	k[0] = adventurer;
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

	// randomize game state to simulate adventurer card play at some point during a game
	// get random counts for deck, hand, and discard
	randomDeckCount = 10 + rand() % (MAX_DECK_COUNT);
	randomHandCount = 2 + rand() % MAX_HAND_COUNT;
	randomDiscardCount = rand() % MAX_DISCARD_COUNT;
	//set player deck, hand and discard counts
	G->deckCount[CUR_PLAYER] = randomDeckCount;
	G->handCount[CUR_PLAYER] = randomHandCount;
	G->discardCount[CUR_PLAYER] = randomDiscardCount;
	
	//Set cards in players hand
	//Make adventurer card be card in hand position zero
	G->hand[CUR_PLAYER][0] = adventurer;
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
	
	//Setup players deck, give deck random number of treasure cards but atleast MIN_ADDED_TREASURES
	minNumTreasures = rand() % MAX_ADDED_TREASURES + MIN_ADDED_TREASURES;
	for (i = 0; i < G->deckCount[CUR_PLAYER]; i++) 
	{
		//select random card from available cards
		randomCard = rand() % treasure_map + 1;
		//check supply for card availability
		if (G->supplyCount[randomCard] > -1){
			//enough in supply so add it
			G->deck[CUR_PLAYER][i] = randomCard;
			//check if this was a treasure card
			if(randomCard == copper || randomCard == silver || randomCard ==  gold){
				numTreasures++;
			}
		}
		else{
			//try again, not enough cards
			i--;
		}
	}
	//check to see if numTreasures is atleast numAddedTreasures
	if(numTreasures < minNumTreasures)
	{
		//need to add treasures to get us to the min number, randomly replace non treasure cards with coppers
		//to make up the difference
		for(i=0; i < minNumTreasures - numTreasures;i++)
		{
			int randomPosition = rand() % randomDeckCount + 1;
			if(G->deck[CUR_PLAYER][randomPosition] == copper || G->deck[CUR_PLAYER][randomPosition] == silver || G->deck[CUR_PLAYER][randomPosition] ==  gold){
				i--;
			}
			else
			{
				numTreasures++;
				G->deck[CUR_PLAYER][randomPosition] = copper;
			}
		}
	}
	
	//Setup players discard, here want to randomize so that 75% of time no Treasures in discard
	int addTreasures = rand()% 3;
	for (i = 0; i < G->discardCount[CUR_PLAYER]; i++) {
		randomCard = rand() % treasure_map + 1;
		//check supply for card availability
		if (G->supplyCount[randomCard] > -1){
			//only add Treasures 25% of time
			if(addTreasures < 3)
			{
				if(randomCard == copper || randomCard == silver || randomCard ==  gold){
					i--;
				}
			}
			else{
				//enough in supply so add it
				G->discard[CUR_PLAYER][i] = randomCard;
			}
		}
		else{
			//try again, not enough cards
			i--;
		}
	}
	return numTreasures;
}

void test_Adventurer()
{
	struct gameState G, testG;
	int testFailed = 0;
	int numTreasures, numTreasuresShoudFind = 0;
	int numTestFailures = 0;
	int run, assertResult;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int treasuresInHandAfterPlay = 0, treasuresInHandBeforePlay = 0;
	int treasuresInDiscardAfterPlay = 0, treasuresInDiscardBeforePlay = 0;

	printf("\n\n------------------Adventurer Start Random Tests-------------------------------------------\n");
	//iterate over number of test runs
	for (run = 0; run < NUMTESTS; run++) {

		//create a random game state
		numTreasures = CreateRandomGameState(&G);
		
		if(numTreasures == 0)
		{
			numTreasuresShoudFind = 0;
		}else if (numTreasures == 1)
		{
			numTreasuresShoudFind = 1;
		}else if (numTreasures > 1)
		{
			numTreasuresShoudFind = 2;
		}

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		//print game state
    	printf("------------------Adventurer Start Random Test: %d seed: %d, Number of Treasure cards: %d-------\n\n", run+1, seed, numTreasures);
		printf("Number of cards in hand before play: %d \n", testG.handCount[CUR_PLAYER]);
		
		treasuresInHandBeforePlay = CountTreasureInHand(&testG, CUR_PLAYER);
		treasuresInDiscardBeforePlay = CountTreasureInDiscard(&testG, CUR_PLAYER);

		//play the adventurer card
		cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

		//test the results of card effect
		//	want to test the following:
		//		- players handcount remains the same if only one treasure card in deck, otherwise should increase by one
		//		- the hand contains the treasure cards found
		//		- the deck count plus discard count should decrease by the number of treasure cards found
		//		- adventurer should be in playedCards
		//		- number of treasures in hand should be 2 in there are at least two in deck

		treasuresInHandAfterPlay = CountTreasureInHand(&testG, CUR_PLAYER);
		treasuresInDiscardAfterPlay = CountTreasureInDiscard(&testG, CUR_PLAYER);
		/* With deck containing only 1 treasure card hand should only get 1 additional card and subtract one because the Adventurer card should be discarded */
		if(numTreasures == 1)
		{
			//check the discard because if only one in deck will look through discard
			if(treasuresInDiscardBeforePlay > 0)
			{
				assertResult = unitTestAssert("With deck plus discard containing more than one treasure cards, player's hand count should increase by one",
				(testG.handCount[CUR_PLAYER] == G.handCount[CUR_PLAYER] + 2 - 1), 0);
				if(assertResult == 0)
				{
					printf("Player's hand count is %d and it should be %d: \n", testG.handCount[CUR_PLAYER], G.handCount[CUR_PLAYER]+1);
					printf("Number of treasure cards in deck: %d \n", numTreasures);
					printf("Number of treasure cards in hand before play: %d \n", treasuresInHandBeforePlay);
					printf("Number of treasure cards in hand after play: %d \n", treasuresInHandAfterPlay);
					printf("Number of treasure cards in discard before play: %d \n", treasuresInDiscardBeforePlay);
					numTestFailures++;
				}
			}
			else
			{
				assertResult = unitTestAssert("With deck containing only 1 treasure card, player's hand count should stay the same",
				(testG.handCount[CUR_PLAYER] == G.handCount[CUR_PLAYER] + 1 - 1), 0);
				if(assertResult == 0)
				{
					printf("Player's hand count is %d and it should be %d: \n", testG.handCount[CUR_PLAYER], G.handCount[CUR_PLAYER]);
					printf("Number of treasure cards in deck before play: %d \n", numTreasures);
					printf("Number of treasure cards in discard before play: %d \n", treasuresInDiscardBeforePlay);
					printf("Number of treasure cards in hand before play: %d \n", treasuresInHandBeforePlay);
					printf("Number of treasure cards in hand after play: %d \n", treasuresInHandAfterPlay);
					numTestFailures++;
				}
			}
		}

		/* With deck containing only 1 treasure card hand should only contain on treasure  */
		if(numTreasures == 1)
		{
			//check the discard because if only one in deck will look through discard
			if(treasuresInDiscardBeforePlay > 0)
			{
				assertResult = unitTestAssert("Incorrect number of Treasure cards drawn - with deck containing more than one treasure card, player's hand should contain the two treasure cards",
				(treasuresInHandAfterPlay == treasuresInHandBeforePlay + 2), 0);
				if(assertResult == 0)
				{
					printf("Number of treasure cards in deck before play: %d \n", numTreasures);
					printf("Number of treasure cards in hand before play: %d \n", treasuresInHandBeforePlay);
					printf("Number of treasure cards in hand after play: %d \n", treasuresInHandAfterPlay);
					printf("Number of treasure cards in discard before play: %d \n", treasuresInDiscardBeforePlay);
					printHand(&testG, CUR_PLAYER);
					numTestFailures++;
				}
			}
			else
			{
				assertResult = unitTestAssert("Incorrect number of Treasure cards drawn - with deck containing only 1 treasure card, player's hand should only contain the single treasure card",
				(treasuresInHandAfterPlay == treasuresInHandBeforePlay + 1), 0);
				if(assertResult == 0)
				{
					printf("Number of treasure cards in deck before play: %d \n", numTreasures);
					printf("Number of treasure cards in hand before play: %d \n", treasuresInHandBeforePlay);
					printf("Number of treasure cards in hand after play: %d \n", treasuresInHandAfterPlay);
					printf("Number of treasure cards in discard before play: %d \n", treasuresInDiscardBeforePlay);
					printHand(&testG, CUR_PLAYER);
					numTestFailures++;
				}
			}
		}

		/* With deck containing 2 or greater treasure card hand should only get 2 additional cards and subtract one because the Adventurer card should be discarded */
		if(numTreasures > 1)
		{
			assertResult = unitTestAssert("With deck containing more than one treasure cards, player's hand count should increase by one",
			(testG.handCount[CUR_PLAYER] == G.handCount[CUR_PLAYER] + 2 - 1), 0);
			if(assertResult == 0)
			{
				printf("Number of treasure cards in deck: %d \n", numTreasures);
				printf("Number of treasure cards in hand before play: %d \n", treasuresInHandBeforePlay);
				printf("Number of treasure cards in hand after play: %d \n", treasuresInHandAfterPlay);
				printf("Player's hand count is %d and it should be %d: \n", testG.handCount[CUR_PLAYER], G.handCount[CUR_PLAYER]+1);
				numTestFailures++;
			}
		}

		/* With deck containing 2 or greater treasure card treasure cards hand should only contain two treasures  */
		if(numTreasures > 1)
		{
			assertResult = unitTestAssert("Incorrect number of Treasure cards drawn - with deck containing more than one treasure card, player's hand should contain the two treasure cards",
			(treasuresInHandAfterPlay == treasuresInHandBeforePlay + 2), 0);
			if(assertResult == 0)
			{
				printf("Number of treasure cards in deck before play: %d \n", numTreasures);
				printf("Number of treasure cards in hand before play: %d \n", treasuresInHandBeforePlay);
				printf("Number of treasure cards in hand after play: %d \n", treasuresInHandAfterPlay);
				printHand(&testG, CUR_PLAYER);
				numTestFailures++;
			}
		}

		/* Deck count plus discard count should decrease by the number of treasure cards found minus card played */
		assertResult = unitTestAssert("Deck count plus discard count should decrease by the number of treasure cards found",
			(testG.deckCount[CUR_PLAYER] + testG.discardCount[CUR_PLAYER] == G.deckCount[CUR_PLAYER] + G.discardCount[CUR_PLAYER] - numTreasuresShoudFind), 0
		);
		if(assertResult == 0)
		{
			int t_deckplusdiscard = testG.deckCount[CUR_PLAYER] + testG.discardCount[CUR_PLAYER];
			int g_deckplusdiscardminustreasure = G.deckCount[CUR_PLAYER] + G.discardCount[CUR_PLAYER] - numTreasuresShoudFind;
			printf("Actual Player's deck count %d plus discard count %d = %d \n", testG.deckCount[CUR_PLAYER], testG.discardCount[CUR_PLAYER], t_deckplusdiscard);
			printf("Ideal Player's deck count %d plus discard count %d minus treasure count %d = %d \n", G.deckCount[CUR_PLAYER], G.discardCount[CUR_PLAYER], numTreasuresShoudFind, g_deckplusdiscardminustreasure);
			numTestFailures++;
		}

		/* Should move played card to playedCards */
		assertResult = unitTestAssert("Should move adventurer card to playedCards",
			hasCard(adventurer, testG.playedCards, testG.playedCardCount), 0
		);
		if(assertResult == 0)
		{
			numTestFailures++;
		}

		printf("------------------Adventurer End Random Test: %d, Number of Tests Failed: %d -------------------------------------------\n\n", run, numTestFailures);
	}


}

int main() {
  time_t t;
  int seedx = (unsigned) time(&t);
  srand(seedx);
  test_Adventurer();
  return 0;
}

