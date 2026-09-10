#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa5"
NUMTESTS=5
RUNTIME=5

gcc -std=c17 -Wall -c -g WordFrequency.c Dictionary.c
gcc -std=c17 -Wall -o WordFrequency WordFrequency.o Dictionary.o -lm

lextestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXRUNTIME=$RUNTIME*3
  timeout $MAXRUNTIME /usr/bin/time -o time$NUM.txt -f "%U" ./WordFrequency "$RELATIVE_PATH/"infile$NUM.txt out$NUM.txt &> /dev/null
  t=$?
  userTime=$(cat time$NUM.txt)
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu out$NUM.txt "$RELATIVE_PATH/"model-outfile$NUM.txt &> diff$NUM.txt
  if [ -f diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]] && [ ! $tooSlow -eq 1 ] && [ ! $t -eq 124 ]; then
    let lextestspassed+=1
  fi
done

timeout $RUNTIME valgrind --leak-check=full -v ./WordFrequency "$RELATIVE_PATH/"infile2.txt out2.txt > /dev/null 2> valgrind-out.txt
valgrindTimeout=$?

if [ ! $lextestspassed -eq 5 ]; then
  exit 1
fi
if [ -f valgrind-out.txt ]; then
  bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' valgrind-out.txt`
else
  exit 2
fi
if [ $valgrindTimeout -eq 124 ] || [ ! $bytes -eq 0 ]; then
  exit 2
fi
exit 0
