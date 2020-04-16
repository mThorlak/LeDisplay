void setup() {
  Serial.begin(9600);
  BluetoothSerial.begin(9600);  // HC-05 9600 baud 
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}
