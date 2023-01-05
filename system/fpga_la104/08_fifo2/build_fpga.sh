curl --request POST \
    http://cloud.valky.eu:8086/synth \
    --form "app.v=@source/app.v" \
    --form "app.asf=@source/app.asf"

#    --form "app.sdc=@source/app.sdc" \
#
mkdir -p build
curl -s http://cloud.valky.eu:8086/result --output build/08_fifo2.fpg
cp build/08_fifo2.fpg /Volumes/LA104/
