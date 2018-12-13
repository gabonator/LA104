mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/26midi.elf fs/26midi.elf
sudo umount fs
rmdir fs