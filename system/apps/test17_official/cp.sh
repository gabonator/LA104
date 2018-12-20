mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/17analys.elf fs/17analys.elf
sudo umount fs
rmdir fs