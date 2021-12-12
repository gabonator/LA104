dd if=/dev/zero of=la104.fs bs=4096 count=2048
DISK=`sudo hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount la104.fs`
echo newdisk: $DISK
sudo newfs_msdos -F 12 -S 4096 -v LA104 $DISK
sudo mkdir /Volumes/LA104
sudo mount -t msdos $DISK /Volumes/LA104
cp -a ../build/image_la104/ /Volumes/LA104
sudo hdiutil detach $DISK
cp la104.fs ../os_platform/mac_sdl/data/la104.fat