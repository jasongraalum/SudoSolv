.PHONY: clean debug perf

IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

CC=clang
DEFINES=-DPDEGREE=4
CFLAGS= -I$(IDIR)
SUFFIX=

LIBS= -lm


all: sudoSolv

debug: CFLAGS= -g -I$(IDIR)
debug: SUFFIX=_debug
debug: clean_debug
debug: sudoSolv 

perf: CFLAGS= -O2 -fprofile-instr-generate -I$(IDIR)
perf: SUFFIX=_perf
perf: clean_perf
perf: sudoSolv 


_DEPS = sudoSolvUtils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sudoSolv.o$(SUFFIX) sudoSolvUtils.o$(SUFFIX) sudoSolvers.o$(SUFFIX)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o$(SUFFIX): $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@$(SUFFIX) $< $(CFLAGS) $(DEFINES)

sudoSolv: $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)

test: sudoSolv
test: $(TDIR)/run_test


clean: clean_debug
clean: clean_perf
clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoSolv core $(INCDIR)/*~ 
clean_debug:
	rm -f $(ODIR)/*.o_debug *~ $(BDIR)/sudoSolv_debug core $(INCDIR)/*~ 
clean_perf:
	rm -f $(ODIR)/*.o_perf *~ $(BDIR)/sudoSolv_perf core $(INCDIR)/*~ 
