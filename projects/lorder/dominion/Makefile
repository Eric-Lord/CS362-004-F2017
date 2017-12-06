CFLAGS = -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)
	
playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

all: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestadventurer randomtestcard1 randomtestcard2 randomtestresults.out

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtestresults.out: randomtestadventurer randomtestcard1 randomtestcard2
	echo "Result for running Dominion random tests:" > randomtestresults.out

	echo "randomtestadventurer.c:" >> randomtestresults.out
	./randomtestadventurer >> randomtestresults.out
	gcov -flb dominion.c >> randomtestresults.out

	echo "randomtestcard1.c:" >> randomtestresults.out
	./randomtestcard1 >> randomtestresults.out
	gcov -flb dominion.c >> randomtestresults.out

	echo "randomtestcard2.c:" >> randomtestresults.out
	./randomtestcard2 >> randomtestresults.out
	gcov -flb dominion.c >> randomtestresults.out

unittestresults.out: unittest1 unittest2 unittest3 unittest4
	echo "Result for running Dominion unit tests:" > unittestresults.out

	echo "unittest1.c:" >> unittestresults.out
	./unittest1 >> unittestresults.out
	gcov -flb dominion.c >> unittestresults.out

	echo "unittest2.c:" >> unittestresults.out
	./unittest2 >> unittestresults.out
	gcov -flb dominion.c >> unittestresults.out

	echo "unittest3.c:" >> unittestresults.out
	./unittest3 >> unittestresults.out
	gcov -flb dominion.c >> unittestresults.out

	echo "unittest4.c:" >> unittestresults.out
	./unittest4 >> unittestresults.out
	gcov -flb dominion.c >> unittestresults.out

unittest1.out: unittest1
	echo "Result for running Dominion Unit test 1:" > unittest1.out

	echo "unittest1.c:" >> unittest1.out
	./unittest1 >> unittest1.out
	gcov -fbl dominion.c >> unittest1.out

unittest2.out: unittest2
	echo "Result for running Dominion Unit test 2:" > unittest2.out

	echo "unittest2.c:" >> unittest2.out
	./unittest2 >> unittest2.out
	gcov -fbl dominion.c >> unittest2.out

unittest3.out: unittest3
	echo "Result for running Dominion Unit test 3:" > unittest3.out

	echo "unittest3.c:" >> unittest3.out
	./unittest3 >> unittest3.out
	gcov -fbl dominion.c >> unittest3.out	

unittest4.out: unittest4
	echo "Result for running Dominion Unit test 4:" > unittest4.out

	echo "unittest4.c:" >> unittest4.out
	./unittest4 >> unittest4.out
	gcov -fbl dominion.c >> unittest4.out

randomtestadventurer.out: randomtestadventurer
	echo "Result for running Dominion Adventurer random tests:" > randomtestadventurer.out

	echo "randomtestadventurer.c:" >> randomtestadventurer.out
	./randomtestadventurer >> randomtestadventurer.out
	gcov -fbl dominion.c >> randomtestadventurer.out

randomtestcard1.out: randomtestcard1
	echo "Result for running Dominion Smithy random tests:" > randomtestcard1.out

	echo "randomtestcard1.c:" >> randomtestcard1.out
	./randomtestcard1 >> randomtestcard1.out
	gcov -fbl dominion.c >> randomtestcard1.out

randomtestcard2.out: randomtestcard2
	echo "Result for running Dominion Village random tests:" > randomtestcard2.out

	echo "randomtestcard2.c:" >> randomtestcard2.out
	./randomtestcard2 >> randomtestcard2.out
	gcov -fbl dominion.c >> randomtestcard2.out

cardtestresults.out: cardtest1 cardtest2 cardtest3 cardtest4
	echo "Result for running Dominion card tests:" > cardtestresults.out

	echo "cardtest1.c:" >> cardtestresults.out
	./cardtest1 >> cardtestresults.out
	gcov -flb dominion.c >> cardtestresults.out

	echo "cardtest2.c:" >> cardtestresults.out
	./cardtest2 >> cardtestresults.out
	gcov -flb dominion.c >> cardtestresults.out

	echo "cardtest3.c:" >> cardtestresults.out
	./cardtest3 >> cardtestresults.out
	gcov -flb dominion.c >> cardtestresults.out

	echo "cardtest4.c:" >> cardtestresults.out
	./cardtest4 >> cardtestresults.out
	gcov -flb dominion.c >> cardtestresults.out

clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so *.out unittest1 unittest2 unittest3 unittest4 unittest1.out unittest2.out unittest3.out unittest4.out cardtest1 cardtest2 cardtest3 cardtest4 randomtestadventurer randomtestcard1 randomtestcard2 randomtestresults.out randomtestadventurer.out randomtestcard1.out randomtestcard2.out
