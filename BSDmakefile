PCRE_CFLAGS 	  	!= pcre-config --cflags
PCRE_LDFLAGS 		!= pcre-config --libs

POSTGRES_CFLAGS 	!= echo -I`pg_config --includedir`
POSTGRES_LDFLAGS 	!= echo -L`pg_config --libdir` -lecpg -lpq -pthread

CFLAGS			= $(PCRE_CFLAGS) $(POSTGRES_CFLAGS) -Wall -std=c99

all: extractor

include .depend

.depend: extract.c extract.h store.h store.c
	$(CC) $(CFLAGS) -E -MM $(.ALLSRC) > $(.TARGET)

.SUFFIXES: .c .o .pgc

.pgc.c:
	ecpg $(.ALLSRC) -o $(.TARGET)

extractor: extract.o store.o main.o
	$(CC) $(PCRE_LDFLAGS) $(POSTGRES_LDFLAGS) $(.ALLSRC) -o $(.TARGET)

clean:
	rm -f *.o store.c extractor

.PHONY: clean