
#ifndef _UNITTESTHELP_H
#define _UNITTESTHELP_H


int unitTestAssert(char condition[], int result, int print);
int isGameStateChanged(struct gameState *stateOriginal, struct gameState *stateTest);
int hasCard(int targetCard, int *arrayToSearch, int arrayCount);
int hasCardTreasure(int *arrayToSearch, int arrayCount);
int CountTreasureInHand(struct gameState * G,  int player);

/* This function simulates assert() without stopping the test suite.
   The function returns 1 if the assert is true, and 0 otherwise.
*/
int unitTestAssert(char assumption[], int result, int print) {
  // print 2 - print out both pass and fail, print 1 - print just pass, print 0 - just fail
  if (result)
  {
    if(print == 2 || print == 1){
    	printf("PASS: %s\n", assumption);
    }
    return 1;
  }
  else {
  	if(print == 2 || print == 0){
    	printf("FAIL: %s\n", assumption);
    }
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

int hasCardTreasure(int *arrayToSearch, int arrayCount)
{

	int i = 0;
	
	/* Search array for the target card */
	for (i = 0; i < arrayCount; i++) {
		if (arrayToSearch[i] == copper || arrayToSearch[i] == silver || arrayToSearch[i] == gold) {
			//card found
		  	return 1;
		}
	}
	//card not found
	return 0;
}
void printHand(struct gameState *G, int player) {
	int card;
	char cardName[50] = "";

	printf("Printing hand\n");
	for (card = 0; card < G->handCount[0]; card++) {
		cardLookup(G->hand[player][card], cardName);
		printf("Player[%d][%d] = %s\n", player, card, cardName);
	}
}

void printDeck(struct gameState *G, int player) {
	int card;
	char cardName[50] = "";

	printf("Printing hand\n");
	for (card = 0; card < G->deckCount[0]; card++) {
		cardLookup(G->deck[player][card], cardName);
		printf("Player[%d][%d] = %s\n", player, card, cardName);
	}
}

void cardLookup(int cardNum, char *cardName){

  switch(cardNum)
  {
	  case curse: 
	  	strcpy(cardName,"Curse");
	    break;
	  case estate: 
	  	strcpy(cardName,"Estate");
	    break;
	  case duchy: 
	  	strcpy(cardName,"Duchy");
	    break;
	  case province: 
	  	strcpy(cardName,"Province");
	    break;
	  case copper: 
	  	strcpy(cardName,"Copper");
	    break;
	  case silver: 
	  	strcpy(cardName,"Silver");
	    break;
	  case gold: 
	  	strcpy(cardName,"Gold");
	    break;
	  case adventurer: 
	  	strcpy(cardName,"Adventurer");
	    break;
	  case council_room: 
	  	strcpy(cardName,"Council Room");
	    break;
	  case feast: 
	  	strcpy(cardName,"Feast");
	    break;
	  case gardens: 
	  	strcpy(cardName,"Gardens");
	    break;
	  case mine: 
	  	strcpy(cardName,"Mine");
	    break;
	  case remodel: 
	  	strcpy(cardName,"Remodel");
	    break;
	  case smithy: 
	  	strcpy(cardName,"Smithy");
	    break;
	  case village: 
	  	strcpy(cardName,"Village");
	    break;
	  case baron: 
	  	strcpy(cardName,"Baron");
	    break;
	  case great_hall: 
	  	strcpy(cardName,"Great Hall");
	    break;
	  case minion: 
	  	strcpy(cardName,"Minion");
	    break;
	  case steward: 
	  	strcpy(cardName,"Steward");
	    break;
	  case tribute: 
	  	strcpy(cardName,"Tribute");
	    break;
	  case ambassador: 
	  	strcpy(cardName,"Ambassador");
	    break;
	  case cutpurse: 
	  	strcpy(cardName,"Cutpurse");
	    break;
	  case embargo: 
	  	strcpy(cardName,"Embargo");
	    break;
	  case outpost: 
	  	strcpy(cardName,"Outpost");
	    break;
	  case salvager: 
	  	strcpy(cardName,"Salvager");
	    break;
	  case sea_hag: 
	  	strcpy(cardName,"Sea Hag");
	    break;
	  case treasure_map: 
	  	strcpy(cardName,"Treasure Map");
	    break;

  	  default: 
  	  	strcpy(cardName,"Name not found");
  }

}

int CountTreasureInHand(struct gameState *G,  int player) {
	int card = 0;
	int numTreasures = 0;

	for (card = 0; card < G->handCount[player]; card++) {
		if (G->hand[player][card] == copper || G->hand[player][card] == silver || G->hand[player][card] == gold)
			numTreasures++;
	}
	
	return numTreasures;
}

int CountTreasureInDiscard(struct gameState *G,  int player) {
	int card = 0;
	int numTreasures = 0;

	for (card = 0; card < G->discardCount[player]; card++) {
		if (G->discard[player][card] == copper || G->discard[player][card] == silver || G->discard[player][card] == gold)
			numTreasures++;
	}
	
	return numTreasures;
}

#endif