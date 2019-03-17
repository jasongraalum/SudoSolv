.PHONY: clean debug perf

IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

CC=mpicc
DEFINES=
CFLAGS= -I$(IDIR)

LIBS= -lm 

all: sudoMPISolv

_DEPS = sudoMPISolvUtils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sudoMPISolv.o sudoMPISolvUtils.o sudoMPISolvers.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

sudoMPISolv: $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)

run: sudoMPISolv $(OBJ)
	mpirun --map-by ppr:3:node -n 9  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/large_expert.pz

run1: sudoMPISolv $(OBJ)
	mpirun --map-by ppr:9:node -n 2  -hostfile mpi_hosts1 ./bin/sudoMPISolv 1 ../test/puzzle_data/large_expert.pz

run16x4: sudoMPISolv $(OBJ)
	mpirun --map-by ppr:4:node -n 16  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz

run16x3: sudoMPISolv $(OBJ)
	mpirun --map-by ppr:3:node -n 16  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz

run16no1: sudoMPISolv $(OBJ)
	mpirun --map-by ppr:4:node -n 16  -hostfile mpi_hosts_no1 ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz

run25: sudoMPISolv $(OBJ)
	mpirun --map-by ppr:4:node -n 25  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/25_degree_puzzle.pz

clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoMPISolv core $(INCDIR)/*~ 
