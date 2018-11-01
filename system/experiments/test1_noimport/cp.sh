mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/1noimp.elf fs/1noimp.elf
sudo umount fs
rmdir fs