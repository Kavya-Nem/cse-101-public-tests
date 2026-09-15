#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa4"
rm -f List.o
testspassed=0
if gcc -c -std=c17 -Wall -g "$RELATIVE_PATH/"ModelListTest.c List.c; then
  ((testspassed++))
fi
if gcc -o ModelListTest ModelListTest.o List.o; then
  ((testspassed++))
fi
timeout 15 /usr/bin/time -o listtime.txt -f "%U" ./ModelListTest -v > ListTest-out.txt 2> /dev/null
t=$?
userTime=$(cat listtime.txt)
tooSlow=$(echo "$userTime > 5" |bc -l)
if [ $tooSlow -eq 0 ] && [ $t -eq 0 ]; then
  ((testspassed++))
fi
timeout 15 valgrind --error-exitcode=2 --leak-check=full -v ./ModelListTest > /dev/null 2> ListTest-mem.txt
if [ $? -eq 0 ]; then
  ((testspassed++))
fi
exit $((4-$testspassed))
