#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa5"
rm -f Dictionary.o
gcc -std=c17 -Wall -c -g "$RELATIVE_PATH/"ModelDictionaryTest.c Dictionary.c
gcc -std=c17 -Wall -o ModelDictionaryTest ModelDictionaryTest.o Dictionary.o -lm
testspassed=0
timeout 10 /usr/bin/time -o time.txt -f "%U" ./ModelDictionaryTest -v > DictionaryTest-out.txt 2> /dev/null
t=$?
userTime=$(cat time.txt)
tooSlow=$(echo "$userTime > 4" |bc -l)
if [ ! $tooSlow -eq 1 ] && [ $t -eq 0 ]; then
   ((testspassed++))
fi
timeout 10 valgrind --leak-check=full -v ./ModelDictionaryTest > /dev/null 2> DictionaryTest-mem.txt
if [ $? -eq 0 ]; then
   bytes=`perl -ane 'print $F[5] if $F[4] eq "exit:"' DictionaryTest-mem.txt`
   if [ ${bytes//,/} -eq 0 ]; then
      ((testspassed++))
   fi
fi
exit $((2-$testspassed))
