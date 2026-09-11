#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa4"
TIME=8

gcc -c -std=c17 -Wall -g "$RELATIVE_PATH/"ModelMatrixTest.c Matrix.c List.c
gcc -o ModelMatrixTest ModelMatrixTest.o Matrix.o List.o

testspassed=0
timeout 20 /usr/bin/time -o time.txt -f "%U" ./ModelMatrixTest -v > MatrixTest-out.txt 2> /dev/null
t=$?
userTime=$(cat time.txt)
tooSlow=$(echo "$userTime > $TIME" |bc -l)
if [ ! $tooSlow -eq 1 ] && [ $t -eq 0 ]; then
  ((testspassed++))
fi
timeout 20 valgrind --leak-check=full -v ./ModelMatrixTest > /dev/null 2> MatrixTest-mem.txt
if [ $? -eq 0 ] && [ -f MatrixTest-mem.txt ]; then
  bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' MatrixTest-mem.txt`
  if [ ${bytes//,/} -eq 0 ]; then
    ((testspassed++))
  fi
fi

exit $((2-$testspassed))
