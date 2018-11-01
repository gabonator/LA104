mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/12uart.elf fs/12uart.elf
sudo umount fs
rmdir fs