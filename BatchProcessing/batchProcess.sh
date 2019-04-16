#! /bin/sh


beg=$67
end=$112
inc=$1

for ((fileNum=beg; fileNum <= end; fileNum+=inc)); do  
f=$(printf '/data/phenix/data/TestBeam2018/run/1/run_00000%d-0000.prdf' $fileNum)
root -b -q 'ProcessRawData.C("'$f'")'
#sleep 40
done
