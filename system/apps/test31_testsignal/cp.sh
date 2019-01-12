mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/31testg.elf fs/31testg.elf
sudo umount fs
rmdir fs