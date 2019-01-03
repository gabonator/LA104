mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/30dcf77.elf fs/30dcf77.elf
sudo umount fs
rmdir fs