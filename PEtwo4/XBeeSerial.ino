/* Telemetry V0.1 */

// Telemetry Globals
String  XBeeDataString = "";         // a string to hold incoming data

// continuous test of XBee serial communication
void HeartBeat() {
  static int HeartBeatTimer_old;
  static int HeartBeatCounter;
  
  if((millis()-HeartBeatTimer_old) < 1000) {
    return;
  }
  HeartBeatTimer_old = millis();
  
//  Serial.println("Hit hb");
  HeartBeatCounter += 1;
  
  // send to remote with XBee
  Serial2.print("hb");
  Serial2.print( HeartBeatCounter );
  Serial2.println("#");
  delay(12); //flush buffer
}

// send a command to the remote over XBee
void XBeeSend(String _tag, float _data) {   
  Serial2.print(_tag);
  Serial2.print(_data);
  Serial2.print("#");
  delay(12); // delay is better then timer here! Flush buffer.
}

void RequestMode() {
  XBeeSend("rm", 0);// mode unknown, ask remote for mode
}

float parseSerialString() {
  XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
  char PcDataChar[XBeeDataString.length()+1];
  XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
  return atof(PcDataChar);
}

void ParseSerialData() {
  Serial.println("hit");

    if (XBeeDataString.indexOf("#") < 0){
      // if index is < 0 then # isn't in the string and this shouldn't happen, we use # to note end of string
//      Serial.print("Error in ParseSerialData(), no # in passed string, coindexOf #:");
//      Serial.println(XBeeDataString.indexOf("#"));
//      Serial.print("XBeeDataString=");
//      Serial.println(XBeeDataString);
      Melody(ledPin, 500, 250);
    }
    
    // First 2 letters tell us what the data is for
    // Format:  kp1050# or ki.05#    
    if (XBeeDataString.startsWith("vl")) { // desired velocity from remote      
      XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
      char PcDataChar[XBeeDataString.length()+1];
      XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      g_speed =int(PcData);
      RoboDuty(g_speed,g_speed);
    }
  
    if (XBeeDataString.startsWith("va")) { // adjust variable     
      XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
      char PcDataChar[XBeeDataString.length()+1];
      XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      g_adjustVar = float(PcData);
      XBeeSend("va", g_adjustVar);
    }    
    
    if (XBeeDataString.startsWith("tr")) { // desired turn from remote
      XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
      char PcDataChar[XBeeDataString.length()+1];
      XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      Drive_Motors_Joystick(g_speed, int(PcData));
      
//      Serial.print("x2:");
//      Serial.print(g_speed);
//      Serial.print(" tr:");
//      Serial.println(int(PcData));
    }    
    
    if (XBeeDataString.startsWith("bb")) { // request for robot battery voltage      
      XBeeSend("bv", ReadBattVolts());
    }      
    
    if (XBeeDataString.startsWith("xy")) { // request for CalcXY
      CalcXY();
    }     
    
    if (XBeeDataString.startsWith("mo")) { // change robot's mode
      XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
      char PcDataChar[XBeeDataString.length()+1];
      XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      g_mode = PcData;// set mode
      g_motorModeSet = false;// clear motor mode
      
      // repeat back to remote that we set the mode
      XBeeSend("mo",g_mode);      
      
//      Serial.print("mo:");
//      Serial.println(g_mode);
    }      
    
    if (XBeeDataString.startsWith("ls")) { // change robot's linefollow speed
      g_MaxFollowSpeed = parseSerialString();     
    }
    
    
    if (XBeeDataString.startsWith("ml")) { // change robot's linefollow mode
      g_modeLineFollow = parseSerialString();
      
      switch(g_modeLineFollow) {
        case 1: {        
          g_MaxFollowSpeed = 12500;
//          g_lineFollowSpeed = 60;
//          kp=7;//1) 110:7/0/20
//          ki=0;
//          kd=4;          
          break;
        }
        case 2: {         
          g_MaxFollowSpeed = 13500;
//          g_lineFollowSpeed = 90;
//          kp=10;//2) 120:10/0/15
//          ki=0;
//          kd=15;          
          break;
        }
        case 3: {         
          g_MaxFollowSpeed = 14000;
//          kp=10;//3) 127:10/0/15
//          ki=0;
//          kd=15;          
          break;
        }
        case 4: {         
          g_MaxFollowSpeed = 14500;
//          kp=10;//4) 
//          ki=0;
//          kd=0;          
          break;
        }
        case 5: {         
          g_MaxFollowSpeed = 15000;
//          kp=0;//5) 
//          ki=.05;
//          kd=0;          
          break;
        }
        case 6: {         
          g_MaxFollowSpeed = 15500;
//          kp=10;//6) 
//          ki=0;
//          kd=2;          
          break;
        }
        //cycle back
      }
      
//      Serial3.print(g_modeLineFollow);   
//      Serial3.print("\t");
//      Serial3.print(g_lineFollowSpeed);   
//      Serial3.print("\t");
//      Serial3.print(kp);   
//      Serial3.print("\t");
//      Serial3.print(ki);   
//      Serial3.print("\t");
//      Serial3.println(kd);  
      
      // repeat back to remote that we set the mode
      XBeeSend("ls",g_MaxFollowSpeed);      
      
//      Serial.print("ml:");
//      Serial.println(g_modeLineFollow);
    }      
    
    
    if (XBeeDataString.startsWith("mb")) { // change robot's beaconkiller mode
      XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
      char PcDataChar[XBeeDataString.length()+1];
      XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
      float PcData = atof(PcDataChar);
      
      g_modeBeaconKiller = PcData;
      
      switch(g_modeBeaconKiller) {
        case 1: {         
          g_BeaconTurnRight = 1; // turn right
          g_BeaconHook=0; //no obstacles, no hook
          break;
        }
        case 2: {        
          g_BeaconTurnRight = 0; // turn left
          g_BeaconHook=0; //no obstacles, no hook
          break;
        }
        case 3: {     
          g_BeaconTurnRight = 1;// turn right
          g_BeaconHook=1;  //go right   
          break;
        }
        case 4: {         
          g_BeaconTurnRight = 0; // turn left
          g_BeaconHook=1;  //go right 
          break;
        }
        case 5: {         
          g_BeaconTurnRight = 1;// turn right
          g_BeaconHook=2;  //go left 
          break;
        }
        case 6: {         
          g_BeaconTurnRight = 0;// turn left
          g_BeaconHook=2;  //go left 
          break;
        }
        case 7: {         
          g_BeaconTurnRight = 1;// turn right
          g_BeaconHook=3;  //go stright 
          break;
        }
        case 8: {         
          g_BeaconTurnRight = 0;// turn left
          g_BeaconHook=3;  //go stright 
          break;
        }
//        default: 
          // if nothing else matches, do the default
          // default is optional
      }
      
//      Serial3.print(g_modeLineFollow);   
//      Serial3.print("\t");
//      Serial3.print(g_lineFollowSpeed);   
//      Serial3.print("\t");
//      Serial3.print(kp);   
//      Serial3.print("\t");
//      Serial3.print(ki);   
//      Serial3.print("\t");
//      Serial3.println(kd);  
      
      // repeat back to remote that we set the mode
      XBeeSend("mb",g_modeBeaconKiller);      
      
//      Serial.print("ml:");
//      Serial.println(g_modeLineFollow);
    }      
    
    
    if (XBeeDataString.startsWith("ss")) { // start/stop button pressed
      XBeeDataString = XBeeDataString.substring(2, XBeeDataString.indexOf("#"));
      char PcDataChar[XBeeDataString.length()+1];
      XBeeDataString.toCharArray(PcDataChar, XBeeDataString.length()+1);
      float PcData = atof(PcDataChar);
      
//      Serial.println("ss:hit");

      if(PcData == 0)
        HandelStopButton(); // call handler as if button SS button on bot physically pressed
      
      if(PcData == 1)
        HandelStartButton(); // call handler as if button SS button on bot physically pressed
    }    
    
    // clear the string and flag:
    XBeeDataString = "";
    PcDataComplete = false;
}

// Get data from XBee
void serialEvent2() {
  delay(1);
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    XBeeDataString += inChar;
    XBeeDataString.trim();
//    Serial.print("XBeeDataString:");
//    Serial.println(XBeeDataString);
    if(inChar=='#'){// we got to the end of the string
      ParseSerialData();
    }
  }
}
