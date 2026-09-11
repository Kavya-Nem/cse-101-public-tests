#!/bin/bash

make WordFrequency
makeexitcode=$?
make Words
wordsexitcode=$?
if [ $makeexitcode -eq 0 ]; then
  makeexitcode=$wordsexitcode
fi
exitcode=0
if [ ! $makeexitcode -eq 0 ] || [ ! -x Words ] || [ ! -x WordFrequency ]; then # exist and executable
  ((exitcode++))
fi

make clean

if [ -f Words ] || (($(compgen -G "*.o" | wc -l) > 0)) || [ -f WordFrequency ]; then
   ((exitcode++))
fi

exit $exitcode
