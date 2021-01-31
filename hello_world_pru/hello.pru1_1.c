/*
 * hello.pru1_1.c
 * Modified by Pierrick Rauby < PierrickRauby - pierrick.rauby@gmail.com >
 * Based on the cloud9 examples:
 * https://github.com/jadonk/cloud9-examples/blob/master/BeagleBone/AI/pru/blinkInternalLED.pru1_1.c
 * The cloud 9 examples was modified to blink one User LED 
 * User LED with inscript D3 will blink with 1 second intervals
 * To compile use: make 
 * To deploy use: sh ./run.sh
*/


#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

volatile register unsigned int __R30;
volatile register unsigned int __R31;

void main(void) {
  // Points to the two GPIO ports that are used
  uint32_t *gpio5 = (uint32_t *)GPIO5;

  /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

  while(1) {
    gpio5[GPIO_SETDATAOUT]   = USR1;  // Turn the USR1 LED on

    __delay_cycles(1000000000/5);   // Wait 1/2 second

    gpio5[GPIO_CLEARDATAOUT] = USR1;  // Off

    __delay_cycles(1000000000/5); 
  }
  __halt();
}

// Turns off triggers
#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
  "/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
  "/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
  "\0\0";
