#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa8"
rm -f Dictionary.o
MAXTIME=$((${1:-1}*6))
testspassed=0

if g++ -std=c++17 -Wall -c -g "$RELATIVE_PATH/"ModelDictionaryTest.cpp Dictionary.cpp; then
  ((testspassed++))
fi
if g++ -std=c++17 -Wall -o ModelDictionaryTest ModelDictionaryTest.o Dictionary.o; then
  ((testspassed++))
fi
timeout $MAXTIME /usr/bin/time -o time.txt -f "%U" ./ModelDictionaryTest -v > DictionaryTest-out.txt 2> /dev/null
t=$?
if [ -f time.txt ]; then
  userTime=$(cat time.txt)
else
  userTime=$((MAXTIME/3+1))
fi
tooSlow=$(echo "$userTime > ($MAXTIME/3)" |bc -l)
if [ $tooSlow -eq 0 ] && [ $t -eq 0 ]; then
  ((testspassed++))
fi
timeout $MAXTIME valgrind --error-exitcode=2 --leak-check=full -v ./ModelDictionaryTest > /dev/null 2> DictionaryTest-mem.txt 
if [ $? -eq 0 ]; then
  ((testspassed++))
fi

exit $((4-testspassed))
