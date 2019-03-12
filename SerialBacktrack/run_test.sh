#!/bin/sh

sn=`hostname`
./bin/sudoSolv 100 ../test/puzzle_data/large_1_filled_cell.pz >~/$sn.serialbt_try1.out
./bin/sudoSolv 100 ../test/puzzle_data/large_1_filled_cell.pz >~/$sn.serialbt_try2.out
./bin/sudoSolv 100 ../test/puzzle_data/large_1_filled_cell.pz >~/$sn.serialbt_try3.out
