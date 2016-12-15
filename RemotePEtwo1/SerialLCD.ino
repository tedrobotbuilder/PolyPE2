// SerialLCD

/* NKC Electronics 16x2 Serial LCD test 16x2 
Serial LCD pinout for UART serial communication 
pin 1: RX pin 2: GND pin 3: +5V 
connect pin1 to Arduino digital pin 1 (TX)
connect pin2 to Arduino GND 
connect pin3 to Arduino +5V 
Default communication speed is 9600 */ 

void clearLCD(){  
  Serial.write(ESC); 
  Serial.write(0x51); //Clear Screen
}

void writeLCDLine(String _text, int _line, int _position) {
    Serial.write(ESC); 
    Serial.write((byte)0x45); //Set Cursor Position
    if(_line==1)
      Serial.write((byte)0x0+_position); //1 byte Put cursor at location, 0x00 to 0x67
    else
      Serial.write((byte)0x40+_position);
    Serial.print(_text);
}

//void writeLCDLine2(String _text, int _line, int _position) {
//    Serial.write(ESC); 
//    Serial.write((byte)0x45); //Set Cursor Position
//    Serial.write((byte)0x40+_position); //1 byte Put cursor at location, 0x00 to 0x67
//    Serial.print(_text);
//}

//void writeLCDLine2x(String _text, int _position) {
//    Serial.write(ESC); 
//    Serial.write((byte)0x45); //Set Cursor Position
//    Serial.write((byte)0x40+_position); //1 byte Put cursor at location, 0x00 to 0x67
//    Serial.print(_text);
//}

//void writeLCDPosition(String _text, String _position) {
//    Serial.write(ESC); 
//    Serial.write((byte)0x45); //Set Cursor Position
//    Serial.write((byte)_position); //1 byte Put cursor at location, 0x00 to 0x67
//    Serial.print(_text);
//}

// Initialize LCD module
void initLCD() {   
  Serial.write(ESC); 
  Serial.write(0x41); //Turn On Display
  
  clearLCD();
  
  // Set Contrast 
  Serial.write(ESC); 
  Serial.write(0x52); 
  Serial.write(40); //between 1 to 50
  
  // Set Backlight 
  Serial.write(ESC); 
  Serial.write(0x53); 
  Serial.write(3); //between 1 to 8
}
