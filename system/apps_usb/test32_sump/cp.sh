mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/32sump.elf fs/32sump.elf
sudo umount fs
rmdir fs