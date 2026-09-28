#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa5"
rm -f Dictionary.o
testspassed=0
if gcc -std=c17 -Wall -c -g "$RELATIVE_PATH/"ModelDictionaryTest.c Dictionary.c; then
   ((testspassed++))
fi
if gcc -std=c17 -Wall -o ModelDictionaryTest ModelDictionaryTest.o Dictionary.o -lm; then
   ((testspassed++))
fi
timeout 10 /usr/bin/time -o time.txt -f "%U" ./ModelDictionaryTest -v > DictionaryTest-out.txt 2> /dev/null
t=$?
if [ -f time.txt ]; then
   userTime=$(cat time.txt)
else
   userTime=5
fi
tooSlow=$(echo "$userTime > 4" |bc -l)
if [ $tooSlow -eq 0 ] && [ $t -eq 0 ]; then
   ((testspassed++))
fi
timeout 10 valgrind --error-exitcode=2 --leak-check=full -v ./ModelDictionaryTest > /dev/null 2> DictionaryTest-mem.txt
if [ $? -eq 0 ]; then
   ((testspassed++))
fi
exit $((4-testspassed))
