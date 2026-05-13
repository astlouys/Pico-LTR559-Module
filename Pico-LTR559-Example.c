/* ============================================================================================================================================================= *\
   Pico-LTR559-Example.c
   St-Louys Andre - September 2025
   astlouys@gmail.com
   https://github.com/astlouys/Pico-MultiSensor-Module
   Revision 12-OCT-2025
   Langage: C
   Version 1.00

   Raspberry Pi Pico example on how to integrate a LTR559 device to a C-Language program.
   NOTE: This Firmware uses Pimoroni's Multi-Sensor Stick device which includes - among other things - a LTR559 sensor.
         If you are to use another device than the Pimoroni's Multi-Sensor Stick, some parameters may require changes
         (for example, the device I2C read and / or write address).

   NOTE:
   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION REGARDING
   THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, THE AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, 
   INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR
   THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCT.

   REVISION HISTORY:
   =================
   25-SEP-2025 1.00 - Initial release.
\* ============================================================================================================================================================= */



/* $PAGE */
/* $TITLE=List of GPIO usage. */
/* ============================================================================================================================================================= *\
                                                                     List of GPIO usage and pin-out.
\* ============================================================================================================================================================= */
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------- *\
Using Waveshare's 2-inch LCD display and Waveshare's BME280 module.
Pin 01 - GPIO  0  - Reserved for UART0 TX (Pico-ASTL-CallerId modem).
Pin 02 - GPIO  1  - Reserved for UART0 RX (Pico-ASTL-CallerId modem).
Pin 03 - GND      - Ground
Pin 04 - GPIO  2  - Button 2 (on Waveshare's 2-inch LCD display).
Pin 05 - GPIO  3  - Button 3 (on Waveshare's 2-inch LCD display).
Pin 06 - GPIO  4  - Reserved for UART1 TX (Pico-ASTL-SoundServer and Pico-ASTL-SPrinter).
Pin 07 - GPIO  5  - Reserved for UART1 RX (Pico-ASTL-SoundServer and Pico-ASTL-SPrinter).
Pin 08 - GND      - Ground
Pin 09 - GPIO  6  - Reserved for I2C1 SDA (data)  for BME280, LTR559 and other I2C devices.
Pin 10 - GPIO  7  - Reserved for I2C1 SCL (clock) for BME280, LTR559 and other I2C devices.
Pin 11 - GPIO  8  - LCD_DATA_COMMAND - data / command control pin  (on Waveshare's 2-inch LCD display).
Pin 12 - GPIO  9  - LCD_CHIP_SELECT  - chip select pin, active low (on Waveshare's 2-inch LCD display).
Pin 13 - GND      - Ground
Pin 14 - GPIO 10  - LCD_CLOCK        - SPI clock pin               (on Waveshare's 2-inch LCD display).
Pin 15 - GPIO 11  - LCD_DATA         - SPI  data pin               (on Waveshare's 2-inch LCD display).
Pin 16 - GPIO 12  - LCD_RESET        - reset pin, active low       (on Waveshare's 2-inch LCD display).
Pin 17 - GPIO 13  - LCD_BRIGHTNESS   - backlight pwm pin           (on Waveshare's 2-inch LCD display).
Pin 18 - GND      - Ground
Pin 19 - GPIO 14  - Reserved for Pico-ASTL-SoundServer status pin.
Pin 20 - GPIO 15  - Button 0 (on Waveshare's 2-inch LCD display).
Pin 21 - GPIO 16  - Reserved for infrared sensor / receiver (VS1838).
Pin 22 - GPIO 17  - Button 1 (on Waveshare's 2-inch LCD display).
Pin 23 - GND      - Ground
Pin 24 - GPIO 18  - Reserved for indicator LED.
Pin 25 - GPIO 19  - Reserved for tri-color LED (RED).
Pin 26 - GPIO 20  - Reserved for tri-color LED (GREEN).
Pin 27 - GPIO 21  - Reserved for tri-color LED (BLUE).
Pin 28 - GND      - Ground
Pin 29 - GPIO 22  - Reserved for Active or Passive buzzer.
Pin 30 - RUN      - Enable pin for RP2040. May be used to reset the Pico.
Pin 31 - GPIO 26  - Reserved for analog device (ADC0).
Pin 32 - GPIO 27  - Reserved for analog device (ADC1).
Pin 33 - GND      - ADC ground
Pin 34 - GPIO 28  - Reserved for analog device (ADC2).
Pin 35 - ADC Vref - ADC voltage reference.
Pin 36 - 3V3 out  - Regulated 3V3 to feed 3V3 devices (max 300mA).
Pin 37 - 3V3 en   - Connected to the SMPS enable pin and pulled high by a 100K resistor. Put it to Low to turn Off 3V3 to the RP2040.
Pin 38 - GND      - Ground
Pin 39 - VSYS 5V  - 5V out if powered by USB or main system input voltage (1.8V to 5.5V) if powered externally. SMPS will generate 3V3 for the RP2040 and GPIOs.
Pin 40 - VBUS 5V  - 5V input voltage coming from Pico's USB connector.

GPIO 23 - Low for high efficiency (PFM) or High for improved ripple (PWM).
GPIO 24 - VBUS sense. Detect USB power or VBus pin (High if VBUS is present, Low otherwise).
GPIO 25 - Pico's LED (output)
GPIO 29 - ADC3 - VSYS sense. Read VSYS / 3 through resistor divider and Q1.
GPIO 30 - ADC4 - Read Pico's internal temperature sensor.

Test point 1 - Ground.
Test point 2 - USB DM  - USB data negative pin.
Test point 3 - USB DP  - USB data positive pin.
Test point 4 - GPIO 23 - Switched Mode Power Supply (SMPS) Pin. Controls the on-board SMPS power save pin.
Test point 5 - GPIO 25 - Pico's LED.
Test point 6 - Bootsel (upload mode).

SPI port: spi1
\* ------------------------------------------------------------------------------------------------------------------------------------------------------------- */



/* $PAGE */
/* $TITLE=Definitions and macros. */
/* ============================================================================================================================================================= *\
                                                                       Definitions and macros.
\* ============================================================================================================================================================= */
/* Firmware version. */
#define FIRMWARE_VERSION "1.00"

/* I2C protocol related data. Those values are not put in Pico-LTR559-Module.h since different projects may use different parameters (but the same source code). */
#define I2C_PORT                 i2c1
#define I2C_DATA                    6  // GPIO for I2C data.
#define I2C_CLOCK                   7  // GPIO for I2C clock.
// #define LTR559_READ_ADDRESS   0xA3  // LTR559 device  read address
// #define LTR559_WRITE_ADDRESS  0x23  // LTR559 device write address
#define LIGHT_LEVEL_GAIN       LTR559_GAIN_4X



/* $PAGE */
/* $TITLE=Include files. */
/* ============================================================================================================================================================= *\
                                                                          Include files
\* ============================================================================================================================================================= */
#include "baseline.h"
#include "hardware/rtc.h"
#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "Pico-LTR559-Module.h"



/* $PAGE */
/* $TITLE=Global variables declaration / definition. */
/* ============================================================================================================================================================= *\
                                                              Global variables declaration / definition.
\* ============================================================================================================================================================= */
UCHAR PicoIdentifier[40];
UCHAR PicoUniqueId[25];
UCHAR Separator[] = "========================================================================================================================\n";

/* Complete day names. */
UCHAR DayName[7][13] =
{
  {"Sunday"}, {"Monday"}, {"Tuesday"}, {"Wednesday"}, {"Thursday"}, {"Friday"}, {"Saturday"}
};

/* Short month names (3 letters). */
UCHAR ShortMonth[13][4] =
{
  {" "}, {"JAN"}, {"FEB"}, {"MAR"}, {"APR"}, {"MAY"}, {"JUN"}, {"JUL"}, {"AUG"}, {"SEP"}, {"OCT"}, {"NOV"}, {"DEC"}
};

struct struct_ltr559 StructLTR559;

datetime_t DateTime;  // for Pico's real-time clock.



/* $PAGE */
/* $TITLE=Function prototypes. */
/* ============================================================================================================================================================= *\
                                                                        Function prototypes.
\* ============================================================================================================================================================= */
/* Blink Pico's LED the specified number of times. NOTE: Will NOT work on a PicoW. */
void blink_pico_led(UINT8 NumberOfTimes);

/* Retrieve Pico's Unique ID from the flash IC and its corresponding Identifier. */
void get_pico_identifier(UCHAR *PicoUniqueId, UCHAR *PicoIdentifier, UINT8 *PicoType);

/* Read a string from stdin. */
void input_string(UCHAR *String, UINT16 StringSize, UINT32 TimeOutMSec);

/* Print log header. */
void log_header(void);

/* Send a string to log file. */
void log_printf(UINT LineNumber, const UCHAR *FunctionName, UCHAR *Format, ...);





/* $PAGE */
/* $TITLE=Main program entry point. */
/* ============================================================================================================================================================= *\
                                                                      Main program entry point.
\* ============================================================================================================================================================= */
INT main()
{
  UCHAR String[32];

  UINT8 Delay;
  UINT8 PicoType;

  INT16 ReturnCode;

  UINT16 Loop1UInt16;


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                           Initializations.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* Note: Parameters below have not been defined in Pico-LTR559-Module.h since they may change from project to project. */
  /* GPIO's used for LTR559. */
  StructLTR559.I2CPort        = I2C_PORT;
  StructLTR559.GpioSDA        = I2C_DATA;
  StructLTR559.GpioSCL        = I2C_CLOCK;  
  StructLTR559.ReadAddress    = LTR559_READ_ADDRESS;
  StructLTR559.WriteAddress   = LTR559_WRITE_ADDRESS;
  StructLTR559.LightLevelGain = LIGHT_LEVEL_GAIN;

  stdio_init_all();

  /* Get Pico's Unique Id and Device Identifier (based on Pico's Unique Id). */
  get_pico_identifier(PicoUniqueId, PicoIdentifier, &PicoType);

  /* Assign default valid values to date and time on entry to prevent garbage. */
  DateTime.dotw  = 0;
  DateTime.day   = 1;
  DateTime.month = 1;
  DateTime.year  = 2027;
  DateTime.hour  = 12;
  DateTime.min   = 00;
  DateTime.sec   = 00;


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                    Wait for USB CDC connection.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* Give a chance for user to start terminal emulator. */
  Delay = 0;
  log_printf(__LINE__, __func__, "Waiting for a USB CDC connection.\n");  // message will not show up on terminal if connection is not established.
  while (!stdio_usb_connected())
  {
    /* Wait until a USB CDC connection has been established. */
    ++Delay;  // one more cycle waiting for USB CDC connection.
    sleep_ms(500);
  }

  sleep_ms(300);  // give some time for USB CDC to be responsive.
  log_printf(__LINE__, __func__, "cls");   // clear terminal emulator screen on entry.
  log_printf(__LINE__, __func__, "home");  // "home" terminal emulator cursor on entry.
  log_printf(__LINE__, __func__, "LOG MASK %X", LOG_LINE + LOG_CORE + LOG_FUNCTION);  // turn On line number, core number and function name only in log file on entry.
  log_header();
  log_printf(__LINE__, __func__, "Main program entry point (Delay: %u msec waiting for USB Communications Device Class (CDC) connection).\n", (Delay * 500));
  if (stdio_usb_connected()) log_printf(__LINE__, __func__, "USB Communications Device Class (CDC) connection has been detected.\n", __LINE__);



  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                               Initialize and set Pico's real-time clock.
     Since the goal of this Firmware is to show how to integrate an LTR559 in an existing project / program, program has been left very simple / basic and we do
                       not connect to Wi-Fi and MQTT to initialize Pico's rtc. User can check other Firmwares in my repository to see examples.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* Ask user to enter current date and time. */
  log_printf(__LINE__, __func__, "Setting Pico's real-time clock:\n");
  log_printf(__LINE__, __func__, "(Sunday = 0   Monday = 1   Tuesday = 2   Wednesday = 3   Thursday = 4   Friday = 5   Saturday = 6)\n");
  log_printf(__LINE__, __func__, "Enter Day-of-week: ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.dotw = atol(String);

  log_printf(__LINE__, __func__, "Enter Day-of-month (1 to 31): ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.day = atol(String);
  if (DateTime.day > 31) DateTime.day = 1;

  log_printf(__LINE__, __func__, "Enter Month (1 to 12)       : ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.month = atol(String);
  if (DateTime.month > 12) DateTime.month = 1;

  log_printf(__LINE__, __func__, "Enter Year (ex: 2025)       : ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.year = atol(String);
  if ((DateTime.year < 2000) || (DateTime.year > 2999)) DateTime.year = 2025;

  log_printf(__LINE__, __func__, "Enter Hour (0 to 23)        : ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.hour = atol(String);
  if (DateTime.hour > 23) DateTime.hour = 12;

  log_printf(__LINE__, __func__, "Enter Minutes (0 to 59)     : ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.min = atol(String);
  if (DateTime.min > 59) DateTime.min = 0;

  log_printf(__LINE__, __func__, "Enter Seconds (0 to 59)     : ");
  input_string(String, sizeof(String), 0);
  if (String[0] != 0x0D) DateTime.sec = atol(String);
  if (DateTime.sec > 59) DateTime.day = 0;

  log_printf(__LINE__, __func__, "Current date and time:   %s   %u-%s-%4.4u   %2.2u:%2.2u:%2.2u\n", 
             DayName[DateTime.dotw],
             DateTime.day,  ShortMonth[DateTime.month], DateTime.year,
             DateTime.hour, DateTime.min,               DateTime.sec);

  rtc_init();                   // initialize Pico's real-time clock hardware.
  rtc_set_datetime(&DateTime);  // set Pico's real-time clock with current date and time entered by user.
  log_printf(__LINE__, __func__, "LOG MASK %X", LOG_ALL);  // enable timestamp in log file.
  printf("\n");
  log_printf(__LINE__, __func__, "Now that Pico's real-time clock has been initialized, logged data will be time stamped.\n");



  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                       Initialize LTR559 sensor.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if ((ReturnCode = ltr559_init()) != 0)
  {
    sleep_ms(300);  ///
    log_printf(__LINE__, __func__, "LTR559 initialization error (return code: %d). Aborting Firmware...\n\n\n", ReturnCode);
    sleep_ms(200);
    exit(1);
  }


  log_printf(__LINE__, __func__, "Press <Enter> to stop LTR559 read cycles...\n");
  while (1)
  {
    /* Trigger another LTR559 read cycle. */
    if (ltr559_read_parameters())
    {
      log_printf(__LINE__, __func__, "Error while trying to read ambient light level and proximity detector...\n");
    }
    printf("\n\n");
    log_printf(__LINE__, __func__, "Ambient light value:      %5.2f\n", StructLTR559.Lux);
    log_printf(__LINE__, __func__, "Proximity relative value: %u\n", StructLTR559.Proximity);

    
    /* Wait 15 seconds before triggering next read cycle. */
    for (Loop1UInt16 = 0; Loop1UInt16 < 30; ++Loop1UInt16)
    {
      input_string(String, 1, 500l);
      if (String[0] != 0x1B)
      {
        printf("\n\n");
        log_printf(__LINE__, __func__, "Press <Enter> to trigger another sensor read cycle or...\n");
        log_printf(__LINE__, __func__, "press <G> to toggle Pico in upload mode: ");
        input_string(String, 1, 0l);
        if ((String[0] == 'G') || (String[0] == 'g'))
        {
          log_printf(__LINE__, __func__, "Toggling Pico in upload mode...\n");
          reset_usb_boot(0l, 0l);
        }
        if (String[0] != 0x1B) break;  // trigger another read cycle.
      }
    }
  }

  return 0;
}





/* $PAGE */
/* $TITLE=blink_pico_led() */
/* ============================================================================================================================================================= *\
                                                          Blink Pico's LED the specified number of times.
                           NOTE: This function WILL NOT WORK with a PicoW since we must go through the CYW43 module to address PicoW's LED
                                                 you may check my repository Pico-WiFi-Module to see how to do it.
\* ============================================================================================================================================================= */
void blink_pico_led(UINT8 NumberOfTimes)
{
  UINT8 Loop1UInt16;

  for (Loop1UInt16 = 0; Loop1UInt16 < NumberOfTimes; ++Loop1UInt16)
  {
    gpio_put(GPIO_PICO_LED, true);
    sleep_ms(120);
    gpio_put(GPIO_PICO_LED, false);
    sleep_ms(300);
  }

  return;
}


#include "get_pico_identifier.c"

#include "input_string.c"


/* $PAGE */
/* $TITLE=log_header() */
/* ============================================================================================================================================================= *\
                                                                         Print log header.
\* ============================================================================================================================================================= */
void log_header(void)
{
  UCHAR DeviceId[72];

  UINT16 Loop1UInt16;


  log_printf(__LINE__, __func__, "\n\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Pico-LTR559-Example\n");
  log_printf(__LINE__, __func__, "<120>Compatible with the SLASH Smart Home ecosystem family.\n");
  log_printf(__LINE__, __func__, "<120>Pico unique ID: <%s>.\n", PicoUniqueId);

  /* Display device identifier. */
  sprintf(DeviceId, "Device identifier: <%s>.", PicoIdentifier);
  log_printf(__LINE__, __func__, "");
  for (Loop1UInt16 = 0; Loop1UInt16 < ((120 - (strlen(DeviceId))) / 2); ++Loop1UInt16)
    printf(" ");
  printf("%s\n", DeviceId);

  /* Display type of sensor used in the project. */
  log_printf(__LINE__, __func__, "                                        Built for Pimoroni's Multi-Sensor stick\n");

  log_printf(__LINE__, __func__, Separator);

  return;
}


#include "log_printf.c"
