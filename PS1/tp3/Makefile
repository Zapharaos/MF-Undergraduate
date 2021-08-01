
CFLAGS = -Wall -Wextra -Wvla -Werror
CC = gcc

OBJS = delegation.o traitement.o
PROG = delegation

all: $(PROG)

$(PROG): $(OBJS)


essai:
	test -d essai-10 || mkdir essai-10 && touch $$(seq -f "essai-10/fichier%.0f.data" 0 9)
	cd essai-10 && ../$(PROG) +slow

test:
	./test.sh 1 2 3 4 5


clean:
	rm -f $(PROG) $(OBJS)
	rm -rf essai-10

# usage interne
archive:
	tar --transform='s,^,tp3/,' -c -z -f tp3.tar.gz \
		traitement.h traitement.c Makefile test.sh
