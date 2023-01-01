curl --request POST \
    http://cloud.valky.eu:8086/synth \
    --form "app.v=@source/app.v" \
    --form "spi_slave.v=@source/spi_slave.v" \
    --form "app.asf=@source/app.asf"

mkdir -p build
curl -s http://cloud.valky.eu:8086/result --output build/98_spi2.fpg
