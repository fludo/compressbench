#!/bin/sh
mkdir results
touch results/datas.txt

for filename in `ls | grep -v compressed | grep -v decompressed | grep -v run.sh | grep -v run-dec.sh | grep -v lz78.sh | grep -v results | grep -v plot.sh | grep -v clean.sh | grep -v create-log.sh` 
do


  size=`ls -l $filename | awk '{print $5}'`

 sizelz78=`ls -l compressed/$filename.lz78 | awk '{print $5}'`
 sizebh=`ls -l compressed/$filename.bh | awk '{print $5}'`
 sizebmh=`ls -l compressed/$filename.bmh | awk '{print $5}'`
 sizebrh=`ls -l compressed/$filename.brh | awk '{print $5}'`
 sizebmrh=`ls -l compressed/$filename.bmrh | awk '{print $5}'`
 sizebrmh=`ls -l compressed/$filename.brmh | awk '{print $5}'`
  echo -e "None\t$filename\t$size\t$size" >> results/datas.txt
  echo -e "LZ78\t$filename\t$size\t$sizelz78" >> results/datas.txt
  echo -e "BH\t$filename\t$size\t$sizebh" >> results/datas.txt
  echo -e "BMH\t$filename\t$size\t$sizebmh" >> results/datas.txt
  echo -e "BRH\t$filename\t$size\t$sizebrh" >> results/datas.txt
  echo -e "BMRH\t$filename\t$size\t$sizebmrh" >> results/datas.txt
  echo -e "BRMH\t$filename\t$size\t$sizebrmh" >> results/datas.txt

  
done;
