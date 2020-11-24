dd if=/dev/zero of=la104.fs bs=4096 count=2048
sudo hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount la104.fs
sudo newfs_msdos -F 12 -S 4096 -v LA104 /dev/disk2
sudo mkdir /Volumes/LA104
sudo mount -t msdos /dev/disk2 /Volumes/LA104
cp -a ../build/image_la104/ /Volumes/LA104
sudo hdiutil detach /dev/disk2