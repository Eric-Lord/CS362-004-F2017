CFLAGS = -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

all: randomtestadventurer randomtestcard1 randomtestcard2 randomtestresults.out

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

clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so *.out randomtestadventurer randomtestcard1 randomtestcard2 randomtestresults.out randomtestadventurer.out randomtestcard1.out randomtestcard2.out
