IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

CC=clang
DEFINES=-DPDEGREE=4
CFLAGS= -I$(IDIR)

LIBS= -lm

_DEPS = sudoSolvUtils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sudoSolv.o sudoSolvUtils.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

sudoSolv: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS) $(DEFINES)

debug: CFLAGS= -g -I$(IDIR)
debug: sudoSolv 

test: sudoSolv
test: $(TDIR)/run_test

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ sudoSolv core $(INCDIR)/*~ 
