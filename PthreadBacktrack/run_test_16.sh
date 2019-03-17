#!/bin/sh

sn=`hostname`
./bin/sudoSolv 100 ../test/puzzle_data/16_degree_puzzle.pz >~/$sn.pthreadbt_16_try1.out
./bin/sudoSolv 100 ../test/puzzle_data/16_degree_puzzle.pz >~/$sn.pthreadbt_16_try2.out
./bin/sudoSolv 100 ../test/puzzle_data/16_degree_puzzle.pz >~/$sn.pthreadbt_16_try3.out
