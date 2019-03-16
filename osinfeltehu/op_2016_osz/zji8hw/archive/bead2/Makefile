CC=gcc

bead2: lebonyolito/main.c nyilvantarto/main.c common/linkedlist.c
	$(CC) -c common/linkedlist.c
	$(CC) -o nyilvantarto_b nyilvantarto/main.c linkedlist.o
	$(CC) -o lebonyolito_b lebonyolito/main.c linkedlist.o
	rm -rf linkedlist.o

clean:
	rm -rf nyilvantarto_b lebonyolito_b visitors.csv events.csv