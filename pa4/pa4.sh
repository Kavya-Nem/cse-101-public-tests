#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/pa4"
EXE_ALL=( pa4-func-check.sh pa4-lunit-check.sh pa4-munit-check.sh pa4-make-check.sh )
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
