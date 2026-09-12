#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa4"
rm -f List.o
gcc -c -std=c17 -Wall -g "$RELATIVE_PATH/"ModelListTest.c List.c
gcc -o ModelListTest ModelListTest.o List.o
testspassed=0
timeout 15 /usr/bin/time -o listtime.txt -f "%U" ./ModelListTest -v > ListTest-out.txt 2> /dev/null
t=$?
userTime=$(cat listtime.txt)
tooSlow=$(echo "$userTime > 5" |bc -l)
if [ ! $tooSlow -eq 1 ] && [ $t -eq 0 ]; then
  ((testspassed++))
fi
timeout 15 valgrind --leak-check=full -v ./ModelListTest > /dev/null 2> ListTest-mem.txt
if [ $? -eq 0 ]; then
  bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' ListTest-mem.txt`
  if [ ${bytes//,/} -eq 0 ]; then
    ((testspassed++))
  fi
fi
exit $((2-$testspassed))
