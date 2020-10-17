mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/18onew.elf fs/18onew.elf
sudo umount fs
rmdir fs