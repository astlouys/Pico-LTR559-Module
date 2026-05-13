/* ============================================================================================================================================================= *\
   Pico-LTR559-Module.c
   St-Louys Andre - September 2025
   astlouys@gmail.com
   https://github.com/astlouys/Pico-ASTL-BME280
   Revision 06-APR-2026
   Langage: C
   Version 1.01

   Raspberry Pi Pico C-Language module to add a LTR559 device to a user program.

   NOTE:
   This program is provided without any warranty of any kind. It is provided
   simply to help the user develop his own program.

   REVISION HISTORY:
   =================
   25-SEP-2025 1.00 - Initial release.
   25-FEB-2026 1.01 - Fine tune ambient light reading function.
\* ============================================================================================================================================================= */

/* ============================================================================================================================================================= *\
                                                                            Light intensity
     One lux means that 1 square meter is uniformly lighted by 1 lumen. So, when a light bulb expressed on lumens reach a surface, lux means the light reaching
    the surface, for example a table. In the lounge, for a cozy atmosphere, 50 lux could be fine. In a bedroom or dining room, 300 lux is a confortable lighting.
                   In an office for reading, 500 lux is fine. On a desk or in the workshop to work on small components, 500 to 1000 lux are OK.
           In the house, during the night, 10 to 50 lux are enough to see the surroundings and walk safely. On the other end, during a sunny day in summer
     without clouds, ambient light level can reach up to 100,000 lux (20,000 lux for a cloudy sky) and in winter, a cloudy sky would reach 3,500 lux or 750 lux
                                        at sunset. During a night with full moon, ambient light would be around 0,25 lux.
\* ============================================================================================================================================================= */


/* $PAGE */
/* $TITLE=Include files. */
/* ============================================================================================================================================================= *\
                                                                               Include files.
\* ============================================================================================================================================================= */
#include "baseline.h"
#include "stdio.h"

#include "Pico-LTR559-Module.h"



/* $PAGE */
/* $TITLE=Definitions. */
/* ============================================================================================================================================================= *\
                                                                                Definitions.
\* ============================================================================================================================================================= */
#define RELEASE_VERSION



/* $PAGE */
/* $TITLE=Global variables. */
/* ============================================================================================================================================================= *\
                                                                              Global variables.
\* ============================================================================================================================================================= */
extern struct struct_ltr559 StructLTR559;



/* $PAGE */
/* $TITLE=Functions prototype. */
/* ============================================================================================================================================================= *\
                                                                             Function prototypes.
\* ============================================================================================================================================================= */
/* Input a string or a keystroke from stdin. */
extern void input_string(UCHAR *String, UINT16 StringSize, UINT32 TimeOutMSec);

/* Send data to log file. */
extern void log_printf(UINT LineNumber, const UCHAR *FunctionName, UCHAR *Format, ...);





/* $PAGE */
/* $TITLE=ltr559_display_registers() */
/* ============================================================================================================================================================= *\
                                                                  Display all LTR559 readable registers.
\* ============================================================================================================================================================= */
void ltr559_display_registers(void)
{
  log_printf(__LINE__, __func__, "Display all LTR559 registers.\n");
  log_printf(__LINE__, __func__, "===========================================\n");
  log_printf(__LINE__, __func__, "AlsControl:                  0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_CONTROL         - 0x80]);
  log_printf(__LINE__, __func__, "PsControl:                   0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_CONTROL          - 0x80]);
  log_printf(__LINE__, __func__, "PsLed:                       0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_LED              - 0x80]);
  log_printf(__LINE__, __func__, "PsNPulses:                   0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_N_PULSES         - 0x80]);
  log_printf(__LINE__, __func__, "PsMeasRate:                  0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_MEAS_RATE        - 0x80]);
  log_printf(__LINE__, __func__, "AlsMeasRate:                 0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_MEAS_RATE       - 0x80]);
  log_printf(__LINE__, __func__, "Part number:                 0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PART_NUMBER         - 0x80]);
  log_printf(__LINE__, __func__, "Manufacturer Id:             0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_MANUFACTURER_ID     - 0x80]);
  log_printf(__LINE__, __func__, "ALS data channel1 (0)        0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL1_0 - 0x80]);
  log_printf(__LINE__, __func__, "ALS data channel1 (1)        0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL1_1 - 0x80]);
  log_printf(__LINE__, __func__, "ALS data channel0 (0)        0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL0_0 - 0x80]);
  log_printf(__LINE__, __func__, "ALS data channel0 (1)        0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL0_1 - 0x80]);
  log_printf(__LINE__, __func__, "ALS & PS Status              0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS       - 0x80]);
  log_printf(__LINE__, __func__, "PS Data 0                    0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_DATA0            - 0x80]);
  log_printf(__LINE__, __func__, "PS Data 1                    0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_DATA1            - 0x80]);
  log_printf(__LINE__, __func__, "Interrupt:                   0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_INTERRUPT           - 0x80]);
  log_printf(__LINE__, __func__, "PsThresholdHi0:              0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_THRESHOLD_HI0    - 0x80]);
  log_printf(__LINE__, __func__, "PsThresholdHi1:              0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_THRESHOLD_HI1    - 0x80]);
  log_printf(__LINE__, __func__, "PsThresholdLo0:              0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_THRESHOLD_LOW0   - 0x80]);
  log_printf(__LINE__, __func__, "PsThresholdLo1:              0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_THRESHOLD_LOW1   - 0x80]);
  log_printf(__LINE__, __func__, "PsOffsetHi:                  0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_OFFSET1          - 0x80]);
  log_printf(__LINE__, __func__, "PsOffsetLo:                  0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_PS_OFFSET0          - 0x80]);
  log_printf(__LINE__, __func__, "Dummy1:                      0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_DUMMY1              - 0x80]);
  log_printf(__LINE__, __func__, "AlsThresholdHi0:             0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_THRESHOLD_HI0   - 0x80]);
  log_printf(__LINE__, __func__, "AlsThresholdHi1:             0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_THRESHOLD_HI1   - 0x80]);
  log_printf(__LINE__, __func__, "AlsThresholdLo0:             0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_THRESHOLD_LOW0  - 0x80]);
  log_printf(__LINE__, __func__, "AlsThresholdLo1:             0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_THRESHOLD_LOW1  - 0x80]);
  log_printf(__LINE__, __func__, "Dummy2:                      0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_DUMMY2              - 0x80]);
  log_printf(__LINE__, __func__, "Dummy3:                      0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_DUMMY3              - 0x80]);
  log_printf(__LINE__, __func__, "Dummy4:                      0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_DUMMY4              - 0x80]);
  log_printf(__LINE__, __func__, "InterruptPersist:            0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_INTERRUPT_PERSIST   - 0x80]);

  printf("\n");
  log_printf(__LINE__, __func__, "Channel0 light value (IR + visible): %5u\n", StructLTR559.Channel0);
  log_printf(__LINE__, __func__, "Channel2 light value (IR only):      %5u\n", StructLTR559.Channel1);
  printf("\n");

  if (StructLTR559.I2CPort == i2c0) log_printf(__LINE__, __func__, "I2C port used:    i2c0\n");
  if (StructLTR559.I2CPort == i2c1) log_printf(__LINE__, __func__, "I2C port used:    i2c1\n");
  log_printf(__LINE__, __func__, "GPIO for I2C data:  %2u\n", StructLTR559.GpioSDA);
  log_printf(__LINE__, __func__, "GPIO for I2C clock: %2u\n", StructLTR559.GpioSCL);

  return;
}





/* $PAGE */
/* $TITLE=ltr559_init() */
/* ============================================================================================================================================================= *\
                                                                      Initialize ltr559 sensor.
\* ============================================================================================================================================================= */
INT16 ltr559_init()
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all times.
#else  // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT8 Buffer[32];
  // UINT8 PortNumber;

  INT16 ReturnCode;

  // UINT16 Loop1UInt16;


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                           Initialize GPIO's.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering ltr559_init()\n");
    if (StructLTR559.I2CPort == i2c0) log_printf(__LINE__, __func__, "I2C port used:    i2c0\n");
    if (StructLTR559.I2CPort == i2c1) log_printf(__LINE__, __func__, "I2C port used:    i2c1\n");
    log_printf(__LINE__, __func__, "GPIO for I2C data:  %2u\n", StructLTR559.GpioSDA);
    log_printf(__LINE__, __func__, "GPIO for I2C clock: %2u\n", StructLTR559.GpioSCL);
  }
  sleep_ms(100);


  /* Initialize GPIOs used for LTR559. */
  i2c_init(StructLTR559.I2CPort, 400000);
	gpio_set_function(StructLTR559.GpioSDA, GPIO_FUNC_I2C);
	gpio_set_function(StructLTR559.GpioSCL, GPIO_FUNC_I2C);
	gpio_pull_up(StructLTR559.GpioSDA);
	gpio_pull_up(StructLTR559.GpioSCL);
  sleep_ms(100);

  
#if 0  // enable this part only to detect all I2C devices that are currently online.
  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                        Scan all I2C ports to find those that are active - debug.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    /* Scan all I2C ports to find those that are currently allocated to any I2C device (for debugging purposes). */
    /* ----------------------------------------------------------*\
       On Pimoroni's Multi-sensor stick:
       0x23 - LTR559 write address (light level and +/- 5cm proximity sensor).
       0x6A - LSM6DS3TR (accelerometer and gyroscope sensor).
       0x76 - BME280 (temperature, humidity and atmospheric pressure sensor).
       0xA3 - LTR559 read address (light level and +/- 5cm proximity sensor).
       0xEA - Unknown.
       0xF6 - Unknown.
    \* ----------------------------------------------------------*/
    Loop1UInt16 = 0;
    PortNumber  = 0;
    while (Loop1UInt16 < 256)
    {
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "Before sending a soft reset to LTR559 ALS_CONTROL. Port 0x%2.2X\n", PortNumber);
      Buffer[0] = LTR559_REGISTER_ALS_CONTROL;
      Buffer[1] = 0x02;  // soft reset.
      ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, PortNumber, Buffer, 2, true);
      if (ReturnCode == PICO_ERROR_GENERIC)
      {
        log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
      }
      else
      {
        log_printf(__LINE__, __func__, "Number of registers written (return code): %d\n", ReturnCode);
      }
      sleep_ms(300);
      ++PortNumber;
      ++Loop1UInt16;
    }
    printf("\n\n");
  }
#endif  // 0


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                        Perform a software reset on LTR559 and adjust ambient light level sensor detection gain.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* Reset ALS_CONTROL register (ambient light sensor). */
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Setting LTR559_REGISTER_ALS_CONTROL while resetting sensor in standby mode.\n");
  Buffer[0] = LTR559_REGISTER_ALS_CONTROL;
  Buffer[1] = 0x02;
  switch (StructLTR559.LightLevelGain)
  {
    case (LTR559_GAIN_1X):
      /* Nothing to do, LTR559 is initialized to 1X gain by default. */
      /* 1X gain = 1 lux to 64k lux */
    break;

    case (LTR559_GAIN_2X):
      /* 2X gain = 0.5 lux to 32k lux */
      Buffer[1] |= 0x04;
    break;

    case (LTR559_GAIN_4X):
      /* 4X gain = 0.25 lux to 16k lux */
      Buffer[1] |= 0x08;
    break;

    case (LTR559_GAIN_8X):
      /* 8X gain = 0.125 lux to 8k lux */
      Buffer[1] |= 0x0C;
    break;

    case (LTR559_GAIN_48X):
      /* 48X gain = 0.02 lux to 1.3k lux */
      Buffer[1] |= 0x18;
    break;

    case (LTR559_GAIN_96X):
      /* 96X gain = 0.01 lux to 600 lux */
      Buffer[1] |= 0x1C;
    break;
  }
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }
  sleep_ms(500);
  if (FlagLocalDebug)
  {
    ltr559_read_register(LTR559_REGISTER_ALS_CONTROL, 31);
    ltr559_display_registers();
    printf("\n\n");
    sleep_ms(500);
  }


  /* Initialize PS_LED register (proximity sensor). */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Setting LTR559_REGISTER_PS_LED to 0x49.\n");
    log_printf(__LINE__, __func__, "Pulse period: 50 kHz\n");
    log_printf(__LINE__, __func__, "Duty cycle: 50\n");
    log_printf(__LINE__, __func__, "LED pulse current: 10 mA\n");
  }
  Buffer[0] = LTR559_REGISTER_PS_LED;
  Buffer[1] = 0x49;
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }
  sleep_ms(500);
  if (FlagLocalDebug)
  {
    ltr559_read_register(LTR559_REGISTER_ALS_CONTROL, 31);
    ltr559_display_registers();
    printf("\n\n");
    sleep_ms(500);
  }


  /* Initialize PS_MEAS_RATE (PS measurement repeat rate) register. */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Setting LTR559_REGISTER_PS_MEAS_RATE to 0x07\n");
    log_printf(__LINE__, __func__, "Measurement rate: 2000 msec while in active mode\n");
  }
  Buffer[0] = LTR559_REGISTER_PS_MEAS_RATE;
  Buffer[1] = 0x07;  // 2000 msec.
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }
  sleep_ms(100);
  if (FlagLocalDebug)
  {
    ltr559_read_register(LTR559_REGISTER_ALS_CONTROL, 31);
    ltr559_display_registers();
    printf("\n\n");
    sleep_ms(300);
  }

  ltr559_read_register(LTR559_REGISTER_ALS_CONTROL, 31);
 
  return 0;
}





/* $PAGE */
/* $TITLE=ltr559_read_parameters() */
/* ============================================================================================================================================================= *\
                                                               Read ambient light level and proximity sensor.
\* ============================================================================================================================================================= */
INT16 ltr559_read_parameters(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all times.
#else  // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR String[32];

  UINT8 Buffer[2];
  UINT8 CurrentGainPattern;
  UINT8 RatioScale;

  INT16 ReturnCode;

  UINT16 Channel0;
  UINT16 Channel1;
  UINT16 Ratio;

  // float RatioF;
  // float Value;


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                      Read ambient light level.
                       NOTE: Channel 0 is sensitive to both visible and infrared light whereas channel1 is sensitive only to infrared light.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Toggling ALS_CONTROL in active mode.\n");
  Buffer[0] = LTR559_REGISTER_ALS_CONTROL;
  Buffer[1] = 0x01;  // toggle ALS_CONTROL in active mode.
  switch (StructLTR559.LightLevelGain)
  {
    case (LTR559_GAIN_1X):
      /* Nothing to do, LTR559 is initialized to 1X gain by default. */
    break;

    case (LTR559_GAIN_2X):
      Buffer[1] |= 0x04;
    break;

    case (LTR559_GAIN_4X):
      Buffer[1] |= 0x08;
    break;

    case (LTR559_GAIN_8X):
      Buffer[1] |= 0x0C;
    break;

    case (LTR559_GAIN_48X):
      Buffer[1] |= 0x18;
    break;

    case (LTR559_GAIN_96X):
      Buffer[1] |= 0x1C;
    break;
  }
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "LTR559 light gain has been set to %uX\n", StructLTR559.LightLevelGain);
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }
  sleep_ms(100);  // ALS integration rate is 100 msec.

  
  /* Loop on status register until a new light level value is available for reading. */
  do
  {
    /* Wait until register ALS_DATA_STATUS is set. */
    ltr559_read_register(LTR559_REGISTER_ALS_PS_STATUS, 1);
    if (StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80] & 0x80)
    {
      log_printf(__LINE__, __func__, "Ambient light data values are invalid (0x%2.2X)\n", StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80]);
      log_printf(__LINE__, __func__, "Wait until light level available: StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80]: 0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80]);
    }
    sleep_ms(50);
  } while (!(StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80] & 0x04));


  if (FlagLocalDebug)
  {
    CurrentGainPattern = (StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80] >> 4) & 0x07;
    switch (CurrentGainPattern)
    {
      case (0):
        sprintf(String, "1X");
        if (StructLTR559.LightLevelGain != 1)
          log_printf(__LINE__, __func__, "Discrepancy between light gain setup and light gain returned by LTR559 (%u and 1X).\n", StructLTR559.LightLevelGain);
      break;

      case (1):
        sprintf(String, "2X");
        if (StructLTR559.LightLevelGain != 2)
          log_printf(__LINE__, __func__, "Discrepancy between light gain setup and light gain returned by LTR559 (%u and 2X).\n", StructLTR559.LightLevelGain);
      break;

      case (2):
        sprintf(String, "4X");
        if (StructLTR559.LightLevelGain != 4)
          log_printf(__LINE__, __func__, "Discrepancy between light gain setup and light gain returned by LTR559 (%u and 4X).\n", StructLTR559.LightLevelGain);
      break;

      case (3):
        sprintf(String, "8X");
        if (StructLTR559.LightLevelGain != 8)
          log_printf(__LINE__, __func__, "Discrepancy between light gain setup and light gain returned by LTR559 (%u and 8X).\n", StructLTR559.LightLevelGain);
      break;

      default:
      case (4):
      case (5):
        sprintf(String, "invalid value");
      break;

      case (6):
        sprintf(String, "48X");
        if (StructLTR559.LightLevelGain != 48)
          log_printf(__LINE__, __func__, "Discrepancy between light gain setup and light gain returned by LTR559 (%u and 48X).\n", StructLTR559.LightLevelGain);
      break;

      case (7):
        sprintf(String, "96X");
        if (StructLTR559.LightLevelGain != 96)
          log_printf(__LINE__, __func__, "Discrepancy between light gain setup and light gain returned by LTR559 (%u and 96X).\n", StructLTR559.LightLevelGain);
      break;
    }
    log_printf(__LINE__, __func__, "Current ambient light gain pattern returned by sensor: 0x%X, corresponding to %s (%u)\n", CurrentGainPattern, String, StructLTR559.LightLevelGain);
    log_printf(__LINE__, __func__, "Before reading ambient light level value.\n");
  }


  /* Read ambient light level value in the right sequence. */
  ltr559_read_register(LTR559_REGISTER_ALS_DATA_CHANNEL1_0, 4);
  if (FlagLocalDebug) 
  {
    log_printf(__LINE__, __func__, "LTR559_REGISTER_ALS_DATA_CHANNEL0 (IR + visible):   0x%2.2X   0x%2.2X\n",
               StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL0_1 - 0x80], StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL0_0 - 0x80]);
    log_printf(__LINE__, __func__, "LTR559_REGISTER_ALS_DATA_CHANNEL1 (IR only):        0x%2.2X   0x%2.2X\n",
               StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL1_1 - 0x80], StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL1_0 - 0x80]);
  }


  /* Convert the two channels' 8-bit values to their corresponding 16-bit values. */
  Channel0 = StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL0_0 - 0x80] + (StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL0_1 - 0x80] << 8);;
  Channel1 = StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL1_0 - 0x80] + (StructLTR559.Register[LTR559_REGISTER_ALS_DATA_CHANNEL1_1 - 0x80] << 8);
  StructLTR559.Channel0 = Channel0;
  StructLTR559.Channel1 = Channel1;
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Channel0 (16 bits) (IR and visible):              0x%4.4X   (%u)\n", Channel0, Channel0);
    log_printf(__LINE__, __func__, "Channel1 (16 bits) (IR only):                     0x%4.4X   (%u)\n", Channel1, Channel1);
  }
  if ((Channel0 == 0) && (Channel1 == 0))
  {
    RatioScale = 1;
    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Both channels are 0\n");
    StructLTR559.Lux = 0.0;
    Ratio = 1000;
  }
  else
  {
    /* Convert raw register values read to light level in lux. */
    // log_printf(__LINE__, __func__, "Channel1 * 1000:     0x%X   (%u)\n", (Channel1 * 1000), (Channel1 * 1000));
    // log_printf(__LINE__, __func__, "Channel0 + Channel1: 0x%X   (%u)\n", (Channel0 + Channel1), (Channel0 + Channel1));
    // log_printf(__LINE__, __func__, "Ratio: A / B: 0x%X   (%u)\n", ((Channel1 * 1000) / (Channel0 + Channel1)), ((Channel1 * 1000) / (Channel0 + Channel1)));
 
    Ratio = (Channel1 * 1000) / (Channel0 + Channel1);

    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Ratio: %u\n", Ratio);
    if (Ratio < 450)
    {
      StructLTR559.Lux = ((17743.0 * Channel0) + (11059.0 * Channel1));
      StructLTR559.Lux /= StructLTR559.LightLevelGain;
      StructLTR559.Lux /= 10000;
      if (FlagLocalDebug)
      {
        RatioScale = 2;
        log_printf(__LINE__, __func__, "  17743 * %u: %f\n", Channel0, 17743.0 * Channel0);
        log_printf(__LINE__, __func__, "  11059 * %u: %f\n", Channel1, 11059.0 * Channel1);
        log_printf(__LINE__, __func__, " (17743 * %u) + (11059 * %u): %f\n", Channel0, Channel1, (17743.0 * Channel0) + (11059.0 * Channel1));
        log_printf(__LINE__, __func__, "((17743 * %u) + (11059 * %u)) / %u: %f\n", Channel0, Channel1, StructLTR559.LightLevelGain, ((17743.0 * Channel0) + (11059.0 * Channel1)) / StructLTR559.LightLevelGain);
        log_printf(__LINE__, __func__, "((17743 * %u) + (11059 * %u)) / %u / 10000: %f\n", Channel0, Channel1, StructLTR559.LightLevelGain, (((17743.0 * Channel0) + (11059.0 * Channel1)) / StructLTR559.LightLevelGain) / 10000);
        log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
      }
    }
    else if (Ratio < 640)
    {
      StructLTR559.Lux = (((37725.0 * Channel0) - (13363.0 * Channel1)) / 10000);
      if (FlagLocalDebug)
      {
        RatioScale = 3;
        log_printf(__LINE__, __func__, "  37725 * %u: %f\n", Channel0, 37725.0 * Channel0);
        log_printf(__LINE__, __func__, "  13363 * %u: %f\n", Channel1, 13363.0 * Channel1);
        log_printf(__LINE__, __func__, " (37725 * %u) + (13363 * %u): %f\n", Channel0, Channel1, (37725.0 * Channel0) + (13363.0 * Channel1));
        log_printf(__LINE__, __func__, "((37725 * %u) + (13363 * %u)) / %u: %f\n", Channel0, Channel1, StructLTR559.LightLevelGain, ((37725.0 * Channel0) + (13363.0 * Channel1)) / StructLTR559.LightLevelGain);
        log_printf(__LINE__, __func__, "((37725 * %u) + (13363 * %u)) / %u / 10000: %f\n", Channel0, Channel1, StructLTR559.LightLevelGain, (((37725.0 * Channel0) + (13363.0 * Channel1)) / StructLTR559.LightLevelGain) / 10000);
        log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
      }
    }
    else if (Ratio < 850)
    {
      StructLTR559.Lux = (((16900.0 * Channel0) - (1690.0 * Channel1)) / 10000);
      if (FlagLocalDebug)
      {
        RatioScale = 4;
        log_printf(__LINE__, __func__, "  16900 * %u: %f\n", Channel0, 16900.0 * Channel0);
        log_printf(__LINE__, __func__, "   1690 * %u: %f\n", Channel1,  1690.0 * Channel1);
        log_printf(__LINE__, __func__, " (16900 * %u) + (1690 * %u): %f\n", Channel0, Channel1, (16900.0 * Channel0) + (1690.0 * Channel1));
        log_printf(__LINE__, __func__, "((16900 * %u) + (1690 * %u)) / %u: %f\n", Channel0, Channel1, StructLTR559.LightLevelGain, ((16900.0 * Channel0) + (1690.0 * Channel1)) / StructLTR559.LightLevelGain);
        log_printf(__LINE__, __func__, "((16900 * %u) + (1690 * %u)) / %u / 10000: %f\n", Channel0, Channel1, StructLTR559.LightLevelGain, (((16900.0 * Channel0) + (1690.0 * Channel1)) / StructLTR559.LightLevelGain) / 10000);
        log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
      }
    }
    else
    {
      StructLTR559.Lux = 0.0;
      if (FlagLocalDebug)
      {
        RatioScale = 5;
        log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
      }
    }
  }


  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "===============================================================================================================================\n");
    log_printf(__LINE__, __func__, "Channel0 (IR and visible):     0x%4.4X (%u)\n", Channel0, Channel0);
    log_printf(__LINE__, __func__, "Channel1 (IR only):            0x%4.4X (%u)\n", Channel1, Channel1);
    log_printf(__LINE__, __func__, "Ratio calculation scale:         %4u\n", RatioScale);
    log_printf(__LINE__, __func__, "Ratio:                           %4u\n", Ratio);
    log_printf(__LINE__, __func__, "Light level:                    %10.4f lux\n", StructLTR559.Lux);
    log_printf(__LINE__, __func__, "===============================================================================================================================\n");
  }


#if 0
  log_printf(__LINE__, __func__, "===============================================================================================================================\n");
  log_printf(__LINE__, __func__, "Method 2\n");
  if ((Channel0 == 0) && (Channel1 == 0))
  {
    Ratio = 0;
    StructLTR559.Lux = 0.0;
  }
  else
  {
    /* Convert raw register values read to light level in lux. */
    Ratio = (Channel1 * 100) / (Channel0 + Channel1);
    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Ratio: %u\n", Ratio);
    if (Ratio < 45)
    {
      StructLTR559.Lux = (((17743.0 * Channel0) + (11059.0 * Channel1)) / StructLTR559.LightLevelGain) / 10000;
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
    }
    else if (Ratio < 64)
    {
      StructLTR559.Lux = (((42785.0 * Channel0) - (19548.0 * Channel1)) / StructLTR559.LightLevelGain) / 10000;
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
    }
    else if (Ratio < 85)
    {
      StructLTR559.Lux = (((5926.0 * Channel0) + (1185.0 * Channel1)) / StructLTR559.LightLevelGain) / 10000;
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "StructLTR559.Lux: %f\n", StructLTR559.Lux);
    }
    else
    {
      StructLTR559.Lux = 0.0 ;
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "StructLTR559.Lux: %f lux\n", StructLTR559.Lux);
    }
  }


  log_printf(__LINE__, __func__, "Method 3\n");
  if (Channel0 + Channel1 == 0)
  {
    RatioF = 1000.0;
    log_printf(__LINE__, __func__, "RatioF = 1000.0\n");
  }
  else
  {
    RatioF = (Channel1 * 1000.0) / (float)(Channel0 + Channel1);
    log_printf(__LINE__, __func__, "RatioF = %f\n", RatioF);
  }



  if (RatioF < 450.0)
  {
    log_printf(__LINE__, __func__, "RatioF ------------------------------- 1\n");
    Value = ((Channel0 * 17743.0) - (Channel1 * -11059.0)) / StructLTR559.LightLevelGain / 10000.0;
  }
  else if (RatioF < 640.0)
  {
    log_printf(__LINE__, __func__, "RatioF ------------------------------- 2\n");
    Value = ((Channel0 * 42785.0) - (Channel1 * 19548.0)) / StructLTR559.LightLevelGain / 10000.0;
  }
  else if (RatioF < 850.0)
  {
    log_printf(__LINE__, __func__, "RatioF ------------------------------- 3\n");
    Value = ((Channel0 * 5926.0) - (Channel1 * -1185.0)) / StructLTR559.LightLevelGain / 10000.0;
  }
  else
  {
    log_printf(__LINE__, __func__, "RatioF ------------------------------- 4\n");
    Value = ((Channel0 * 0.0) - (Channel1 * 0)) / StructLTR559.LightLevelGain / 10000.0;
  }
  log_printf(__LINE__, __func__, "Lux value: %f\n", Value);

  log_printf(__LINE__, __func__, "===============================================================================================================================\n\n");
#endif  // 0  



  /* Toggle ALS sensor in stand-by mode when done. */
  // log_printf(__LINE__, __func__, "Toggling back ALS_CONTROL in standby mode.\n");
  Buffer[0] = LTR559_REGISTER_ALS_CONTROL;
  Buffer[1] = 0x00;  // toggle ALS_CONTROL in standby mode.
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }





  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                        Read proximity sensor.
     NOTE: Converting the LTR559 raw proximity values into real-world distance - if required - must be done in the specific environment in which it is operated.
           The sensor will detect ambient infrared light and some other interference and the raw values will also be dependant on the reflective value of the
           surface in which the infrared LED light is reflected, the angle at which the light is reflected, etc... Consequently, calibration must be done in the
           operating environment. Also, this device will only detect objects very close to the sensor (more or less 10 cm or less). It's easy to understand when
           we know that one popular use for this sensor is to disable the screen keyboard of a cellular phone (portable phone) when we approach it from our ear.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* Toggle PS_CONTROL in active mode. */
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Toggling PS_CONTROL to active mode.\n");
  Buffer[0] = LTR559_REGISTER_PS_CONTROL;
  Buffer[1] = 0x03;
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }
  sleep_ms(400);


  /* Loop until PS_DATA_STATUS is set: a new proximity sensor value is available for reading. */
  do
  {
    ltr559_read_register(LTR559_REGISTER_ALS_PS_STATUS, 1);
    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Wait until proximity sensor value available: StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80]: 0x%2.2X\n", StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80]);
    sleep_ms(50);
  } while (!(StructLTR559.Register[LTR559_REGISTER_ALS_PS_STATUS - 0x80] & 0x01));


  /* Read proximity sensor value in the right sequence. */
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Reading proximity sensor value (2 simultaneous registers).\n");
  StructLTR559.Register[0] = LTR559_REGISTER_PS_DATA0;  // first register to read.
  i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, &StructLTR559.Register[0], 1, true);
  ltr559_read_register(LTR559_REGISTER_PS_DATA0, 2);
  if (FlagLocalDebug) 
  {
    log_printf(__LINE__, __func__, "LTR559_REGISTER_PS_DATA:    0x%2.2X - 0x%2.2X\n", 
               StructLTR559.Register[LTR559_REGISTER_PS_DATA1 - 0x80], StructLTR559.Register[LTR559_REGISTER_PS_DATA0 - 0x80]);
  }
  StructLTR559.Proximity = StructLTR559.Register[LTR559_REGISTER_PS_DATA0 - 0x80] + ((StructLTR559.Register[LTR559_REGISTER_PS_DATA1 - 0x80] & 0x03) << 8);
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Proximity value: 0x%4.4lX (%u)\n", StructLTR559.Proximity, StructLTR559.Proximity);
  sleep_ms(400);


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Toggling PS_CONTROL to stand-by when done.\n");
  Buffer[0] = LTR559_REGISTER_PS_CONTROL;
  Buffer[1] = 0x00;
  ReturnCode = i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, Buffer, 2, true);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to communicate with LTR559 through I2C protocol.\n");
    return -1;
  }
  sleep_ms(400);

  return 0;
}





/* $PAGE */
/* $TITLE=ltr559_read_register() */
/* ============================================================================================================================================================= *\
                                                        Read one or more specified register(s) via I2C protocol.
\* ============================================================================================================================================================= */
INT16 ltr559_read_register(UINT8 StartRegisterAddress, UINT8 NbOfRegisters)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all times.
#else  // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  INT8 ReturnCode;

  UINT8 Buffer;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering ltr559_read_register()   (0x%2.2X - %u)\n", StartRegisterAddress, NbOfRegisters);


  if ((StartRegisterAddress + NbOfRegisters) > LTR559_REGISTER_END)
  {
    log_printf(__LINE__, __func__, "Start address (0x%2.2X) and number of registers (%u) would go out of register limit... Aborting operation...\n", StartRegisterAddress, NbOfRegisters);
    return -1;
  }


  /* Send address of first register to read. */
  Buffer = StartRegisterAddress;
  i2c_write_blocking(StructLTR559.I2CPort, StructLTR559.WriteAddress, &Buffer, 1, true);

  /* Then, read requested registers. */
  ReturnCode = i2c_read_blocking(StructLTR559.I2CPort, StructLTR559.ReadAddress, &StructLTR559.Register[StartRegisterAddress - 0x80], NbOfRegisters, false);
  if (ReturnCode == PICO_ERROR_GENERIC)
  {
    log_printf(__LINE__, __func__, "Error while trying to read starting with register 0x%2.2X from LTR559 through I2C protocol.\n", StartRegisterAddress);
    return -1;
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "i2c_read_blocking return code (number of registers read): %d\n", ReturnCode);

  return 0;
}
