#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa8"

NUMTESTS=5
RUNTIME=$((${1:-1}*15))

g++ -std=c++17 -Wall -c -g Words.cpp Dictionary.cpp
g++ -std=c++17 -Wall -o Words Words.o Dictionary.o

wordstestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME /usr/bin/time -o time$NUM.txt -f "%U" ./Words "$RELATIVE_PATH/"infile$NUM.txt outfile$NUM.txt &> /dev/null
  t=$?
  userTime=$(cat time$NUM.txt)
  tooSlow=$(echo "$userTime > $RUNTIME" |bc -l)
  diff -bBwu --speed-large-files outfile$NUM.txt "$RELATIVE_PATH/"model-outfile$NUM.txt &> diff$NUM.txt
  if [ -f diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]] && [[ $tooSlow -eq 0 ]] && [[ $t -eq 0 ]]; then
    let wordstestspassed+=1
  fi
done

valgrindtestspassed=0
for NUM in $(seq 1 $NUMTESTS); do
  let MAXTIME=$RUNTIME*3
  timeout $MAXTIME valgrind --leak-check=full -v ./Words "$RELATIVE_PATH/"infile$NUM.txt outfile$NUM.txt > /dev/null 2> valgrind-out$NUM.txt
  if [ $? -eq 0 ] && [ -f valgrind-out$NUM.txt ]; then
    bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' valgrind-out$NUM.txt`
    if [ ${bytes//,/} -eq 0 ]; then
      let valgrindtestspassed+=1
    fi
  fi
done
exit $(((2*$NUMTESTS)-($wordstestspassed+$valgrindtestspassed)))

