#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa5"

echo "Press Enter To Continue with Dictionary Unit Test"
read verbose

echo ""
echo ""

gcc -std=c17 -Wall -c -g "$RELATIVE_PATH/"ModelDictionaryTest.c Dictionary.c
gcc -std=c17 -Wall -o ModelDictionaryTest ModelDictionaryTest.o Dictionary.o -lm

timeout 10 valgrind --leak-check=full -v ./ModelDictionaryTest -v > DictionaryTest-out.txt 2> DictionaryTest-mem.txt
if [ $? -eq 124 ]; then
   echo -e "${RED} MODEL Dictionary TEST TIMED OUT ${NC}"
fi

cat DictionaryTest-out.txt
echo
echo "Press Enter for Memory Leaks Report:"
read verbose
cat DictionaryTest-mem.txt

rm -f *.o

