mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/15chars.elf fs/15chars.elf
sudo umount fs
rmdir fs