#!/bin/bash
rm -f Words WordFrequency *.o
make WordFrequency
make Words
exitcode=0
if [ ! -x Words ] || [ ! -x WordFrequency ] || (($(compgen -G "*.o" | wc -l) == 0)); then # exist
  ((exitcode++))
fi

make clean

if [ -f Words ] || (($(compgen -G "*.o" | wc -l) > 0)) || [ -f WordFrequency ]; then
   ((exitcode++))
fi
exit $exitcode
