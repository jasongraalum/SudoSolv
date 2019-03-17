#!/bin/sh

sn=`hostname`

make run16no1 >~/$sn.mpibt_16no1_try1.out
make run16x3 >~/$sn.mpibt_16x3_try1.out
make run16x4 >~/$sn.mpibt_16x4_try1.out

for i in {1..99}
do
make run16no1 >>~/$sn.mpibt_16no1_try1.out
make run16x3 >>~/$sn.mpibt_16x3_try1.out
make run16x4 >>~/$sn.mpibt_16x4_try1.out
done