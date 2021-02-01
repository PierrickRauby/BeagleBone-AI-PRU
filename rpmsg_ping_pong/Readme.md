# rpmsg_ping_pong

This example present the use of rpmsg between PRU1_1 and the ARM Cortex. The ARM Cortex sends a message to the PRU and the PRU will answer to the ARM. 


## Contents:
```
├── pru_codes: 
│   ├── Makefile: compiles the PRU codes 
│   ├── run.sh: uses the Makefile to compile the codes, place the firmware and restart the PRU
│   ├── simple_ping_pong_rpmsg.pru1_1.c: PRU codes for the example
└── user_space
    ├── Makefile: compile the ARM code 
    └── simple_ping_pong_rpmsg_user_space.c: 
```
## How to use the example:
### First the PRU

To start the PRU code, go into the pru folder and run `run.sh`
```
cd pru_codes
sh run.sh
```
Once done, the pru is running the program and waiting for the ARM to send the first message.
You can check that it is actually working with the User LED 3 that blinks 5 times when the PRU starts. It will blink 5 times again when the ARM sends messages.
### Then the ARM

Then go to the ARM  folder compile and run the codes

``` 
cd user_space
make clean
make 
./simple_ping_pong_user_space.pru1_1
```


