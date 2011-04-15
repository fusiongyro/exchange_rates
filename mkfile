CC     = cc

PCRE_CFLAGS       = `pcre-config --cflags`
PCRE_LDFLAGS      = `pcre-config --libs`

POSTGRES_CFLAGS   = -I`pg_config --includedir`
POSTGRES_LDFLAGS  = -L`pg_config --libdir` -lecpg -lpq -pthread

CFLAGS            = $PCRE_CFLAGS $POSTGRES_CFLAGS -Wall -std=c99

all:V: extractor

<|$CC $CFLAGS -E -MM *.c *.h

%.o: %.c
  $CC $CFLAGS -c $stem.c -o $stem.o

%.c: %.pgc
  ecpg $stem.pgc -o $stem.c

extractor: extract.o store.o main.o
  $CC $PCRE_LDFLAGS $POSTGRES_LDFLAGS $prereq -o $alltarget

clean:V:
  rm -f *.o store.c extractor
