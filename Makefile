SHELL=/bin/bash -O extglob -c # need extglob pour purge + {a,b} ...
CC := gcc
RM := rm -f
CFLAGS := -Iinclude
TARGETS := decoupe splitwiki noac char
OBJECTS := lexico.o dico.o

.PHONY: all clean purge test

all: $(OBJECTS) $(TARGETS)

lexico.o: src/lexico.c include/lexico.h
	$(CC) $(CFLAGS) -c $<

dico.o: src/dico.c include/lexico.h include/dico.h
	$(CC) $(CFLAGS) -c $<

decoupe: src/decoupe.c include/lexico.h include/dico.h lexico.o dico.o
	$(CC) $(CFLAGS) -o $@ lexico.o dico.o $<

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

1: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	./noac files/*
	./decoupe noacfiles/*
