mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/22seqen.elf fs/22seqen.elf
sudo umount fs
rmdir fs