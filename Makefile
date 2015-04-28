SHELL=/bin/bash -O extglob -c # need extglob pour purge + {a,b} ...
CC := mpicc
RM := rm -f
MPIEXEC := mpiexec
NP := 4
CFLAGS := -Iinclude -O3 -Wall -fopenmp -g
TARGETS := decoupe splitwiki char
OBJECTS := lexico.o dico.o sets.o matrix.o

.PHONY: all clean purge test 1 haiku cr log err

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

char: src/char.c
	$(CC) -o $@ $<

clean:
	$(RM) $(OBJECTS) *.log *.err

purge:
	$(RM) $(OBJECTS) $(TARGETS) *.log *.err
	$(RM) {,noac}files/!(.*) # rm tout sauf dotfiles

test: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/ex.txt
	bash noac.sh files/* # glob evite les dotfiles
	llsubmit ll.sh

1: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	bash noac.sh files/*
	llsubmit ll.sh

haiku: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/haiku.txt
	bash noac.sh files/*
	llsubmit ll.sh

haikuexec: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/haiku.txt
	bash noac.sh files/*
	mpiexec -np $(NP) ./decoupe noacfiles/*

ab: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	bash noac.sh files/Ab*
	llsubmit ll.sh

ac: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	bash noac.sh files/Ac*
	llsubmit ll.sh

acexec: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt
	bash noac.sh files/Ac*
	mpiexec -np $(NP) ./decoupe noacfiles/*

cr:
	pdflatex cr/cr.tex
	#groff -D utf-8 -e -ms cr/cr.ms | ps2pdf - > cr/cr.pdf
	evince cr/cr.pdf 2> /dev/null &

log:
	less -R lex.log

err:
	less -R lex.err
