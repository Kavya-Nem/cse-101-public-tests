#!/bin/bash

rm -f *.o Words

make -j8
exitcode=0

if [ ! -x Words ] || (($(compgen -G "*.o" | wc -l) == 0)); then # exist
  ((exitcode++))
fi

make clean

if [ -f Words ] || (($(compgen -G "*.o" | wc -l) > 0)); then
   ((exitcode++))
fi
exit $exitcode
