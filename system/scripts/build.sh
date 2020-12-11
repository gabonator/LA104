./build_full.sh $1
./release_image_ds203.sh $1
./release_image_ds213.sh $1
./release_image_la104.sh $1

node -v
# applist disabled because of old nodejs version in docker builds
# ./build_applist.sh $1