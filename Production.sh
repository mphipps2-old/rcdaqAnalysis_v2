#! /bin/sh

beg=$1
end=$2

for ((fileNum=beg; fileNum <= end; fileNum+=1)); do  
f=$(printf '/home/beamtest/Desktop/data/TestBeam2018/run/3/run_00000%d-0000.prdf' $fileNum)
root -b -q 'singleRunProduction.C("'$f'")'
#sleep 40
done
