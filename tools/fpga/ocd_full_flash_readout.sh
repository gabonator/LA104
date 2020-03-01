openocd -f /usr/local/share/openocd/scripts/interface/stlink.cfg -f /usr/local/share/openocd/scripts/target/stm32f1x.cfg -c init -c halt -c "flash read_bank 0 readout2" -c exit
