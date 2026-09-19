#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa5"
NUMTESTS=5
RUNTIME=5
lextestspassed=0
if gcc -std=c17 -Wall -c -g WordFrequency.c Dictionary.c; then
  ((lextestspassed++))
fi
if gcc -std=c17 -Wall -o WordFrequency WordFrequency.o Dictionary.o -lm; then
  ((lextestspassed++))
fi
for NUM in $(seq 1 $NUMTESTS); do
  let MAXRUNTIME=$RUNTIME*3
  timeout $MAXRUNTIME /usr/bin/time -o time$NUM.txt -f "%U" ./WordFrequency "$RELATIVE_PATH/"infile$NUM.txt out$NUM.txt &> /dev/null
  t=$?
  if [ -f time$NUM.txt ]; then
    userTime=$(cat time$NUM.txt)
  else
    userTime=$((RUNTIME+1))
  fi
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu out$NUM.txt "$RELATIVE_PATH/"model-outfile$NUM.txt &> diff$NUM.txt
  if [ -f "diff$NUM.txt" ] && [[ ! -s "diff$NUM.txt" ]] && [ $tooSlow -eq 0 ] && [ $t -eq 0 ]; then
    let lextestspassed+=1
  fi
done

valgrindtestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXRUNTIME=$RUNTIME*3
  timeout $MAXRUNTIME valgrind --leak-check=full --error-exitcode=2 -v ./WordFrequency "$RELATIVE_PATH/"infile$NUM.txt out$NUM.txt > /dev/null 2> valgrind-out$NUM.txt
  if [ $? -eq 0 ]; then
    let valgrindtestspassed+=1
  fi
done
exit $(((2*$NUMTESTS+2)-($lextestspassed+$valgrindtestspassed)))
