.PHONY: clean debug perf

IDIR=./include
BDIR=./
SDIR=./
ODIR=./
LDIR=lib

OMPI_CC=clang
CC=mpicc
MPI_EX=mpirun --map-by node -n 24 -hostfile mpi_hosts
DEFINES=
CFLAGS= -g -I$(IDIR)

LIBS= -lm

all: mpi_loop


_OBJ = mpi_loop.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

mpi_loop: $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)

run: mpi_loop $(OBJ)
	$(MPI_EX) ./mpi_loop 1000000 100 ../test/puzzle_data/large_expert.pz

runblock: mpi_loop $(OBJ)
	$(MPI_EX) ./mpi_loop 1000000 100 0 ../test/puzzle_data/large_expert.pz

clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/mpi_loop core $(INCDIR)/*~ 
