.PHONY: clean debug perf

IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

OMPI_CC=clang
CC=mpicc
MPI_EX=mpirun --map-by ppr:2:node -n 2 -hostfile mpi_hosts
DEFINES=
CFLAGS= -g -I$(IDIR)

LIBS= -lm

all: sudoSolv

_DEPS = sudoSolvUtils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sudoSolv.o sudoSolvUtils.o sudoSolvers.o sudoSolverMPI.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

sudoSolv: $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)

run: sudoSolv $(OBJ)
	$(MPI_EX) ./bin/sudoSolv 1 ../test/puzzle_data/large_expert.pz

small: sudoSolv $(OBJ)
	$(MPI_EX) ./bin/sudoSolv 1 ../test/puzzle_data/test_15_gap.pz

val: sudoSolv $(OBJ)
	valgrind --leak-check=full $(MPI_EX) ./bin/sudoSolv 1 ../test/puzzle_data/test_4_gap.pz

debug: sudoSolv $(OBJ)
	gdb  ./bin/sudoSolv 1 ../test/puzzle_data/test_4_gap.pz

clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoSolv core $(INCDIR)/*~ 
