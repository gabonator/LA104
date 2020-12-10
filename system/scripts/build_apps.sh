#!/bin/bash

ERRORS=0
VERBOSE=0
if [ "$1" == "verbose" ]; then
  set -x
  VERBOSE=1
fi

OUTPUT=../build
mkdir $OUTPUT/apps

buildApp () {
  (
    cd $1
    if [ ! -f build.sh ]; then
      echo "[WARNING] $1: No build script"
      exit 1
    fi

    rm -rf build
    if [ $VERBOSE -eq 1 ]; then
      ./build.sh
    else
      ./build.sh > /dev/null 2> /dev/null
    fi

    if [ $? -eq 1 ]; then
      echo "[WARNING] $1: Build failed"
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
    echo "[WARNING] $1: No executable found"
    exit 1
  )

  if [ $? -eq 0 ]; then
    echo $1: Success!
  else
    ERRORS=1
  fi
}

cleanApp () {
  (
    cd $1
    rm -rf build
  )
}

for d in ../apps_featured/*/ ; do
    buildApp "$d"
done
for d in ../apps_shell/*/ ; do
    buildApp "$d"
done
for d in ../apps_experiments/*/ ; do
    buildApp "$d"
done
for d in ../apps/*/ ; do
    buildApp "$d"
done
for d in ../apps_usb/*/ ; do
    buildApp "$d"
done

for d in ../apps_ds213/*/ ; do
    buildApp "$d"
done

# copy fresh builds
cp ../os_host/build/system*.hex $OUTPUT/firmware

if [ $? -eq 1 ]; then
  exit 1
fi

for d in ../apps*/*/ ; do
    cp $d/build/*.elf $OUTPUT/apps/
done
rm $OUTPUT/apps/output.elf

# cleanup
for d in ../apps_featured/*/ ; do
    cleanApp "$d"
done
for d in ../apps_shell/*/ ; do
    cleanApp "$d"
done
for d in ../apps_experiments/*/ ; do
    cleanApp "$d"
done
for d in ../apps/*/ ; do
    cleanApp "$d"
done
for d in ../apps_usb/*/ ; do
    cleanApp "$d"
done
for d in ../apps_ds213/*/ ; do
    cleanApp "$d"
done

if [ $ERRORS -eq 1 ]; then
  echo [WARNING] Built with errors!
else
  echo Build ok!
fi

exit $ERRORS