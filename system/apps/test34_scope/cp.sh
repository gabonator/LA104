mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/34scope.elf fs/34scope.elf
sudo umount fs
rmdir fs