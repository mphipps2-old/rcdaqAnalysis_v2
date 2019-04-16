#! /bin/sh


beg=388
end=391
inc=1

for ((fileNum=beg; fileNum <= end; fileNum+=inc)); do  
f=$(printf '/data/phenix/data/TestBeam2018/run/3/run_00000%d-0000.prdf' $fileNum)
root -l -b -q 'ProcessRawData.C("'$f'")'
#sleep 40
done
