mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/32graph.elf fs/32graph.elf
sudo umount fs
rmdir fs