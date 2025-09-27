#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa1"
NUMTESTS=3
PNTSPERTEST=5
let MAXPTS=$NUMTESTS*$PNTSPERTEST

gcc -c -std=c17 -Wall -g Words.c List.c
gcc -o Words Words.o List.o

wordstestspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout while reserving stderr for valgrind output"
echo "Words tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f outfile$NUM.txt
  timeout 5 valgrind --leak-check=full -v ./Words "$RELATIVE_PATH/infile$NUM.txt" outfile$NUM.txt &> valgrind-out$NUM.txt
  if [ $? -eq 124 ]; then
    echo -e "${RED} Words TEST TIMED OUT ${NC}"
  fi
  diff -bBwu outfile$NUM.txt "$RELATIVE_PATH/model-outfile$NUM.txt" &> diff$NUM.txt >> diff$NUM.txt
  echo "Words Test $NUM:"
  echo "=========="
  cat diff$NUM.txt
  echo "=========="
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    let wordstestspassed+=1
  fi
done

let wordstestpoints=5*wordstestspassed

echo "Passed $wordstestspassed / $NUMTESTS Words tests"
echo "This gives a total of $wordstestpoints / $MAXPTS points"
echo ""
echo ""

echo "Press Enter To Continue with Valgrind Results for Words"
#TODO find a way to automate detecting if leaks and errors are found and how many
read garbage

for NUM in $(seq 1 $NUMTESTS); do
   echo "Words Valgrind Test $NUM: (press enter)"
   read garbage
   echo "=========="
   cat valgrind-out$NUM.txt
   echo "=========="
done

echo ""
echo ""


