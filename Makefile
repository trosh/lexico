CC := gcc
RM := rm -f

.PHONY: all
all: lexico.o decoupe

lexico.o: lexico.c lexico.h
	$(CC) -c $<

decoupe: decoupe.c lexico.h lexico.o
	$(CC) -o $@ lexico.o $<

.PHONY: clean
clean:
	$(RM) lexico.o decoupe
