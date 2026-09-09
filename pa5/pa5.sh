#!/bin/bash
# usage: ../cse-101-public-tests/pa5/pa5.sh
# (run within your pa5 directory to test your code)

RELATIVE_PATH="../cse-101-public-tests/pa5"
EXE_ALL=( pa5-func-check.sh pa5-unit-check.sh pa5-make-check.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
OVERALL=0
for i in $(seq 0 $EXE_RANGE); do
  FULLPATH="$RELATIVE_PATH/${EXE_ALL[i]}"
  chmod +x $FULLPATH
  ./$FULLPATH
  rc=$?
  if [ $rc -ne 0 ]; then
    OVERALL=$((OVERALL + 1))
  fi
done
exit $OVERALL
