#!/bin/bash
rm -f *.o Sparse
make

makeexitcode=$?
exitcode=0
if [ ! $makeexitcode -eq 0 ] || [ ! -x Sparse ]; then # exist and executable
  ((exitcode++))
fi

make clean

if [ -f Sparse ] || (($(compgen -G "*.o" | wc -l) > 0)); then
  ((exitcode++))
fi
exit $exitcode

