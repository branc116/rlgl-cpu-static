#!/usr/bin/env sh

set -ex

test -d examples || mkdir examples;

EX_DIR="../../raysan5/raylib/examples"
EXAMPLES=$(find $EX_DIR -name "*.c")
SLIBS="build/raylib.sw.a build/simp.o"

CUR_DIR=$(pwd)

for i in $EXAMPLES
do
  DIR=$(dirname ${i})
  OUT_NAME=$(echo ${i} | sed 's|../../raysan5/raylib/||')
  OUT_DIR=$(dirname ${OUT_NAME})
  test -d $OUT_DIR || mkdir $OUT_DIR
  if [ ! -e "$OUT_NAME.png" ]
  then
    gcc -I../../raysan5/raylib/src -ggdb -fsanitize=address -o $OUT_NAME ${i} $SLIBS -lm
    OUT_FULL_NAME=$(which $OUT_NAME)
    cd $DIR
    $OUT_FULL_NAME
    cd $CUR_DIR
    mv "$DIR/done.png" "$OUT_NAME.png"
  fi
done
