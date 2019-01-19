mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/36espwww.elf fs/36espwww.elf
#cp test.txt fs/test.txt
sudo umount fs
rmdir fs