dd if=/dev/zero of=test2.img bs=50 count=1048576
DISK=`sudo hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount test2.img`
echo newdisk: $DISK
diskutil eraseDisk FAT32 SDCARD MBRFormat $DISK
cp -a fs/ /Volumes/SDCARD
sudo hdiutil detach $DISK
