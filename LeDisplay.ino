/*
 * Works only with led matrix MAX72XX
 * You can see all possible feature in MD_Parola.h file
 */

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <MD_UISwitch.h>
#include "Parola_Fonts_data.h"

const uint16_t WAIT_TIME = 1000;

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 5
#define CLK_PIN   52
#define DATA_PIN  51
#define CS_PIN    53

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

/*
 * Manage message configuration
 */
// Max size of the message to send
#define  BUF_SIZE  100
// Message show once
char curMessage[BUF_SIZE] = { "Welcome" };
// Message show in loop
char newMessage[BUF_SIZE] = { "Insert new message ~"};
// Indicator when new message has been send by bluetooth serial
bool newMessageAvailable = true;

/*
 * Bluetooth configuration
 */
SoftwareSerial BluetoothSerial(10, 11); // RX | TX  = > BT-TX=10 BT-RX=11
String messageReceived;

/*
 * Scroll configuration 
 */
uint8_t scrollSpeed = 100;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2000; // in milliseconds

/*
 * uiSwitches for managing sprites
 */
const uint8_t JUSTIFY_SET = 6;    // change the justification
const uint8_t INVERSE_SET = 9;    // set/reset the display to inverse
uint8_t uiPins[] = { JUSTIFY_SET, INVERSE_SET };
MD_UISwitch_Digital uiSwitches(uiPins, ARRAY_SIZE(uiPins));

/*
 * Sprite creation
 */
const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] =  // gobbling pacman animation
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};

const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] =  // ghost pursued by a pacman
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
};

struct 
{
  const uint8_t *data;
  uint8_t width;
  uint8_t frames;
} 

sprite[] =
{
  { pacman2, W_PMAN2, F_PMAN2 },
  { pacman1, W_PMAN1, F_PMAN1 }
};

/*
 * Define new character
 */
uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C
uint8_t degF[] = { 6, 3, 3, 124, 20, 20, 4 }; // Deg F
uint8_t waveSine[] = { 8, 1, 14, 112, 128, 128, 112, 14, 1 }; // Sine wave
uint8_t waveSqar[] = { 8, 1, 1, 255, 128, 128, 128, 255, 1 }; // Square wave
uint8_t waveTrng[] = { 10, 2, 4, 8, 16, 32, 64, 32, 16, 8, 4 }; // Triangle wave
uint8_t arrowup[] = { 5, 4, 2, 127, 2, 4 }; // arrow up
uint8_t bike[] = { 10, 64, 160, 176, 88, 16, 18, 82, 188, 160, 64 }; // bike

/*
 * ASCII conversion
 */
uint8_t utf8Ascii(uint8_t ascii){
  static uint8_t cPrev;
  uint8_t c = '\0';

  if (ascii < 0x7f)   // Standard ASCII-set 0..0x7F, no conversion
  {
    cPrev = '\0';
    c = ascii;
  }
  else
  {
    switch (cPrev)  // Conversion depending on preceding UTF8-character
    {
    case 0xC2: c = ascii;  break;
    case 0xC3: c = ascii | 0xC0;  break;
    case 0x82: if (ascii==0xAC) c = 0x80; // Euro symbol special case
    }
    cPrev = ascii;   // save last char
  }

  return(c);
}

void utf8Ascii(char* s){
  uint8_t c;
  char *cp = s;

  while (*s != '\0')
  {
    c = utf8Ascii(*s++);
    if (c != '\0')
      *cp++ = c;
  }
  *cp = '\0';   // terminate the new string
}
 
