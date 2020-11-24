sudo umount /Volumes/LA104
#sudo dd if=la104.fat of=/dev/disk2 bs=4096 count=2048
sudo dd if=la104.fs | pv -s 8M | sudo dd of=/dev/disk2 bs=4096