CFLAGS  := -std=c99 -Wall -I/opt/local/include/postgresql90 -I/usr/local/include
LDFLAGS := -lpcre -L/opt/local/lib/postgresql90 -L/usr/local/lib -lecpg -lpq
ECPG    := ecpg

all: pcretest ecpgtest

store.c: store.pgc
	$(ECPG) $< -o $@

depend: .depend
.depend: $(SOURCES)
	$(CC) -E -MM $^ > .depend

include .depend

pcretest: pcretest.o
	$(CC) $(LDFLAGS) $^ -o $@

ecpgtest: ecpgtest.o store.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o store.c ecpgtest pcretest