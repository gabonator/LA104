mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/27irsend.elf fs/27irsend.elf
sudo umount fs
rmdir fs