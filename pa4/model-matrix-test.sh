#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/sparse"
TIME=8
rm -f Matrix.o List.o
testspassed=0
if gcc -c -std=c17 -Wall -g "$RELATIVE_PATH/"ModelMatrixTest.c Matrix.c List.c; then
  ((testspassed++))
fi
if gcc -o ModelMatrixTest ModelMatrixTest.o Matrix.o List.o; then
  ((testspassed++))
fi
timeout 20 /usr/bin/time -o time.txt -f "%U" ./ModelMatrixTest -v > MatrixTest-out.txt 2> /dev/null
t=$?
if [ -f time.txt ]; then
  userTime=$(cat time.txt)
else
  userTime=$((TIME+1))
fi
tooSlow=$(echo "$userTime > $TIME" |bc -l)
if [ $tooSlow -eq 0 ] && [ $t -eq 0 ]; then
  ((testspassed++))
fi
timeout 20 valgrind --error-exitcode=2 --leak-check=full -v ./ModelMatrixTest > /dev/null 2> MatrixTest-mem.txt
if [ $? -eq 0 ]; then
  ((testspassed++))
fi
exit $((4-testspassed))
