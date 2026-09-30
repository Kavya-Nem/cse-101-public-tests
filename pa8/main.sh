#!/bin/bash
RELATIVE_PATH="../cse-101-public-tests/redblacktree"
EXE_ALL=( words.sh wordfrequency.sh model-redblacktree-test.sh redblacktree-build.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
OVERALL=0
for i in $(seq 0 $EXE_RANGE); do
  FULLPATH="$RELATIVE_PATH/${EXE_ALL[i]}"
  chmod +x $FULLPATH
  ./$FULLPATH "${1:-1}"
  rc=$?
  if [ $rc -ne 0 ]; then
    OVERALL=$((OVERALL + 1))
  fi
done
exit $OVERALL
