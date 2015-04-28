SHELL=/bin/bash -O extglob -c # need extglob pour purge + {a,b} ...
CC := mpicc
RM := rm -f
MPIEXEC := mpiexec
NP := 4
CFLAGS := -Iinclude -O3 -Wall -fopenmp -g
TARGETS := decoupe splitwiki char
DATTARGETS := datdecoupe splitwiki char
OBJECTS := lexico.o dico.o sets.o matrix.o

.PHONY: all clean purge test 1 haiku cr log err

all: $(OBJECTS) $(TARGETS)

lexico.o: src/lexico.c include/lexico.h
	$(CC) $(CFLAGS) -c $<

dico.o: src/dico.c include/lexico.h include/dico.h
	$(CC) $(CFLAGS) -c $<

sets.o: src/sets.c include/dico.h include/sets.h
	$(CC) $(CFLAGS) -c -o $@ $<

matrix.o: src/matrix.c include/sets.h include/dico.h include/matrix.h
	$(CC) $(CFLAGS) -c -o $@ $<

datmatrix.o: src/matrix.c include/sets.h include/dico.h include/matrix.h
	$(CC) $(CFLAGS) -DDAT -c -o $@ $<

decoupe: src/decoupe.c lexico.o dico.o sets.o matrix.o
	$(CC) $(CFLAGS) -o $@ lexico.o dico.o sets.o matrix.o $<

datdecoupe: src/decoupe.c lexico.o dico.o sets.o datmatrix.o
	$(CC) $(CFLAGS) -DDAT -o $@ lexico.o dico.o sets.o datmatrix.o $<

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
	./splitwiki wiki/ex.txt > splitwiki.log
	bash noac.sh files/* > noac.log # glob evite les dotfiles
	llsubmit ll.sh

1: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt > splitwiki.log
	bash noac.sh files/* > noac.log
	llsubmit ll.sh

haiku: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/haiku.txt > splitwiki.log
	bash noac.sh files/* > noac.log
	llsubmit ll.sh

haikuexec: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/haiku.txt > splitwiki.log
	bash noac.sh files/* > noac.log
	mpiexec -np $(NP) ./decoupe noacfiles/*

ab: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt > splitwiki.log
	bash noac.sh files/Ab* > noac.log
	llsubmit ll.sh

ac: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt > splitwiki.log
	bash noac.sh files/Ac* > noac.log
	llsubmit ll.sh

acompscale: $(DATTARGETS)
	$(RM) {noac,}files/!(.*)
	$(RM) dat/!(.*)
	./splitwiki wiki/1.txt > splitwiki.log
	bash noac.sh files/Ac* > noac.log
	llsubmit llomp1.sh
	llsubmit llomp2.sh
	llsubmit llomp4.sh
	llsubmit llomp8.sh
	llsubmit llomp16.sh
	llsubmit llomp32.sh

acexec: $(TARGETS)
	$(RM) {noac,}files/!(.*)
	./splitwiki wiki/1.txt > splitwiki.log
	bash noac.sh files/Ac* > noac.log
	mpiexec -np $(NP) ./decoupe noacfiles/*

acexecompscale: $(DATTARGETS)
	$(RM) {noac,}files/!(.*)
	$(RM) dat/!(.*)
	./splitwiki wiki/1.txt > splitwiki.log
	bash noac.sh files/Ac* > noac.log
	OMP_NUM_THREADS=1  mpiexec -np 1 ./datdecoupe noacfiles/*
	OMP_NUM_THREADS=2  mpiexec -np 1 ./datdecoupe noacfiles/*
	OMP_NUM_THREADS=4  mpiexec -np 1 ./datdecoupe noacfiles/*
	OMP_NUM_THREADS=8  mpiexec -np 1 ./datdecoupe noacfiles/*
	OMP_NUM_THREADS=16 mpiexec -np 1 ./datdecoupe noacfiles/*
	OMP_NUM_THREADS=32 mpiexec -np 1 ./datdecoupe noacfiles/*

cr:
	pdflatex cr/cr.tex
	#groff -D utf-8 -e -ms cr/cr.ms | ps2pdf - > cr/cr.pdf
	evince cr/cr.pdf 2> /dev/null &

log:
	less -R lex.log

err:
	less -R lex.err
