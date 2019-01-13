mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/35view.elf fs/viewtxt.elf
#cp test.txt fs/test.txt
sudo umount fs
rmdir fs