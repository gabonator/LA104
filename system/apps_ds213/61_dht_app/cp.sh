mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/9dht.elf fs/9dht.elf
sudo umount fs
rmdir fs