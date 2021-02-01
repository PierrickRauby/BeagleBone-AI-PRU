# read_register_from_pru

This example use the PRU to read one of the GPIO registers with the PRU and send the stored value to the arm via rpmsg.
The default GPIO use for this example is GPIO5, so based on the [TRM](https://www.ti.com/product/AM5729) pages 7110 the registers important for this example are: 
| Register          | Offset      | Address for GPIO5 | Function                                          |
|-------------------|-------------|-------------------|---------------------------------------------------|
| GPIO_DATAOUT      | 0x0000 013C | 0x4805 B13C       | Read the value on the GPIO5 control register      |
| GPIO_CLEARDATAOUT | 0x0000 0190 | 0x4805 B190       | Clear the corresponding bits of the GPIO register |
| GPIO_SETDATAOUT   | 0x0000 0194 | 0x4805 B194       | Set the corresponding bits of the GPIO register   |



## Contents:
```
├── Readme.md
├── pru_codes
│   ├── Makefile
│   ├── read_add_from_pru.pru1_1.c
│   └── run.sh
└── user_space
    ├── Makefile
    └── simple_ping_pong_rpmsg_user_space.c

```
## How to use the example:
### First the PRU

To start the PRU code, go into the pru folder and run `run.sh`
```
cd pru_codes
sh run.sh

```
Once done, the pru is running the program and waiting for the ARM to send a message, on reception it will read the DATAOUT register  .k
### Then the ARM

Then go to the ARM  folder compile and run the codes

``` 
cd user_space
make clean
make 
./simple_ping_pong_user_space.pru1_1

```


