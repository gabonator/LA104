mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/10i2c.elf fs/10i2c.elf
sudo umount fs
rmdir fs