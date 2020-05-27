void loop() {
  /*
   * Wait new meesage given by bluetooth
   * And display it
   * If no new message
   * Then display the laste sent
   */
  readBluetooth();
  display_scroll_message();
}
