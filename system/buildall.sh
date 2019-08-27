(
  echo Building host...
  cd os_host
  rm -rf build
#  rm build/manager.hex 2> /dev/null
  ./build.sh
  if [ ! -f build/system_la104.hex ]; then
    echo Unable to build operating system firmware
  fi
)

if [ $? -eq 1 ]; then
    exit 1
fi

(
  echo Building library
  cd os_library
  rm -rf build
#  rm build/libbios.so 2> /dev/null
  ./build.sh
  if [ ! -f build/libbios_la104.so ]; then
    echo Unable to build shared library prototype
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
    ./build.sh > /dev/null 2> /dev/null

    for f in build/*.hex; do
      [ -e "$f" ] && exit 0
      break
    done
    for f in build/*.elf; do
      [ -e "$f" ] && exit 0
      break
    done
    echo "$1: Build failed"
    exit 1
  )

  if [ $? -eq 0 ]; then
    echo $1: Success!
  fi
}

cleanApp () {
  (
    cd $1
    rm -rf build
  )
}

for d in apps_shell/*/ ; do
    buildApp "$d"
done
for d in apps_experiments/*/ ; do
    buildApp "$d"
done
for d in apps/*/ ; do
    buildApp "$d"
done
for d in apps_arduino/*/ ; do
    buildApp "$d"
done
for d in apps_usb/*/ ; do
    buildApp "$d"
done

# copy fresh builds
cp os_host/build/system*.hex release/
mkdir release
mkdir release/bin
for d in apps*/*/ ; do
    cp $d/build/*.elf release/bin/
done
rm release/bin/output.elf

# cleanup
for d in apps_shell/*/ ; do
    cleanApp "$d"
done
for d in apps_experiments/*/ ; do
    cleanApp "$d"
done
for d in apps/*/ ; do
    cleanApp "$d"
done
for d in apps_arduino/*/ ; do
    cleanApp "$d"
done
for d in apps_usb/*/ ; do
    cleanApp "$d"
done
