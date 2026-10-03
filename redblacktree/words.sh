#!/bin/bash
RELATIVE_PATH="../low-latency-data-structures-tests/redblacktree"

NUMTESTS=5
RUNTIME=$((${1:-1}*15))
wordstestspassed=0

if g++ -std=c++17 -Wall -c -g Words.cpp Dictionary.cpp; then
  ((wordstestspassed++))
fi
if g++ -std=c++17 -Wall -o Words Words.o Dictionary.o; then
  ((wordstestspassed++))
fi

for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME /usr/bin/time -o time$NUM.txt -f "%U" ./Words "$RELATIVE_PATH/"infile$NUM.txt outfile$NUM.txt &> /dev/null
  t=$?
  if [ -f time$NUM.txt ]; then
    userTime=$(cat time$NUM.txt)
  else
    userTime=$((RUNTIME+1))
  fi
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu --speed-large-files outfile$NUM.txt "$RELATIVE_PATH/"model-outfile$NUM.txt &> diff$NUM.txt
  if [[ -f "diff$NUM.txt" ]] && [[ ! -s "diff$NUM.txt" ]] && [[ $tooSlow -eq 0 ]] && [[ $t -eq 0 ]]; then
    let wordstestspassed+=1
  fi
done

valgrindtestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME valgrind --leak-check=full --error-exitcode=2 -v ./Words "$RELATIVE_PATH/"infile$NUM.txt outfile$NUM.txt > /dev/null 2> valgrind-out$NUM.txt
  if [ $? -eq 0 ]; then
    let valgrindtestspassed+=1
  fi
done
exit $((2*NUMTESTS+2-wordstestspassed-valgrindtestspassed))
