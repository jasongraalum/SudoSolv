#!/bin/sh

sn=`hostname`
mpirun --map-by ppr:4:node -n 12  -hostfile mpi_hosts ./bin/sudoMPISolv 100 ../test/puzzle_data/large_1_filled_cell.pz

