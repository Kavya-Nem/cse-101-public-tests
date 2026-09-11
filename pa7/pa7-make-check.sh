#!/bin/bash

rm -f *.o Words

make -j8
makeexitcode=$?
exitcode=0

if [ ! $makeexitcode -eq 0 ] || [ ! -x Words ]; then # exist and executable
  ((exitcode++))
fi

make clean

if [ -f Words ] || (($(compgen -G "*.o" | wc -l) > 0)); then
   ((exitcode++))
fi
exit $exitcode
