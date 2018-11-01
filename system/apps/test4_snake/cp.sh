mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/4snake.elf fs/4snake.elf
sudo umount fs
rmdir fs