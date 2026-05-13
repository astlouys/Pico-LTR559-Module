/* ============================================================================================================================================================= *\
   Pico-LTR559-Module.h
   St-Louys Andre - September 2025
   astlouys@gmail.com
   Revision 06-APR-2026
   Langage: C
\* ============================================================================================================================================================= */

#ifndef __PICO_LTR559_MODULE_H
#define __PICO_LTR559_MODULE_H



/* $PAGE */
/* $TITLE=Include files. */
/* ============================================================================================================================================================= *\
                                                                      Include files.
\* ============================================================================================================================================================= */
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "math.h"
#include "stdlib.h"



/* $PAGE */
/* $TITLE=Definitions. */
/* ============================================================================================================================================================= *\
                                                                        Definitions.
\* ============================================================================================================================================================= */
#define LTR559_READ_ADDRESS    0xA3  // LTR559 device  read address
#define LTR559_WRITE_ADDRESS   0x23  // LTR559 device write address

/* Ambient light level detection gain for LTR559. */
#define LTR559_GAIN_1X     1  // 1     lux to 64k   lux
#define LTR559_GAIN_2X     2  // 0.5   lux to 32k   lux
#define LTR559_GAIN_4X     4  // 0.25  lux to 16k   lux
#define LTR559_GAIN_8X     8  // 0.125 lux to  8k   lux
#define LTR559_GAIN_48X   48  // 0.02  lux to  1.3k lux
#define LTR559_GAIN_96X   96  // 0.01  lux to  600  lux

/* List of LTR559 internal registers. */
#define LTR559_REGISTER_ALS_CONTROL          0x80  // light sensor control register.
#define LTR559_REGISTER_PS_CONTROL           0x81  // proximity sensor control register.
#define LTR559_REGISTER_PS_LED               0x82  // proximity sensor LED frequency, duty cycle and peak current.
#define LTR559_REGISTER_PS_N_PULSES          0x83  // proximity sensor number of LED pulses emitted.
#define LTR559_REGISTER_PS_MEAS_RATE         0x84  // proximity sensor measurement repeat rate.
#define LTR559_REGISTER_ALS_MEAS_RATE        0x85  // light sensor measurement repeat rate (in active mode).
#define LTR559_REGISTER_PART_NUMBER          0x86  // sensor part number and revision identification.
#define LTR559_REGISTER_MANUFACTURER_ID      0x87  // sensor manufacturer idntification.
#define LTR559_REGISTER_ALS_DATA_CHANNEL1_0  0x88  // light sensor data base address (0x88 / 0x89 / 0x8A / 0x8B)
#define LTR559_REGISTER_ALS_DATA_CHANNEL1_1  0x89  // light sensor data base address (0x88 / 0x89 / 0x8A / 0x8B)
#define LTR559_REGISTER_ALS_DATA_CHANNEL0_0  0x8A  // light sensor data base address (0x88 / 0x89 / 0x8A / 0x8B)
#define LTR559_REGISTER_ALS_DATA_CHANNEL0_1  0x8B  // light sensor data base address (0x88 / 0x89 / 0x8A / 0x8B)
#define LTR559_REGISTER_ALS_PS_STATUS        0x8C  // light sensor and proximity sensor current status.
#define LTR559_REGISTER_PS_DATA0             0x8D  // proximity sensor data base address (0x8D / 0x8E).
#define LTR559_REGISTER_PS_DATA1             0x8E  // proximity sensor data base address (0x8D / 0x8E).
#define LTR559_REGISTER_INTERRUPT            0x8F  // sensor's interrupt pin control.
#define LTR559_REGISTER_PS_THRESHOLD_HI0     0x90  // proximity sensor sensibility interrupt threshold (0x90 / 0x91 / 0x92 / 0x93).
#define LTR559_REGISTER_PS_THRESHOLD_HI1     0x91  // proximity sensor sensibility interrupt threshold (0x90 / 0x91 / 0x92 / 0x93).
#define LTR559_REGISTER_PS_THRESHOLD_LOW0    0x92  // proximity sensor sensibility interrupt threshold (0x90 / 0x91 / 0x92 / 0x93).
#define LTR559_REGISTER_PS_THRESHOLD_LOW1    0x93  // proximity sensor sensibility interrupt threshold (0x90 / 0x91 / 0x92 / 0x93).
#define LTR559_REGISTER_PS_OFFSET1           0x94  // proximity sensor offset compensation value base address (0x94 / 0x95).
#define LTR559_REGISTER_PS_OFFSET0           0x95  // proximity sensor offset compensation value base address (0x94 / 0x95).
#define LTR559_REGISTER_DUMMY1               0x96  // undefined register position.
#define LTR559_REGISTER_ALS_THRESHOLD_HI0    0x97  // light sensor threshold base address (0x97 / 0x98 / 0x99 / 0x9A).
#define LTR559_REGISTER_ALS_THRESHOLD_HI1    0x98  // light sensor threshold base address (0x97 / 0x98 / 0x99 / 0x9A).
#define LTR559_REGISTER_ALS_THRESHOLD_LOW0   0x99  // light sensor threshold base address (0x97 / 0x98 / 0x99 / 0x9A).
#define LTR559_REGISTER_ALS_THRESHOLD_LOW1   0x9A  // light sensor threshold base address (0x97 / 0x98 / 0x99 / 0x9A).
#define LTR559_REGISTER_DUMMY2               0x9B  // undefined register position.
#define LTR559_REGISTER_DUMMY3               0x9C  // undefined register position.
#define LTR559_REGISTER_DUMMY4               0x9D  // undefined register position.
#define LTR559_REGISTER_INTERRUPT_PERSIST    0x9E  // out-of-range counter.
#define LTR559_REGISTER_END                  0x9F  // tag indicating end of register limit.



/* $PAGE */
/* $TITLE=Variable definitions. */
/* ============================================================================================================================================================= *\
                                                                      Variable definitions.
\* ============================================================================================================================================================= */
/* Main structure containing all BME280-related data. */
struct struct_ltr559
{
  /* Must be initialized by user for specific project values. */
  i2c_inst_t *I2CPort;   // I2C port used (0 or 1).
  UINT8 GpioSDA;         // GPIO used for I2C data line.
  UINT8 GpioSCL;         // GPIO used for I2C clock line.
  UINT8 ReadAddress;     // LTR559 device read address.
  UINT8 WriteAddress;    // LTR559 device write address.
  UINT8 LightLevelGain;  // LTR559 ambient light level sensor gain.

  /* Generic parameters. */
  UINT32 Errors;         // cumulative number of errors while trying to read BME280 sensor.
  UINT32 ReadCycles;     // cumulative number of read cycles from BME280.

  /* Sensor values. */
  UINT16 Channel0;       // channel 0 raw value (for both visible and infrared light).
  UINT16 Channel1;       // channel 1 raw value (for infrared light only).
  DOUBLE Lux;            // current ambient light level in lux.
  UINT16 Proximity;      // relative proximity value.

  /* Readable registers. */
  UINT8 Register[31];
};



/* $PAGE */
/* $TITLE=Function prototypes. */
/* ============================================================================================================================================================= *\
                                                                      Function prototypes.
\* ============================================================================================================================================================= */
/* Display all LTR559 readable registers. */
void ltr559_display_registers(void);

/* If the device is a Pimoroni "multi-sensor stick", there is a light sensor integrated with the BME280. */
INT16 ltr559_init(void);

/* Read ambient brightness level. */
INT16 ltr559_read_parameters(void);

/* Read a specified register via I2C protocol. */
INT16 ltr559_read_register(UINT8 StartRegisterAddress, UINT8 NbOfRegister);

#endif  // __PICO_LTR559_MODULE_H
