#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa5"
NUMTESTS=5
PNTSPERTEST=3
let MAXPTS=$NUMTESTS*$PNTSPERTEST

echo ""
echo ""

gcc -std=c17 -Wall -c -g WordFrequency.c Dictionary.c
gcc -std=c17 -Wall -o WordFrequency WordFrequency.o Dictionary.o -lm

lextestspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout while reserving stderr for valgrind output"
echo "WordFreq tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f out$NUM.txt
  timeout 5 valgrind --leak-check=full -v ./WordFrequency "$RELATIVE_PATH/"infile$NUM.txt out$NUM.txt 2> valgrind-out$NUM.txt
  if [ $? -eq 124 ]; then
    echo -e "${RED} WORDFREQ TEST TIMED OUT ${NC}"
  fi
  diff -bBwu out$NUM.txt "$RELATIVE_PATH/"model-outfile$NUM.txt &> diff$NUM.txt >> diff$NUM.txt
  echo "WordFreq Test $NUM: (Press enter to continue...)"
  read verbose
  echo "=========="
  cat diff$NUM.txt
  echo "=========="
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    let lextestspassed+=1
  fi
done

let lextestpoints=${PNTSPERTEST}*lextestspassed

echo "Passed $lextestspassed / $NUMTESTS WordFreq tests"
echo "This gives a total of $lextestpoints / $MAXPTS points"
echo ""
echo ""

echo "Press Enter To Continue with Valgrind Results for WordFreq"
echo "The valgrind report will only show the number of leaks and errors. For a detail report, please negivate to the corresponding output valgrind-out#.txt"
#TODO find a way to automate detecting if leaks and errors are found and how many
read garbage

for NUM in $(seq 1 $NUMTESTS); do
   echo "WordFreq Valgrind Test $NUM: (press enter to continue...)"
   read verbose
   echo "=========="
   cat valgrind-out$NUM.txt
   echo "=========="
done

echo ""
echo ""
rm -f *.o WordFrequency

