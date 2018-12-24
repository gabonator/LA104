mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/29fileman.elf fs/fileman.elf
sudo umount fs
rmdir fs