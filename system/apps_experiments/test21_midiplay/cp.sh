mkdir fs
sudo mount -t msdos /dev/disk2 fs
cp build/21mplay.elf fs/21mplay.elf
#cp laisla.mid fs/laisla.mid
#cp thesign.mid fs/thesign.mid
#cp gabosong.mid fs/gabosong.mid
#cp salsa.mid fs/salsa.mid
#cp demo.mid fs/demo.mid
sudo umount fs
rmdir fs