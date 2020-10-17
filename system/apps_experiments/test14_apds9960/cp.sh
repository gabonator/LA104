mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/14apds1.elf fs/14apds1.elf
cp build/14apds2.elf fs/14apds2.elf
sudo umount fs
rmdir fs