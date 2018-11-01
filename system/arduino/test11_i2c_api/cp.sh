mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/11i2c.elf fs/11i2c.elf
sudo umount fs
rmdir fs