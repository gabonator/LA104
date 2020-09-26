mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/37icscan.elf fs/37icscan.elf
#cp test.txt fs/test.txt
sudo umount fs
rmdir fs