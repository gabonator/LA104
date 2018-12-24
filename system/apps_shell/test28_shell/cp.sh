mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/28shell.elf fs/shell.elf
#cp folder.bmp fs/folder.bmp
#cp out.bmp fs/out.bmp
sudo umount fs
rmdir fs