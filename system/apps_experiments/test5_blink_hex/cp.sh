mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/5blink.hex fs/5blink.hex
sudo umount fs
rmdir fs