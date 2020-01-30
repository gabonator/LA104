#!/bin/bash
mkdir build
cd build 

# code origin: https://github.com/merbanan/rtl_433

PATH=$PATH:/Users/gabrielvalky/Documents/git/ext/emsdk/emscripten/1.38.28

INCLUDES="-I ../rtltest"

SOURCE="\
  ../rtltest/alloc.c \
  ../rtltest/bitbuffer.c \
  ../rtltest/data.c \
  ../rtltest/decoder_util.c \
  ../rtltest/main.cpp \
  ../rtltest/pulse_demod.c \
  ../rtltest/util.c \
  ../rtltest/devices/efergy_e2_classic.c \
  ../rtltest/devices/hondaremote.c \
  ../rtltest/devices/springfield.c \
  ../rtltest/devices/thermopro_tp11.c \
  ../rtltest/devices/kerui.c \
  ../rtltest/devices/solight_te44.c \
  ../rtltest/devices/fineoffset_wh1080.c \
  ../rtltest/devices/tfa_pool_thermometer.c \
  ../rtltest/devices/gt_wt_02.c \
  ../rtltest/devices/nexus.c \
  ../rtltest/devices/digitech_xc0324.c \
  ../rtltest/devices/oregon_scientific.c \
  ../rtltest/devices/ttx201.c \
  ../rtltest/devices/nexa.c \
  ../rtltest/devices/honeywell_wdb.c \
  ../rtltest/devices/infactory.c \
  ../rtltest/devices/elro_db286a.c \
  ../rtltest/devices/alecto.c \
  ../rtltest/devices/ambientweather_tx8300.c \
  ../rtltest/devices/tpms_ford.c \
  ../rtltest/devices/radiohead_ask.c \
  ../rtltest/devices/x10_sec.c \
  ../rtltest/devices/tpms_citroen.c \
  ../rtltest/devices/elv.c \
  ../rtltest/devices/tpms_pmv107j.c \
  ../rtltest/devices/waveman.c \
  ../rtltest/devices/esa.c \
  ../rtltest/devices/ht680.c \
  ../rtltest/devices/chuango.c \
  ../rtltest/devices/thermopro_tp12.c \
  ../rtltest/devices/emontx.c \
  ../rtltest/devices/brennenstuhl_rcs_2044.c \
  ../rtltest/devices/oregon_scientific_v1.c \
  ../rtltest/devices/intertechno.c \
  ../rtltest/devices/kedsum.c \
  ../rtltest/devices/quhwa.c \
  ../rtltest/devices/esperanza_ews.c \
  ../rtltest/devices/ambientweather_wh31e.c \
  ../rtltest/devices/maverick_et73x.c \
  ../rtltest/devices/x10_rf.c \
  ../rtltest/devices/ge_coloreffects.c \
  ../rtltest/devices/current_cost.c \
  ../rtltest/devices/ft004b.c \
  ../rtltest/devices/simplisafe.c \
  ../rtltest/devices/inovalley-kw9015b.c \
  ../rtltest/devices/ibis_beacon.c \
  ../rtltest/devices/prologue.c \
  ../rtltest/devices/lacrosse_TX141TH_Bv2.c \
  ../rtltest/devices/silvercrest.c \
  ../rtltest/devices/tpms_renault.c \
  ../rtltest/devices/schraeder.c \
  ../rtltest/devices/tfa_twin_plus_30.3049.c \
  ../rtltest/devices/bresser_3ch.c \
  ../rtltest/devices/new_template.c \
  ../rtltest/devices/efergy_optical.c \
  ../rtltest/devices/newkaku.c \
  ../rtltest/devices/blyss.c \
  ../rtltest/devices/oil_watchman.c \
  ../rtltest/devices/generic_temperature_sensor.c \
  ../rtltest/devices/calibeur.c \
  ../rtltest/devices/opus_xt300.c \
  ../rtltest/devices/cardin.c \
  ../rtltest/devices/rftech.c \
  ../rtltest/devices/wg_pb12v1.c \
  ../rtltest/devices/ambient_weather.c \
  ../rtltest/devices/m_bus.c \
  ../rtltest/devices/mebus.c \
  ../rtltest/devices/bresser_5in1.c \
  ../rtltest/devices/tpms_toyota.c \
  ../rtltest/devices/generic_remote.c \
  ../rtltest/devices/rubicson.c \
  ../rtltest/devices/fineoffset_wh1050.c \
  ../rtltest/devices/generic_motion.c \
  ../rtltest/devices/dish_remote_6_3.c \
  ../rtltest/devices/fineoffset.c \
  ../rtltest/devices/smoke_gs558.c \
  ../rtltest/devices/dsc.c \
  ../rtltest/devices/lacrossews.c \
  ../rtltest/devices/maverick_et73.c \
  ../rtltest/devices/proove.c \
  ../rtltest/devices/philips.c \
  ../rtltest/devices/hideki.c \
  ../rtltest/devices/steelmate.c \
  ../rtltest/devices/wssensor.c \
  ../rtltest/devices/wt450.c \
  ../rtltest/devices/acurite.c \
  ../rtltest/devices/lacrosse.c \
  ../rtltest/devices/s3318p.c \
  ../rtltest/devices/lightwave_rf.c \
  ../rtltest/devices/vaillant_vrt340f.c \
  ../rtltest/devices/akhan_100F14.c \
  ../rtltest/devices/oil_standard.c \
  ../rtltest/devices/honeywell.c \
  ../rtltest/devices/oregon_scientific_sl109h.c \
  ../rtltest/devices/interlogix.c \
  ../rtltest/devices/fordremote.c \
  ../rtltest/devices/lacrosse_tx35.c \
  ../rtltest/devices/bt_rain.c \
  ../rtltest/devices/wt0124.c \
  ../rtltest/devices/danfoss.c
"
EXPORTED="['_convert', '_buffer']"
emcc ${INCLUDES} ${SOURCE} -g4 -O3 -s TOTAL_STACK=4096 -s TOTAL_MEMORY=131072 -s MINIMAL_RUNTIME=1 -s WASM=1 -s EXPORTED_FUNCTIONS="${EXPORTED}" -o rtl433.js -DEMSCRIPTEN -s ERROR_ON_UNDEFINED_SYMBOLS=0
cp rtl433.wasm app.wasm
node /Users/gabrielvalky/Documents/git/LA104/system/os_platform/wasm/htmllite/package.js
rm index.html
rm app.js
rm app.wasm
rm rtl433.js
rm rtl433.wast
