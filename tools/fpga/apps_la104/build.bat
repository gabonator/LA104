@echo off

set path=%PATH%;C:\Remote\altera\quartus\bin
set path=%PATH%;C:\Remote\Supra-2021.06.b0-66adbd53-win32-all\bin

rmdir app /s /q
mkdir app

cd app
rem Uncomment to enable PLL
rem af.exe -B -X "exit [catch {generate_pll {inpll} -pll_type {PLLX} -input_freq {2} -output_freqs { {25}} -fb_mode {NO_REFERENCE}}]"
af.exe -B --setup --mode QUARTUS --design app --device AG1KLPQ48 -X "set DEVICE_FAMILY 1"

copy ..\app.v app.v
copy ..\app.asf app.asf
quartus_sh -t af_quartus.tcl 
af.exe -B --batch --mode QUARTUS -X "set QUARTUS_SDC true" -X "set FITTING timing_more" -X "set FITTER full" -X "set EFFORT highest" -X "set HOLDX default" -X "set SKEW basic"

copy app.bin ..