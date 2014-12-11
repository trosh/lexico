CC := gcc
RM := rm -f
TARGETS := decoupe splitwiki noac
OBJECTS := lexico.o

.PHONY: all clean purge test

all: lexico.o $(TARGETS)

lexico.o: lexico.c lexico.h
	$(CC) -c $< -g

decoupe: decoupe.c lexico.h lexico.o
	$(CC) -o $@ lexico.o $< -g

splitwiki: splitwiki.c
	$(CC) -o $@ $<

noac: noac.c
	$(CC) -o $@ $< -lunac

clean:
	$(RM) $(OBJECTS)

purge:
	$(RM) $(OBJECTS) $(TARGETS) files/*.txt noacfiles/*.txt

test: $(TARGETS)
	rm -f {noac,}files/*
	./splitwiki wiki/ex.txt
	./noac files/*.txt
	./decoupe noacfiles/*.txt
