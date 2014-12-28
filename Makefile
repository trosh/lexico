SHELL=/bin/bash -O extglob -c # need extglob pour purge + {a,b} ...
CC := gcc
RM := rm -f
TARGETS := decoupe splitwiki noac char
OBJECTS := lexico.o

.PHONY: all clean purge test

all: $(OBJECTS) $(TARGETS)

lexico.o: lexico.c lexico.h
	$(CC) -c $< -g

decoupe: decoupe.c lexico.h lexico.o
	$(CC) -o $@ lexico.o $< -g

splitwiki: splitwiki.c
	$(CC) -o $@ $<

noac: noac.c
	$(CC) -o $@ $< -lunac

char: char.c
	$(CC) -o $@ $<

clean:
	$(RM) $(OBJECTS)

purge:
	$(RM) $(OBJECTS) $(TARGETS)
	$(RM) {,noac}files/!(.*) # rm tout sauf dotfiles

test: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/ex.txt
	./noac files/* # glob evite les dotfiles
	./decoupe noacfiles/*
