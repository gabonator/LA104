mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/8ds.elf fs/8ds.elf
sudo umount fs
rmdir fs