/* Telemetry V0.1 */

// Telemetry Globals
String  PcDataString = "";         // a string to hold incoming data

float   bv = 0; //battery volts
//int neededTurn=0;

int sentTime2;
long ack;
long sendError;
long lastHeartBeat=0;
long missedHeartBeats=0;
String heartbeat;

void XBeeSend(String _tag, float _data) {  
  XBee.print(_tag);
  XBee.print(_data);
  XBee.print("#");
  delay(12); // delay is better then timer here!
  
//  Serial.print(_tag);
//  Serial.print(_data);
//  Serial.print("# ");
}

// Parse Incoming XBee Serial Data
void ParseSerialData() {  
  //writeLCDLine(PcDataString,2,3);
  // PcDataComplete - flag for saying serial read from pc is complete
  // First 2 letters tell us what the data is for
  // Format:  kp1050# or kp.05#
    if (PcDataString.startsWith("hb")) { // Heartbeat
//    Serial.println("Hit hb");
      // Get number passed, string must end with #
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      // create array with correct size
      char PcDataChar[PcDataString.length()+1];
      // put String into array
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      // Change to float type
      float PcData = atof(PcDataChar); 
      // one's place digit to show as heartbeat
      String heartBeat =  PcDataString.substring(PcDataString.length()-1);                 
          
      if (millis() < 1000) {
        // startup
        lastHeartBeat = PcData;
          
      } else {
        if (lastHeartBeat+1 != PcData) {
          // missed a beat
          missedHeartBeats += 1;
          heartBeat = String(missedHeartBeats);
          heartBeat =  heartBeat.substring(heartBeat.length()-1); 
          Serial.write(ESC); 
          Serial.write((byte)0x45);
          Serial.write((byte)0x4F);
          Serial.print(heartBeat);
        } else {
          // Show beats
          Serial.write(ESC); 
          Serial.write((byte)0x45);
          Serial.write((byte)0x0F);
          Serial.print(heartBeat);
        }
      lastHeartBeat = PcData;
      } 
    }
    
    if (PcDataString.startsWith("va")) { // Variable adjustment
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      writeLCDLine("                ",2,0);
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("bv")) { // battery voltage
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      //bv = PcData;
      writeLCDLine("Robot Battery:  ",1,0);
      writeLCDLine("                ",2,0);
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("CL")) { // IMU calibration state
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
//      float PcData = atof(PcDataChar);
      //bv = PcData;
      //writeLCDLine("IMU Cal State:  ",1,0);
      writeLCDLine("                ",2,0);
      writeLCDLine(PcDataString,2,0); 
    }
    
    if (PcDataString.startsWith("yw")) { // yaw from IMU
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      g_yaw = PcData;
      writeLCDLine("      ",2,0);
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("ya")) { // yaw from encoders
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
//      yaw = PcData;
      writeLCDLine("     ",1,0);
      writeLCDLine(PcDataChar,1,0); 
    }
    
    if (PcDataString.startsWith("xp")) { // x_pos - x for cartesian coordinates
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
//      yaw = PcData;
      writeLCDLine("          ",1,5);
      writeLCDLine(PcDataChar,1,7); 
    }
    
    if (PcDataString.startsWith("yp")) { // y_pos - y for cartesian coordinates
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
//      yaw = PcData;
      writeLCDLine("          ",2,5);
      writeLCDLine(PcDataChar,2,7); 
    }
    
    if (PcDataString.startsWith("rb")) { // remote control's battery voltage
//      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
//      char PcDataChar[PcDataString.length()+1];
//      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
//      float PcData = atof(PcDataChar);
//      bv = PcData;
    }
    
    if (PcDataString.startsWith("pd")) { // pwr diff for Linefollow PID
      // Get number passed, string must end with #
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      bv = PcData;
      writeLCDLine("pwr diff:             ",1,0);
      writeLCDLine("                ",2,0);   
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("bs")) { // show bot speed
      // Get number passed, string must end with #
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      bv = PcData;
      
      writeLCDLine("                ",2,0);   
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("wi")) { // wii angle to display
      // Get number passed, string must end with #
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      bv = PcData;
      
      writeLCDLine("                ",2,0);   
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("pi")) { // pipe seen!
      // Get number passed, string must end with #
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
//      bv = PcData;
      writeLCDLine("Pipe:             ",1,0);
      writeLCDLine("                ",2,0);   
      writeLCDLine(PcDataChar,2,0); 
    }
    
    if (PcDataString.startsWith("rm")) { // robot asked for mode which means it just reset or turned on
      Reset();
    }
    
    if (PcDataString.startsWith("ss")) { // start/stop state
//    Serial.println("Hit ss");
      // Get number passed, string must end with #
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      g_robotStarted = boolean(PcData);
//      Serial.println(g_robotStarted);
      if (g_robotStarted == true){
        writeLCDLine("Started         ",2,0);
//        Serial.println("Hit ss true");
      }
      if (g_robotStarted == false){
        writeLCDLine("Stopped         ",2,0);
//        Serial.println("Hit ss false");
      }
//    Serial.println("Hit ss end");
    }
    
    if (PcDataString.startsWith("mo")) { // robot passed mode back after setting it
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      g_mode = int(PcData);
      
      // show mode
      if (g_mode == BULLDOZER) {
        writeLCDLine("Bulldozer        ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == LINEFOLLOW) {
        writeLCDLine("Line Follower   ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == BEACONKILLER) {
        writeLCDLine("Beacon Killer   ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == BEACONKILLERWO) {
        writeLCDLine("Beacon Killer WO",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == PLAYLINEBACK) {
        writeLCDLine("Play Line Back  ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == CUBEQUEST) {
        writeLCDLine("Cube Quest      ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == DEADREC) {
        writeLCDLine("Dead Reckoning  ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == IMUCAL) {
        writeLCDLine("IMU Cal Status  ",1,0);
        writeLCDLine("                ",2,0);
      }
      if (g_mode == BATTREM) {
        //batt:10.55V /A2:4.21V
        //max:11.2V  /min:8 /range:3.2V
        writeLCDLine("Remote Battery  ",1,0);
        writeLCDLine("                ",2,0);
        float Vout = analogRead(A2);//read Vout from votage divider (voltage out of the voltage divider)
        Vout = 5*(Vout/1024);//convert 0-1024 reading to volts
        float Vin = Vout*(9.8+14.75)/9.8; //find Vin given Vout (batt pack voltage level)
        float Vpercent = 100*(Vin-9.4)/0.92; //percent batt life left 100*(Vin-BottomLevel)/(TopLevel-BottomLevel)
        char VinChar[10];
        dtostrf(Vin,2,1,VinChar); //convert float to string
        char VpercentChar[10];
        dtostrf(Vpercent,2,0,VpercentChar); //convert float to string
        String Vdisplay = VinChar;// concatonate strings for display
        Vdisplay += "V  ";
        Vdisplay += VpercentChar;
        Vdisplay += "%";
        writeLCDLine(Vdisplay,2,0);  
      }      
      if (g_mode == BATTBOT) {
        XBeeSend("bb",1);// ask robot for battery voltage
      }
    }
    
    if (PcDataString.startsWith("ls")) { // robot passed linefollow speed back after setting it
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      writeLCDLine(PcDataChar,2,0);
    }
    
    if (PcDataString.startsWith("ml")) { // robot passed linefollow mode back after setting it
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      modeLineFollow = int(PcData);
      
      // show mode
      switch(modeLineFollow) {
        case 1: {
          writeLCDLine("                ",2,0);
          writeLCDLine("1) 110:7/0/20   ",2,0);
          break;
        }
        case 2:{
          writeLCDLine("                ",2,0);
          writeLCDLine("2) 120:10/0/15  ",2,0);
          break;
        }
        case 3:{
          writeLCDLine("                ",2,0);
          writeLCDLine("3) 127:10/0/15  ",2,0);
          break;
        }
        case 4:{
          writeLCDLine("                ",2,0);
          writeLCDLine("4) 0:10/0/0     ",2,0);
          break;
        }
        case 5:{
          writeLCDLine("                ",2,0);
          writeLCDLine("5) 0:0/.05/0    ",2,0);
          break;
        }
        case 6:{
          writeLCDLine("                ",2,0);
          writeLCDLine("6) 0:4/0/2    ",2,0);
          break;
        }
//        default: 
          // if nothing else matches, do the default
          // default is optional
      }
    }
    
    if (PcDataString.startsWith("mb")) { // robot passed beaconkiller mode back after setting it
      PcDataString = PcDataString.substring(2, PcDataString.indexOf("#"));
      char PcDataChar[PcDataString.length()+1];
      PcDataString.toCharArray(PcDataChar, PcDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      modeBEACONKILLER = int(PcData);
      
      // show mode
      switch(modeBEACONKILLER) {
        case 1: {
          writeLCDLine("                ",2,0);
          writeLCDLine("1) TR           ",2,0);
          break;
        }
        case 2:{
          writeLCDLine("                ",2,0);
          writeLCDLine("2) TL           ",2,0);
          break;
        }
        case 3:{
          writeLCDLine("                ",2,0);
          writeLCDLine("3) TR HR        ",2,0);
          break;
        }
        case 4:{
          writeLCDLine("                ",2,0);
          writeLCDLine("4) TL HR        ",2,0);
          break;
        }
        case 5:{
          writeLCDLine("                ",2,0);
          writeLCDLine("5) TR HL        ",2,0);
          break;
        }
        case 6:{
          writeLCDLine("                ",2,0);
          writeLCDLine("6) TL HL        ",2,0);
          break;
        }
        case 7:{
          writeLCDLine("                ",2,0);
          writeLCDLine("7) TR ST        ",2,0);
          break;
        }
        case 8:{
          writeLCDLine("                ",2,0);
          writeLCDLine("8) TL ST        ",2,0);
          break;
        }
//        default: 
          // if nothing else matches, do the default
          // default is optional
      }
    }
        
    // clear the string and flag:
    PcDataString = "";
    g_PcDataComplete = false;
}

// Get data from xBee, called from loop()
void XBeeEvent() {
  while (XBee.available()) {
    // get the new byte:
    char inChar = (char)XBee.read();
    // add it to the inputString:
    PcDataString += inChar;
    PcDataString.trim();
    if(inChar=='#'){
      ParseSerialData();
    }
  }
}

//// Get data from USB Port
//void serialEvent() {
//  //Serial.println("Hit serialEvent()");
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    PcDataString += inChar;
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    //Serial.println(inChar);
//    if (inChar == '#') {
//      PcDataComplete = true;
//    }
//  }
//}
