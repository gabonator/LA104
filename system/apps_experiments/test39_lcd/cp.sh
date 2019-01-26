mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/3gui.elf fs/3gui.elf
sudo umount fs
rmdir fs