#!/bin/bash
rm -f *.o Sparse
make
exitcode=0
if [ ! -x Sparse ] || (($(compgen -G "*.o" | wc -l) == 0)); then # exist and executable
  ((exitcode++))
fi
make clean
if [ -f Sparse ] || (($(compgen -G "*.o" | wc -l) > 0)); then
  ((exitcode++))
fi
exit $exitcode
