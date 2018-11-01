mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/13mp3.elf fs/13mp3.elf
sudo umount fs
rmdir fs