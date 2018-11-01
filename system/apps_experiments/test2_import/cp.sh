mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/2import.elf fs/2import.elf
sudo umount fs
rmdir fs