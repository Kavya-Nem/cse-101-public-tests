#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa8"

NUMTESTS=3
RUNTIME=$((${1:-1}*10))

rm -f Dictionary.o
g++ -std=c++17 -Wall -c -g WordFrequency.cpp Dictionary.cpp
g++ -std=c++17 -Wall -o WordFrequency WordFrequency.o Dictionary.o

lextestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME /usr/bin/time -o WF-time$NUM.txt -f "%U" ./WordFrequency "$RELATIVE_PATH/"WF-infile$NUM.txt WF-outfile$NUM.txt &> /dev/null
  t=$?
  userTime=$(cat WF-time$NUM.txt)
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu --speed-large-files WF-outfile$NUM.txt "$RELATIVE_PATH/"Model-WF-outfile$NUM.txt &> WF-diff$NUM.txt
  if [[ ! -s WF-diff$NUM.txt ]] && [[ $tooSlow -eq 0 ]] && [[ $t -eq 0 ]]; then
    let lextestspassed+=1
  fi
done

valgrindtestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME valgrind --leak-check=full -v ./WordFrequency "$RELATIVE_PATH/"WF-infile$NUM.txt WF-outfile$NUM.txt > /dev/null 2> valgrind-out-WF$NUM.txt
  if [ $? -eq 0 ]; then
    bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' valgrind-out-WF$NUM.txt`
    if [ ${bytes//,/} -eq 0 ]; then
      let valgrindtestspassed+=1
    fi
  fi
done

exit $(((2*$NUMTESTS)-($lextestspassed+$valgrindtestspassed)))
