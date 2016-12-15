/* USBSerial */

// Globals - these need to be defined at the top of your code
// and used in your Line Following procedure:

//float g_MaxFollowSpeed;
//float kp;
//float ki;
//float kd;

// Example of outputing 1 stream of data to be graphed:

//  static float xtempx = .01;
//  xtempx++;
//  terminal.print(xtempx);
//  terminal.println(",");
//  delay(50);

// Parse and put data from USB Serial 
void USBRead(String p_USBDataString) {
  Melody(ledPin, 1000, 10);
  String stringParsedData = p_USBDataString;
  
  stringParsedData = stringParsedData.substring(2, stringParsedData.indexOf("#"));
  char charParsedData[stringParsedData.length()+1];
  stringParsedData.toCharArray(charParsedData, stringParsedData.length()+1);
  float floatParsedData = atof(charParsedData);
  
  if (p_USBDataString.startsWith("kp")) { // change robot's linefollow speed
    kp = floatParsedData; 
    
    // send back to Graph program that we got the data
    terminal.print("kp");   
    terminal.print(kp);
    terminal.println("#"); 
  }
  
  if (p_USBDataString.startsWith("ki")) { // change robot's linefollow speed
    ki = floatParsedData; 
    
    // send back to Graph program that we got the data
    terminal.print("ki");   
    terminal.print(ki);
    terminal.println("#"); 
  }
  
  if (p_USBDataString.startsWith("kd")) { // change robot's linefollow speed
    kd = floatParsedData;   
    
    // send back to Graph program that we got the data
    terminal.print("kd"); 
    terminal.print(kd);
    terminal.println("#"); 
  }
  
  if (p_USBDataString.startsWith("sp")) { // change robot's linefollow speed
    g_MaxFollowSpeed = floatParsedData;  

    // send back to Graph program that we got the data
    terminal.print("sp");
    terminal.print(g_MaxFollowSpeed);
    terminal.println("#");     
  }
    
//  terminal.print(g_USBDataString);
//  terminal.println(","); 
}

// Get incoming data from USB
void serialEvent() {
  delay(1);
  static String  s_USBDataString = "";
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    s_USBDataString += inChar;
    s_USBDataString.trim();
    if(inChar=='#'){ // we got to the end of the string
      USBRead(s_USBDataString);
      s_USBDataString = "";
    }
  }
}
