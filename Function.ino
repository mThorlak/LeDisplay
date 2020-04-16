/*
 * ----------------------------------------------------------------------------------
 * Bluetooth
 * ----------------------------------------------------------------------------------
 */
 
/*
 * Read data send in bluetooth device
 * Send char[] MessageReceived into newMessage data
 * To change bluetooth configuration, checkout LeDisplay file
 */
void readBluetooth(void)
{
  while (BluetoothSerial.available()){
    // Read message send by BluetoothSerial
    messageReceived = BluetoothSerial.readString();
    if (messageReceived != "") {
      newMessageAvailable = true;
      messageDispatcher(messageReceived);
      // messageReceived.toCharArray(newMessage,BUF_SIZE);
    }
  }
}

/*
 * ----------------------------------------------------------------------------------
 * Display and scroll message
 * ----------------------------------------------------------------------------------
 */

/*
 * Display messageReceived
 * To change scroll configuration, checkout LeDisplay file
 */
void display_scroll_message() {
  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      P.displayClear();
      P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
  readBluetooth();
}

/*
 * |SS25|SELeft|SALeft|SP2000|
 */
void messageDispatcher(String message) {

  int i = 0;
  int cpt = 0;
  int cptOptionConcerned = 0;
  bool lastOptionIsDispatched = false;
  String optionConcerned = "";
  String optionDirective = "";
  Serial.println(message);

  while (lastOptionIsDispatched == false) {

    while (cpt != 2) {
      if (message[i] == '|') {
        cpt ++;
      }
      else if (cpt == 1 && cptOptionConcerned != 2) {
        optionConcerned.concat(message[i]);
        cptOptionConcerned ++;
      }
      else {
        optionDirective.concat(message[i]);
      }
      i++;
    }
  
    if (optionConcerned == "SS") {
      scrollSpeed = optionDirective.toInt();
      Serial.print("ScrollSpeed : ");
      Serial.println(scrollSpeed);
    }
    else if (optionConcerned == "SE") {
      manageTextEffect(optionDirective);
    }
    else if (optionConcerned == "SA") {
      manageTextPosition(optionDirective);
    }
    else if (optionConcerned == "SP") {
      scrollPause = optionDirective.toInt();
      Serial.print("ScrollPause : ");
      Serial.println(scrollPause);
      lastOptionIsDispatched = true;
      i++;
    }
    i--;
    cpt = 0;
    cptOptionConcerned = 0;
    optionConcerned = "";
    optionDirective = "";
 }
 String messageToDisplay = message.substring(i,message.length()-1);
 messageToDisplay.toCharArray(newMessage,BUF_SIZE);
  
}
 
/*
 * Case text effect
 */
void manageTextEffect(String message) {
  if(message == "No effect") {
    scrollEffect = PA_NO_EFFECT;
  }
  if(message == "Print") {
    scrollEffect = PA_PRINT;
  }
  if(message == "Scroll Up") {
    scrollEffect = PA_SCROLL_UP;
  }
  if(message == "Scroll Down") {
    scrollEffect = PA_SCROLL_DOWN;
  }
  if(message == "Scroll Left") {
    scrollEffect = PA_SCROLL_LEFT;
  }
  if(message == "Scroll Right") {
    scrollEffect = PA_SCROLL_RIGHT;
  }
  Serial.print("Message text effect : ");
  Serial.println(message);
}

/*
 * Case text position
 */
void manageTextPosition(String message) {
  if (message == "Center") {
    scrollAlign = PA_CENTER;
  }
  if (message == "Left") {
    scrollAlign = PA_LEFT;
  }
  if (message == "Right") {
    scrollAlign = PA_RIGHT;
  }
  Serial.print("Manage text position : ");
  Serial.println(message);
}
