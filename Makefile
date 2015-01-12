SHELL=/bin/bash -O extglob -c # need extglob pour purge + {a,b} ...
CC := gcc
RM := rm -f
CFLAGS := -Iinclude -O3
TARGETS := decoupe splitwiki noac char
OBJECTS := lexico.o dico.o sets.o matrix.o

.PHONY: all clean purge test 1 haiku

all: $(OBJECTS) $(TARGETS)

lexico.o: src/lexico.c include/lexico.h
	$(CC) $(CFLAGS) -c $<

dico.o: src/dico.c include/lexico.h include/dico.h
	$(CC) $(CFLAGS) -c $<

sets.o: src/sets.c include/dico.h include/sets.h
	$(CC) $(CFLAGS) -c $<

matrix.o: src/matrix.c include/sets.h include/dico.h include/matrix.h
	$(CC) $(CFLAGS) -c $<

decoupe: src/decoupe.c \
         include/lexico.h include/dico.h include/sets.h include/matrix.h \
         lexico.o dico.o sets.o matrix.o
	$(CC) $(CFLAGS) -o $@ lexico.o dico.o sets.o matrix.o $<

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
	./decoupe -d noacfiles/*

1: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	./noac files/*
	./decoupe noacfiles/*

haiku: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/haiku.txt
	./noac files/*
	./decoupe -d noacfiles/*

ab: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	./noac files/Ab*
	./decoupe noacfiles/*
