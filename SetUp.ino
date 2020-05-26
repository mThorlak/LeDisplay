void setup() {
  Serial.begin(9600);

  // user interface switches
  uiSwitches.begin();
  
  BluetoothSerial.begin(9600);  // HC-05 9600 baud
  
  P.begin();
  // Use ASCII
  P.setFont(ExtASCII);
  // Replace character by the one created
  P.addChar('~', bike);
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, PA_SPRITE, PA_SPRITE);
  P.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman2,  W_PMAN2, F_PMAN2);
}
