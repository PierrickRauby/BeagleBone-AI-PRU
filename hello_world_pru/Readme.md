# hello_world_pru

This example makes the PRU blink the user LED D3

## Contents:
```
├── Makefile: used to compile the C code into the PRU firmware
├── hello.pru1_1.c: once started the PRU1_1 will blink the user LED marked D3 on the Beaglebone AI
└── run.sh: compile hello.pru1_1.c using the Makefile, place the firmware under `/lib/firmware/am57xx-pru1_1-fw`, and restart the PRU. 
```

## How to use the example:
Once into the directory simply run:

```
sh run.sh
```

