#!/bin/bash


mpirun --map-by ppr:4:node -n 16  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz >$sn.mpibt_16x4_try1.out
mpirun --map-by ppr:3:node -n 16  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz >$sn.mpibt_16x3_try1.out
mpirun --map-by ppr:4:node -n 16  -hostfile mpi_hosts_no1 ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz >$sn.mpibt_16x4no1_try1.out


for i in {1..99}
do
    mpirun --map-by ppr:4:node -n 16  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz >>$sn.mpibt_16x4_try1.out
    mpirun --map-by ppr:3:node -n 16  -hostfile mpi_hosts ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz >>$sn.mpibt_16x3_try1.out
    mpirun --map-by ppr:4:node -n 16  -hostfile mpi_hosts_no1 ./bin/sudoMPISolv 1 ../test/puzzle_data/16_degree_puzzle.pz >>$sn.mpibt_16x4no1_try1.out
done
