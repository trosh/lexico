SHELL=/bin/bash -O extglob -c # need extglob pour purge + {a,b} ...
CC := gcc
RM := rm -f
CFLAGS := -Iinclude -g
TARGETS := decoupe splitwiki noac char
OBJECTS := lexico.o

.PHONY: all clean purge test

all: $(OBJECTS) $(TARGETS)

lexico.o: src/lexico.c include/lexico.h
	$(CC) $(CFLAGS) -c $<

decoupe: src/decoupe.c include/lexico.h lexico.o
	$(CC) $(CFLAGS) -o $@ lexico.o $<

splitwiki: src/splitwiki.c
	$(CC) -o $@ $<

noac: src/noac.c
	$(CC) -o $@ $< -lunac

char: src/char.c
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
