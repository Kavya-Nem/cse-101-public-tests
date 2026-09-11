#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa4"

gcc -c -std=c17 -Wall -g "$RELATIVE_PATH/"ModelListTest.c List.c
gcc -o ModelListTest ModelListTest.o List.o

timeout 5 ./ModelListTest -v > ListTest-out.txt

exit $?
