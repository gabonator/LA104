gcc -std=c++11 -fno-threadsafe-statics -fno-rtti -fno-exceptions -D LA104 main.cpp terminal.cpp -o prerender
IMAGEROOT=$PWD/../../system/build/image_la104/
TOOL=$PWD/prerender
# ./prerender $IMAGEROOT > report

cd $IMAGEROOT
$TOOL ./ > report

cat report | grep -q Missing
if [ $? -eq 0 ];
then
  echo "Missing bitmaps:"
  cat report | sort | uniq | grep Missing
  exit 1
fi

# cat report | grep Origin: | sort | uniq | cut -c 9- | xargs echo rm | sh
#rm report