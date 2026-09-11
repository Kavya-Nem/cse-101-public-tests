#!/bin/bash

rm -f *.o WordFrequency
make
makeexitcode=$?
exitcode=0

if [ ! $makeexitcode -eq 0 ] || [ ! -x WordFrequency ]; then # exist and executable
  ((exitcode++))
fi

make clean

if [ -f WordFrequency ] || (($(compgen -G "*.o" | wc -l) > 0)); then
  ((exitcode++))
fi

exit $exitcode
