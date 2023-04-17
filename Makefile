IDIR =include
CC=gcc
CFLAGS=-I $(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm

_DEPS = schedule.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o schedule.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: old/src $(DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

bin/schedule_lnx: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 