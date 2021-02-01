# Simple examples to use the PRU on the Beaglbone AI :

## Contents

Each folder contains an example: 

```
├── hello_world_pru: Once started the PRU1_1 will blink the user LED marked D3 on the Beaglebone AI
├── include: contains the resource table for PRU1
├── read_register_from_pru: The ARM (user space) will send a message to the PRU core via rpmsg, when the pru core receives the message it will answer via the same channel
└── rpmsg_ping_pong: The ARM sends a register adress to the PRU via rpmsg, when the pru receives the adress it will answer to ARM by sending the value of the regaister
```

## Usage:

To use this project __on the Beaglebone AI__:
```
cd ~
git clone https://github.com/PierrickRauby/BeagleBone-AI-PRU.git
cd BeagleBone-AI-PRU
mv include/resource_table_1.h /var/lib/cloud9/common/
cat Readme.md

```
Each example contains its own Readme with instructions on how to use it. 

## Usefull notes:

The following table contains some notes when it comes to work with the PRU
| /sys/class/remoteproc/ | Pru Core |
|------------------------|----------|
| remoteproc4            | pru1_0   |
| remoteproc5            | pru1_1   |
| remoteproc6            | pru2_0   |
| remoteproc7            | pru2_1   |

## Resources websites and repository:
Here is a list of the website and Github repository used to create those examples:
- [Element 14 Beaglebone AI Survival Guide](https://www.element14.com/community/community/project14/visionthing/blog/2019/11/16/beagleboard-ai-brick-recovery-procedure#jive_content_id_analogInc)
- [Mark A. Yoder, PRU Cookbook](http://beagleboard.org/static/prucookbook/)
- [Cloud 9 Beaglebone AI PRU examples](https://github.com/beagleboard/cloud9-examples/tree/v2020.01/BeagleBone/AI/pru)
