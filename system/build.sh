#!/bin/bash

# Make sure we've got the compiler
if [ "`which arm-none-eabi-gcc`" == "" ]; then
  if [ "`which apt`" != "" ]; then
    echo "Installing gcc-arm-none-eabi..."
    sudo apt install -y gcc-arm-none-eabi
  else
    echo "[ERROR] arm-none-eabi-gcc not found!"
    exit 1
  fi
fi

# Required device
DEVICE="$1"
if [ "$DEVICE" == "" ]; then
  echo
  echo "Usage: $0 [DEVICE] [THREADING]"
  echo
  echo " DEVICE"
  echo "   ds203: Build for DS203"
  echo "   ds213: Build for DS213"
  echo "   la104: Build for LA104"
  echo "   all:   Build for all the devices"
  echo
  echo " THREADING"
  echo "   single: Build one app at a time (Default)"
  echo "   multi:  Build all the apps in a dir at the same time"
  echo "   all:    Build ALL the apps at the same time"
  echo
  exit 1
fi

# Should we go multi-threading?
THREADED="$2"
if [ "$THREADED" != "multi" -a "$THREADED" != "all" ]; then
  THREADED="single"
fi

# If we have to build all of them
if [ "$DEVICE" == "all" ]; then
  for build in ds203 ds213 la104; do
    echo "Building $build..."
    $0 $build || exit 1
  done
fi

if [ ! -f "../tools/elfstrip/elfstrip" ]; then
  echo "Building elfstrip..."
  (
    cd "../tools/elfstrip"
    ./build.sh
  )
  if [ ! -f "../tools/elfstrip/elfstrip" ]; then
    echo "[ERROR] Failed building elfstrip"
    exit 1
  fi
fi

echo "Building library..."
(
  cd os_library
  rm -rf build
  # rm build/libbios.so 2> /dev/null
  ./build.sh $DEVICE
  if [ ! -f build/libbios_$DEVICE.so ]; then
    echo "[ERROR] Unable to build shared library prototype"
    exit 1
  fi
)

if [ $? -eq 1 ]; then
    exit 1
fi

echo "Building host..."
(
  cd os_host
  rm -rf build
  # rm build/manager.hex 2> /dev/null
  ./build.sh $DEVICE
  if [ ! -f build/system_$DEVICE.hex ]; then
    echo "[ERROR] Unable to build operating system firmware"
    exit 1
  fi
)

if [ $? -eq 1 ]; then
    exit 1
fi

buildApp () {
  (
    cd $1
    if [ ! -f build.sh ]; then
      echo "$1: No build script"
      exit 1
    fi

    rm -rf build
    ./build.sh $DEVICE > /dev/null 2> /dev/null

    for f in build/*.hex; do
      [ -e "$f" ] && exit 0
      break
    done
    for f in build/*.elf; do
      [ -e "$f" ] && exit 0
      break
    done
    echo "$1: FAILED!"
    exit 1
  )

  if [ $? -eq 0 ]; then
    echo "$1: Success!"
  fi
}

copyApp() {
  (
    if [ -d $1/build ]; then
      rm -f $1/build/output.elf
      if [ "`ls $1/build/ | grep \.elf`" != "" ] ;then
        mkdir -p release/$2
        cp $1/build/*.elf release/bin/
        cp $1/build/*.elf release/$2/
      fi
    fi
  )
}

cleanApp () {
  (
    cd $1
    rm -rf build
  )
}

buildCopyCleanApp() {
  buildApp "$1"
  copyApp "$1" "$2"
  cleanApp "$1"
}

waitForBuildsToFinish() {
  while [ true ]; do
    if [ "`ps ax | grep -v grep | grep arm-none-eabi- | wc -l`" == "0" ]; then
      sleep 1s
      if [ "`ps ax | grep -v grep | grep arm-none-eabi- | wc -l`" == "0" ]; then
        break
      fi
    fi
    sleep 1s
  done
}

mkdir -p release/bin
cp os_host/build/system*.hex release/

# Build apps
for APPS_GROUP in $(ls -1 | grep apps); do
  for APP_DIR in $APPS_GROUP/* ; do
    if [ ! -d "$APP_DIR" ]; then
      continue
    fi
    if [ "$THREADED" == "single" ]; then
      buildCopyCleanApp "$APP_DIR" "$APPS_GROUP"
    else
      buildCopyCleanApp "$APP_DIR" "$APPS_GROUP" &
    fi
  done
  [[ "$THREADED" == "multi" ]] && waitForBuildsToFinish
done
[[ "$THREADED" == "all" ]] && waitForBuildsToFinish

echo "Done"
