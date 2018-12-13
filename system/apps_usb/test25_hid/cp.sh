mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/25hid.elf fs/25hid.elf
sudo umount fs
rmdir fs