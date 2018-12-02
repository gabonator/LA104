mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/23screen.elf fs/23screen.elf
sudo umount fs
rmdir fs