Use this tool to upload firmware into LA104 or DS203 from mac or linux

To use this tool (OSX):
    * disable automatic mounting as described here https://discussions.apple.com/docs/DOC-7942 
    * run 'sudo vifs'
    * add similar line as this 'UUID=0C6BB56F-CEA7-3564-A81A-FB98D5C3BC33 none msdos rw,noauto'
    * run 'sudo automount -vc'
    * run 'sudo ./dfuload /dev/disk2 cp myfile.hex'

Credits:
    * based on this project: https://github.com/qihaiyan/fat12.git