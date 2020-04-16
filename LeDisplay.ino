/*
 * Works only with led matrix MAX72XX
 * You can see all possible feature in MD_Parola.h file
 */

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <SoftwareSerial.h>

const uint16_t WAIT_TIME = 1000;

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

/*
 * TODO : change later for matric 8x64
 */
#define MAX_DEVICES 4
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
char curMessage[BUF_SIZE] = { "LeDisplay" };
// Message show in loop
char newMessage[BUF_SIZE] = { "Hello! Enter new message?" };
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
