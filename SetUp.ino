void setup() {
  Serial.begin(9600);

  // User interface switches (used for sprite and font)
  uiSwitches.begin();
  
  BluetoothSerial.begin(9600);  // HC-05 9600 baud
  
  P.begin();
  // Use ASCII, comment line below if you don't want to use it
  P.setFont(ExtASCII);
  /* 
   * Replace character by the one created  
   * to add more or less character, please see LeDisplay file
   */
  P.addChar('~', bike);

  /*
   * First initialisation of message displayed
   */
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, PA_SPRITE, PA_SPRITE);

  // Initialize sprites
  P.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman2,  W_PMAN2, F_PMAN2);
}
