#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/hashtable"
EXE_ALL=( hashtable.sh model-hashtable-test.sh hashtable-build.sh )
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
