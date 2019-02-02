mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/41alchemy.elf fs/41alch.elf
sudo umount fs
rmdir fs