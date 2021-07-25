cd $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )
mkdir build
cd build
cmake .. -DDESKTOP=1 -DCMAKE_INSTALL_PREFIX=../_install -GXcode