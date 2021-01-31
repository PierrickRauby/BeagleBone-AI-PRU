/*
 * simple_ping_pong_rpmsg.pru1_1.c
 * Modified by Pierrick Rauby < PierrickRauby - pierrick.rauby@gmail.com >
 * Based on the cloud9 examples:
 * https://github.com/jadonk/cloud9-examples/blob/master/BeagleBone/AI/pru/blinkInternalLED.pru1_1.c
 * The cloud 9 examples was modified to blink one User LED 
 * User LED with inscript D3 will blink with 1 second intervals
 * To compile use: make 
 * To deploy use: sh ./run.sh
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>            // atoi
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include "resource_table_1.h"
#include "init_pins_empty.h"
#include "prugpio.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT            ((uint32_t) 1 << 31)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
    * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
     * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
      * Be sure to change the values in resource_table_0.h too.
       */
#define TO_ARM_HOST            18    
#define FROM_ARM_HOST        19

// To blink the LED 
//Points to the two GPIO ports that are used
uint32_t *gpio5 = (uint32_t *)GPIO5;
/*
   * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
   * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
   */
#define CHAN_NAME            "rpmsg-pru"
#define CHAN_DESC            "Channel 30"
#define CHAN_PORT            30

/*
    * Used to make sure the Linux drivers are ready for RPMsg communication
     * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
      */
#define VIRTIO_CONFIG_S_DRIVER_OK    4

char payload[RPMSG_BUF_SIZE];



void main(void) {

    struct pru_rpmsg_transport transport;
    uint16_t src, dst, len;
    volatile uint8_t *status;


    /* Allow OCP master port access by the PRU so the PRU can read external memories */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    // This line is different between AM335x and Am5729 
    CT_INTC.SICR_bit.STATUS_CLR_INDEX = FROM_ARM_HOST;

    /* Make sure the Linux drivers are ready for RPMsg communication */
    status = &resourceTable.rpmsg_vdev.status;

    // Blink the Usr LED to say Hi ! 

    int i;
    for(i=0;i<5;++i){
      gpio5[GPIO_SETDATAOUT]   = USR1;  // Turn the USR1 LED on
      __delay_cycles(500000000/5);   // Wait 1/2 second
      gpio5[GPIO_CLEARDATAOUT] = USR1;  // Off
      __delay_cycles(500000000/5); 
    }

    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

    /* Initialize the RPMsg transport structure */
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

    // I checked with LED, I go up to here at least 

    /* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
    // This seems to work according to dmesg 
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);

    while (1) {
      /* Check bit 30 of register R31 to see if the ARM has kicked us */
      if (__R31 & HOST_INT) {
        for(i=0;i<5;++i){
          gpio5[GPIO_SETDATAOUT]   = USR1;  // Turn the USR1 LED on
          __delay_cycles(500000000/5);   // Wait 1/2 second
          gpio5[GPIO_CLEARDATAOUT] = USR1;  // Off
          __delay_cycles(500000000/5); 
        }
        /* Clear the event status */
    CT_INTC.SICR_bit.STATUS_CLR_INDEX = FROM_ARM_HOST;
    while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {

        /* echo the message back to the same address from which we just received */
        strcpy((char *)payload, "hello cortex-a8!");
        pru_rpmsg_send(&transport, dst, src, payload, 16);
    }
      }
    }
}

/*
 * Directly from the rpmsg hello This was not working for some reason, most likely a difference between am5729 and am335x
*/

//  struct pru_rpmsg_transport transport;
//  uint16_t src, dst, len;
//  volatile uint8_t *status;
//
//  /* Allow OCP master port access by the PRU so the PRU can read external memories */
//  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
//
//  /* Clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us */
//  // This line changes between AM335X and AM5729
//  CT_INTC.SICR_bit.STS_CLR_INDEX = FROM_ARM_HOST;
//
//  /* Make sure the Linux drivers are ready for RPMsg communication */
//  status = &resourceTable.rpmsg_vdev.status;
//  while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));
//
//
//  /* Initialize the RPMsg transport structure */
//  pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);
//
//  /* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
//  while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
//  while (1) {
//    /* Check bit 30 of register R31 to see if the ARM has kicked us */
//    if (__R31 & HOST_INT) {
//      /* clear the event status */
//      ct_intc.sicr_bit.sts_clr_idx = from_arm_host;
//      /* receive all available messages, multiple messages can be sent per kick */
//      while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == pru_rpmsg_success) {
//        /* echo the message back to the same address from which we just received */
//        strcpy((char *)payload, "hello cortex-a8!");
//        pru_rpmsg_send(&transport, dst, src, payload, 16);
//      }
//    }
//  }

/* 
 * The following part is commented has it is the blink LED example
 *
*/

/**/
   //Points to the two GPIO ports that are used
  /*uint32_t *gpio5 = (uint32_t *)GPIO5;*/
/**/
   //Clear SYSCFG[STANDBY_INIT] to enable OCP master port 
  /*CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;*/
/**/
  /*while(1) {*/
    /*gpio5[GPIO_SETDATAOUT]   = USR1;  // Turn the USR1 LED on*/
/**/
    /*__delay_cycles(1000000000/5);   // Wait 1/2 second*/
/**/
    /*gpio5[GPIO_CLEARDATAOUT] = USR1;  // Off*/
/**/
    /*__delay_cycles(1000000000/5); */
  /*}*/
  /*__halt();*/

// The following pragma was preventing the compilation with error "line 159: error #150: variable "init_pins" has already been initialized"

// Turns off triggers
//#pragma DATA_SECTION(init_pins, ".init_pins")
//#pragma RETAIN(init_pins)
//const char init_pins[] =  
//  "/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
//  "/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
//  "\0\0";
