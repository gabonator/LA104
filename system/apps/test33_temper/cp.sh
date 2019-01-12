mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/33temper.elf fs/33temper.elf
sudo umount fs
rmdir fs