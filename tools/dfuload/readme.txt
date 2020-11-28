Use this tool to upload firmware into LA104 or DS203 from mac or linux

To use this tool:
  - unmount the disk 'sudo umount /dev/disk2' if mounted
  - run 'sudo dfuload /dev/disk2 cp ../../system/os_host/build/system_la104.hex'
  - sometimes the tool cannot exit properly when the flashing is done, just power cycle the unit or press Ctrl+C to skip waiting for the disk disconnection

```
Opening: '/dev/disk2' ...
OemName: MSDOS5.0
Read direntry at 4000 ok
Read direntry at 4000 ok
Read direntry at 4020 ok
Read direntry at 4040 ok
Read direntry at 4060 ok
Read direntry at 4080 ok
Read direntry at 40a0 ok
Read direntry at 40c0 ok
Read direntry at 40e0 ok
Read direntry at 4100 ok
Read direntry at 4120 ok
Read direntry at 4140 ok
Read direntry at 4160 ok
..............................................................................................................................................
...............................................................................................................
Finished writing 129217 bytes
Updating direntry at 4160 ok
Closing data Ok
Closing disk <-- safe to disconnect at this point
```

Other scripts:
  - cp.sh - copies the LA104 system to the disk /dev/disk2
  - format.sh - formats the internal storage of LA104, results in clean disk, it just calls 'sudo newfs_msdos -F 12 -v la104 -O "MSDOS5.0" -S 4096 /dev/disk2'
  - snapshot.sh - downloads whole filesystem from LA104 (DSxxx) into single file for use in emulated environment (running LA104 apps from mac/pc, look for la104.fat file in repository)

To prevent automatic mountig (OSX):
    * disable automatic mounting as described here https://discussions.apple.com/docs/DOC-7942
    * find the UUID of the volume "diskutil info /Volumes/LA104"
    * run 'sudo vifs'
    * add similar line as this 'UUID=0C6BB56F-CEA7-3564-A81A-FB98D5C3BC33 none msdos rw,noauto'
    * run 'sudo automount -vc'
    * run 'sudo ./dfuload /dev/disk2 cp myfile.hex'

Credits:
    * based on this project: https://github.com/qihaiyan/fat12.git

