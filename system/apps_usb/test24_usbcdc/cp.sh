mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/24ncdc.elf fs/24ncdc.elf
sudo umount fs
rmdir fs