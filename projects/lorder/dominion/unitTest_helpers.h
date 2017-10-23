
#ifndef _UNITTESTHELP_H
#define _UNITTESTHELP_H


int unitTestAssert(char condition[], int result);
int isGameStateChanged(struct gameState *stateOriginal, struct gameState *stateTest);
int hasCard(int targetCard, int *arrayToSearch, int arrayCount);

/* This function simulates assert() without stopping the test suite.
   The function returns 1 if the assert is true, and 0 otherwise.
*/
int unitTestAssert(char assumption[], int result) {
  
  if (result)
  {
    printf("PASS: %s\n", assumption);
    return 1;
  }
  else {
    printf("FAIL: %s\n", assumption);
    return 0;
  }
}

int isGameStateChanged(struct gameState *stateOriginal, struct gameState *stateTest)
{
	int i, j;

	if(stateOriginal->numPlayers != stateTest->numPlayers)
	{
		return 1;
	}

	if(stateOriginal->outpostPlayed != stateTest->outpostPlayed)
	{
		return 1;
	}

	if(stateOriginal->outpostTurn != stateTest->outpostTurn)
	{
		return 1;
	}

	if(stateOriginal->whoseTurn != stateTest->whoseTurn)
	{
		return 1;
	}

	if(stateOriginal->phase != stateTest->phase)
	{
		return 1;
	}

	if(stateOriginal->numActions != stateTest->numActions)
	{
		return 1;
	}

	if(stateOriginal->coins != stateTest->coins)
	{
		return 1;
	}

	if(stateOriginal->numBuys != stateTest->numBuys)
	{
		return 1;
	}

	if(stateOriginal->playedCardCount != stateTest->playedCardCount)
	{
		return 1;
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
	    for (j = 0; j < MAX_HAND; j++) {
	      if (stateOriginal->hand[i][j] != stateTest->hand[i][j]) {
	        return 1;
	      }
	    }
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
		if (stateOriginal->handCount[i] != stateTest->handCount[i]) {
			return 1;
		}
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
	    for (j = 0; j < MAX_DECK; j++) {
	      if (stateOriginal->deck[i][j] != stateTest->deck[i][j]) {
	        return 1;
	      }
	    }
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
	    if (stateOriginal->deckCount[i] != stateTest->deckCount[i]) {
	        return 1;
	    }
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
	    if (stateOriginal->discardCount[i] != stateTest->discardCount[i]) {
	        return 1;
	    }
	}

	for (i = 0; i < MAX_PLAYERS; i++) {
	    for (j = 0; j < MAX_DECK; j++) {
	      if (stateOriginal->discard[i][j] != stateTest->discard[i][j]) {
	        return 1;
	      }
	    }
	}

	for (i = 0; i < MAX_DECK; i++) {
	    if (stateOriginal->playedCards[i] != stateTest->playedCards[i]) {
	        return 1;
	    }
	}

	for (i = 0; i < treasure_map+1; i++) 
	{
		if(stateOriginal->embargoTokens[i] != stateTest->embargoTokens[i])
		{
			return 1;
		}
  	}

  	for (i = 0; i < treasure_map+1; i++) 
	{
		if(stateOriginal->supplyCount[i] != stateTest->supplyCount[i])
		{
			return 1;
		}
  	}
	
  	return 0;
}

int hasCard(int targetCard, int *arrayToSearch, int arrayCount)
{

	int i = 0;

	/* Search array for the target card */
	for (i = 0; i < arrayCount; i++) {
		if (arrayToSearch[i] == targetCard) {
			//card found
		  	return 1;
		}
	}
	//card not found
	return 0;
}

#endif