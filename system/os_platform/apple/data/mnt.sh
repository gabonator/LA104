if [ -d "fs" ]; then
  sudo umount /dev/disk2; sudo hdiutil detach /dev/disk2; rmdir fs
else
  mkdir fs; sudo hdiutil attach -imagekey diskimage-class=CRawDiskImage -nomount la104.fat; sudo mount -t msdos /dev/disk2 fs
fi
