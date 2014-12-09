CC := gcc
RM := rm -f
TARGETS := decoupe splitwiki
OBJECTS := lexico.o

.PHONY: all clean purge

all: lexico.o decoupe splitwiki

lexico.o: lexico.c lexico.h
	$(CC) -c $<

decoupe: decoupe.c lexico.h lexico.o
	$(CC) -o $@ lexico.o $<

splitwiki: splitwiki.c
	$(CC) -o $@ $<

clean:
	$(RM) $(OBJECTS)

purge:
	$(RM) $(OBJECTS) $(TARGETS) files/*.txt
