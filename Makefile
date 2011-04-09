PCRE_CFLAGS 	  	= $(shell pcre-config --cflags)
PCRE_LDFLAGS 		= $(shell pcre-config --libs)
                        
POSTGRES_CFLAGS 	= -I$(shell pg_config --includedir)
POSTGRES_LDFLAGS 	= -L$(shell pg_config --libdir) -lecpg -lpq -pthread
                        
CFLAGS			= $(PCRE_CFLAGS) $(POSTGRES_CFLAGS) -Wall -std=c99

all: extractor

include .depend

.depend: extract.c extract.h store.h store.c
	$(CC) $(CFLAGS) -E -MM $^ > $@

%.c: %.pgc
	ecpg $< -o $@

extractor: extract.o store.o main.o
	$(CC) $(PCRE_LDFLAGS) $(POSTGRES_LDFLAGS) $^ -o $@

clean:
	rm -f *.o store.c extractor

.PHONY: clean