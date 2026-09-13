#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa8"
rm -f Dictionary.o
MAXTIME=$((${1:-1}*6))

g++ -std=c++17 -Wall -c -g "$RELATIVE_PATH/"ModelDictionaryTest.cpp Dictionary.cpp
g++ -std=c++17 -Wall -o ModelDictionaryTest ModelDictionaryTest.o Dictionary.o
testspassed=0
timeout $MAXTIME /usr/bin/time -o time.txt -f "%U" ./ModelDictionaryTest -v > DictionaryTest-out.txt 2> /dev/null
t=$?
userTime=$(cat time.txt)
tooSlow=$(echo "$userTime > ($MAXTIME/3)" |bc -l)
if [ ! $tooSlow -eq 1 ] && [ $t -eq 0 ]; then
  ((testspassed++))
fi
timeout $MAXTIME valgrind --leak-check=full -v ./ModelDictionaryTest > /dev/null 2> DictionaryTest-mem.txt 
if [ $? -eq 0 ]; then
  bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' DictionaryTest-mem.txt`
  if [ ${bytes//,/} -eq 0 ]; then
    ((testspassed++))
  fi
fi

exit $((2-$testspassed))
