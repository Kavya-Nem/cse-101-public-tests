#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa4"
NUMTESTS=5
TIME=5
RED='\033[0;31m'
NC='\033[0m' # No Color

gcc -c -Wall -std=c17 -g Sparse.c Matrix.c List.c
gcc -o Sparse Sparse.o Matrix.o List.o

pathtestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let RUNTIME=$TIME
  let MAXRUNTIME=$RUNTIME*3
  if [ $NUM -eq 4 ]; then
    let RUNTIME=3*$TIME
    let MAXRUNTIME=$RUNTIME*3
  fi
  timeout $MAXRUNTIME /usr/bin/time -o time$NUM.txt -f "%U" ./Sparse "$RELATIVE_PATH/"infile$NUM.txt outfile$NUM.txt &> /dev/null
  t=$?
  userTime=$(cat time$NUM.txt)
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu outfile$NUM.txt "$RELATIVE_PATH/"model-outfile$NUM.txt &> diff$NUM.txt
  if [ -f diff$NUM.txt ] && [ ! -s "diff$NUM.txt" ] && [ ! $tooSlow -eq 1 ] && [ ! $t -eq 124 ]; then
    let pathtestspassed+=1
  fi
done

timeout $TIME valgrind --leak-check=full -v ./Sparse "$RELATIVE_PATH/"infile2.txt outfile2.txt > /dev/null 2> Sparse-mem.txt

valgrindTimeout=$?
if [ ! $pathtestspassed -eq 5 ]; then
  exit 1
fi
if [ -f Sparse-mem.txt ]; then
  bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' Sparse-mem.txt`
else
  exit 2
fi
if [ $valgrindTimeout -eq 124 ] || [ ! $bytes -eq 0 ]; then
  exit 2
fi
