#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo ""
echo ""
echo "press Enter for make tests"
read garbage

make WordFrequency
make Words

if [ ! -e Words ] || [ ! -x Words ]; then # exist and executable
  echo ""
  echo -e "${RED}Makefile probably doesn't correctly create Words!!${NC}"
  echo ""
else
  echo ""
  echo -e "${GREEN}Makefile probably correctly creates Words!${NC}"
  echo ""
fi

if [ ! -e WordFrequency ] || [ ! -x WordFrequency ]; then # exist and executable
  echo ""
  echo -e "${RED}Makefile probably doesn't correctly create WordFrequency!!${NC}"
  echo ""
else
  echo ""
  echo -e "${GREEN}Makefile probably correctly creates WordFrequency!${NC}"
  echo ""
fi

make clean

echo ""
echo ""

if [ -e Words ] || [ -e *.o ]; then
   echo -e "${RED}WARNING: Makefile didn't successfully clean all files${NC}"
fi