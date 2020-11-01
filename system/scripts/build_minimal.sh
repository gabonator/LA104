#!/bin/bash

set -x

./build_check.sh
if [ $? -eq 1 ]; then
  echo Building environment not configured properly
  exit 1
fi

OUTPUT=../build
rm -rf $OUTPUT
mkdir $OUTPUT
mkdir $OUTPUT/firmware
mkdir $OUTPUT/apps

buildApp () {
  (
    cd $1
    if [ ! -f build.sh ]; then
      echo "$1: No build script"
      exit 1
    fi

    rm -rf build
    ./build.sh

    if [ $? -eq 1 ]; then
      echo "$1: Build failed"
      exit 1
    fi

    for f in build/*.hex; do
      [ -e "$f" ] && exit 0
      break
    done
    for f in build/*.elf; do
      [ -e "$f" ] && exit 0
      break
    done
    echo "$1: No executable found"
    exit 1
  )

  if [ $? -eq 0 ]; then
    echo $1: Success!
  else
    # exit script
    exit 1
  fi
}

cleanApp () {
  (
    cd $1
    rm -rf build
  )
}

./build_os.sh
if [ $? -eq 1 ]; then
  echo Building OS failed!
  exit 1
fi

for d in ../apps_featured/*/ ; do
  buildApp "$d"
done
for d in ../apps_shell/*/ ; do
  buildApp "$d"
done

cp ../os_host/build/system*.hex $OUTPUT/firmware

if [ $? -eq 1 ]; then
  exit 1
fi

for d in ../apps_featured/*/ ; do
    cp $d/build/*.elf $OUTPUT/apps
done
for d in ../apps_shell/*/ ; do
    cp $d/build/*.elf $OUTPUT/apps
done
rm $OUTPUT/output.elf

for d in ../apps_featured/*/ ; do
    cleanApp "$d"
done
for d in ../apps_shell/*/ ; do
    cleanApp "$d"
done

echo Minimal build succeeded
echo Target folder: $(pwd)/$OUTPUT

