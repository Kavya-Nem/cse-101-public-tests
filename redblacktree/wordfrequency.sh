#!/bin/bash
RELATIVE_PATH="../low-latency-data-structures-tests/redblacktree"

NUMTESTS=3
RUNTIME=$((${1:-1}*10))
lextestspassed=0

rm -f Dictionary.o
if g++ -std=c++17 -Wall -c -g WordFrequency.cpp Dictionary.cpp; then
  ((lextestspassed++));
fi
if g++ -std=c++17 -Wall -o WordFrequency WordFrequency.o Dictionary.o; then
  ((lextestspassed++));
fi

for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME /usr/bin/time -o WF-time$NUM.txt -f "%U" ./WordFrequency "$RELATIVE_PATH/"WF-infile$NUM.txt WF-outfile$NUM.txt &> /dev/null
  t=$?
  if [ -f WF-time$NUM.txt ]; then
    userTime=$(cat WF-time$NUM.txt)
  else
    userTime=$((RUNTIME+1))
  fi
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu --speed-large-files WF-outfile$NUM.txt "$RELATIVE_PATH/"Model-WF-outfile$NUM.txt &> WF-diff$NUM.txt
  if [[ -f "WF-diff$NUM.txt" ]] && [[ ! -s "WF-diff$NUM.txt" ]] && [[ $tooSlow -eq 0 ]] && [[ $t -eq 0 ]]; then
    let lextestspassed+=1
  fi
done

valgrindtestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*6
  timeout $MAXTIME valgrind --leak-check=full --error-exitcode=2 -v ./WordFrequency "$RELATIVE_PATH/"WF-infile$NUM.txt WF-outfile$NUM.txt > /dev/null 2> valgrind-out-WF$NUM.txt
  if [ $? -eq 0 ]; then
    let valgrindtestspassed+=1
  fi
done

exit $((2*NUMTESTS+2-lextestspassed-valgrindtestspassed))
