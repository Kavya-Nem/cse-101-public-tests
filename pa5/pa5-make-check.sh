#!/bin/bash

rm -f *.o WordFrequency
make
exitcode=0

if [ ! -x WordFrequency ] || (($(compgen -G "*.o" | wc -l) == 0)); then # exist
  ((exitcode++))
fi

make clean

if [ -f WordFrequency ] || (($(compgen -G "*.o" | wc -l) > 0)); then
  ((exitcode++))
fi

exit $exitcode
