Use this tool to upload firmware into LA104 or DS203 from mac or linux

To use this tool (OSX):
    * disable automatic mounting as described here https://discussions.apple.com/docs/DOC-7942
    * find the UUID of the volume "diskutil info /Volumes/LA104"
    * run 'sudo vifs'
    * add similar line as this 'UUID=0C6BB56F-CEA7-3564-A81A-FB98D5C3BC33 none msdos rw,noauto'
    * run 'sudo automount -vc'
    * run 'sudo ./dfuload /dev/disk2 cp myfile.hex'

If you want to format the 8MB eeprom drive, use *format.sh* or call 'sudo newfs_msdos -F 12 -v la104 -O "MSDOS5.0" -S 4096 /dev/disk2'

Credits:
    * based on this project: https://github.com/qihaiyan/fat12.git


More tools:
  - format.sh - formats the internal storage of LA104, results in clean disk
  - snapshot.sh - downloads whole filesystem from LA104 (DSxxx) into single file for use in emulated environment (running LA104 apps from mac/pc, look for la104.fat file in repository)