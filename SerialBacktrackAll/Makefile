.PHONY: clean debug perf

IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

CC=clang
DEFINES=
CFLAGS= -g -I$(IDIR)

LIBS= -lm

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

small: sudoSolv $(OBJ)
	./bin/sudoSolv 1 ../test/puzzle_data/test_4_gap.pz

val: sudoSolv $(OBJ)
	valgrind --leak-check=full ./bin/sudoSolv 1 ../test/puzzle_data/test_4_gap.pz

clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoSolv core $(INCDIR)/*~ 
