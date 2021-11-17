set path=%PATH%;%CD%\quartus\bin
set path=%PATH%;%CD%\Supra\bin

rmdir app /s /q
mkdir app

cd app

copy ..\*.v .
copy ..\*.ip .
copy ..\*.asf .

af.exe -B --setup --mode QUARTUS --design app --device AG1KLPQ48 --ip gabobram.ip -X "set DEVICE_FAMILY 1"
quartus_sh -t af_quartus.tcl
af.exe -B --batch --mode QUARTUS -X "set QUARTUS_SDC true" -X "set FITTING timing_more" -X "set FITTER full" -X "set EFFORT highest" -X "set HOLDX default" -X "set SKEW basic"

copy app.bin ..
cd ..
rmdir app /s /q

