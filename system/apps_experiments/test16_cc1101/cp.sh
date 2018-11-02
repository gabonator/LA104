mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/16cc1101.elf fs/16cc1101.elf
sudo umount fs
rmdir fs