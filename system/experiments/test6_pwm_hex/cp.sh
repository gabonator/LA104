mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/6pwm.hex fs/6pwm.hex
sudo umount fs
rmdir fs