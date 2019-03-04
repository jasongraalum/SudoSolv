.PHONY: clean debug perf

IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

CC=clang
DEFINES=
CFLAGS= -I$(IDIR)

LIBS= -lm -pthread -g

all: sudoSolv

_DEPS = sudoSolvUtils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sudoSolv.o sudoSolvUtils.o sudoSolvers.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

sudoSolv: $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)

run: sudoSolv $(OBJ)
	./bin/sudoSolv 1 ../test/puzzle_data/large_expert.pz

clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoSolv core $(INCDIR)/*~ 
