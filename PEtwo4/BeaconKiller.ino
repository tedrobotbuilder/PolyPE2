// Beacon Killer

int g_modeBeaconKiller=1;//1-8
boolean g_BeaconTurnRight = true;
byte g_BeaconHook=0; // 0-no hook; 1-hook right; 2-hook left; 3-no hook
long g_BeaconKiller_millis;
int g_forwardSpeed = 0;

//class PixyMemory{
// public:
// unsigned int someBlockStuff=1000;
// void beaconLookOnceTest();
//
// PixyMemory(){ //the constructor
//   //i=0;
// }
// 
// private:
// int somePrivateStuff=0;
//};
// 
//PixyMemory PixyMem = PixyMemory();
    
    boolean g_beaconSeen;// did we see beacon this call?
    float g_headingError;// heading error
    int g_beaconWidth;// size of beacon's box
    int g_beaconSizeSizeVar;// variance for Size
    int g_beaconElevation;
    boolean g_beaconSeenRight=false;
    
    boolean g_obSeen;
    int g_obError;
    int g_obSize;
    int g_obElevation;
    int g_obHeight;
    boolean g_obClose;
    int g_obWidth;
    boolean g_obTurnRight;
   
// #####################  BeaconKiller  ############################################
// #####################  BeaconKiller  ############################################
// #####################  BeaconKiller  ############################################

void BeaconKiller(boolean _obstaclesOn) {
  if((millis() - g_BeaconKiller_millis) < 1) return;
  g_BeaconKiller_millis = millis();
  
  int i;
  int j;
  uint16_t blocks;
  char buf[32]; 
//  static int beaconError = 5000;
  int beaconSize;
  int pBeacon = 0;
  int searchSpeed = 2000;
  static float s_iBeacon = 0;
  static float s_last_pBeacon = 0;
  static int s_beaconSpeed = 0;// current bot speed
  int maxBeaconSpeed = 1700;// max speed limit for beacon killer
  int maxBeanonSpeedNoOb = 7000;//
  static boolean s_FoundBeacon = false;
  static float s_iOb=0;
  static float s_last_pOb=0;
  float hookFactor = 140;
  float beaconDistance = 8 * 12 * CLICKS_PER_INCH;
  
  PixyLookOnce(_obstaclesOn);
  
//  if (g_beaconWidth > 240) {// REMOVE FOR RALLY ###################
//    terminal.println("stop at beacon -Remove for Rally#############");
//    RoboDrive(0,0);
//    delay(50);
//    RoboDuty(0,0);
//    delay(50);
//    while(true){
//      if(Serial2.available())return;
//    }
//  }
  
  //###################################### Search #####################################################################
  if (g_beaconSeen == false) {// Search for beacon
    terminal.println("Search ");
    
    if (g_BeaconTurnRight == 1) {
//      terminal.println(" Search Right ");
      RoboDrive(searchSpeed,-searchSpeed);
    } else {
//      terminal.println(" Search Left ");
      RoboDrive(-searchSpeed,searchSpeed);
    }
    
    return;
  }

  //##################################### avoid obstacle ######################################################################
  if (g_obSeen == true && _obstaclesOn) {// avoid obstacle
    terminal.println(" avoid obstacle ");
    
    //avoidOb(int p_speed, boolean p_turnRight, float p_distance, float p_overRun, float p_turn)
    if (g_obClose) {
      avoidOb(s_beaconSpeed, g_obTurnRight, .7*abs(g_obError)/152, .3, 1);
    } else {
      avoidOb(s_beaconSpeed, g_obTurnRight, .7*abs(g_obError)/152, .3, 2);
    }  

    return;
  }

  //##################################### Drive ######################################################################
  if (g_beaconSeen == true && (g_obSeen == false || _obstaclesOn == false) ) {// We are tracking beacon = kill it
    terminal.println("Drive ");
//    terminal.print(" beaconSeen: ");
//    terminal.print(g_beaconSeen);
//    terminal.print(" obSeen: ");
//    terminal.print(g_obSeen);
//    terminal.print(" ObOn: ");
//    terminal.println(_obstaclesOn);
//    terminal.print(" g_BeaconHook: ");
//    terminal.println(g_BeaconHook);
    
//    g_headingError = constrain (g_headingError - (float(g_obError) * (float(g_obHeight)/50) ), -170, 170);

    float holdEncoderValue = ReturnAvgEnc();
    
//    terminal.print(" g_headingError: ");
//    terminal.println(g_headingError);

    if (holdEncoderValue < beaconDistance && g_BeaconHook != 0) {
      if (g_BeaconHook == 1) g_headingError = g_headingError + hookFactor*(beaconDistance - holdEncoderValue )/beaconDistance;// Right
      if (g_BeaconHook == 2) g_headingError = g_headingError - hookFactor*(beaconDistance - holdEncoderValue )/beaconDistance;// Left
    }
    
//    terminal.print(" g_headingError: ");
//    terminal.println(g_headingError);
    
    pBeacon = -g_headingError;// P is our heading error
    float dBeacon = pBeacon - s_last_pBeacon;// Compute the derivative (change) D of the error
    s_iBeacon += pBeacon;// integral (sum) of the error. I
    
    if (pBeacon == 0) s_iBeacon = 0; //unwind I
    
    s_last_pBeacon = pBeacon;// Remember the last heading error.
    
    float power_difference = (pBeacon*6 + s_iBeacon*0 + dBeacon*2);// Multiply by PID constants
    
    if (!_obstaclesOn) maxBeaconSpeed = maxBeanonSpeedNoOb;
  
    if (s_beaconSpeed < maxBeaconSpeed){// ramp up to set speed
      s_beaconSpeed = s_beaconSpeed + 700;
    } else {
      s_beaconSpeed = maxBeaconSpeed;
    }

//    float rightSpeed = ( constrain(s_beaconSpeed + power_difference, -Bot.maxMotorSpeed, Bot.maxMotorSpeed) );
//    float leftSpeed = ( constrain(s_beaconSpeed - power_difference, -Bot.maxMotorSpeed, Bot.maxMotorSpeed) );

    float rightSpeed = s_beaconSpeed + power_difference;
    float leftSpeed = s_beaconSpeed - power_difference;

    RoboDrive(leftSpeed, rightSpeed);

//    terminal.print("s_beaconSpeed: ");
//    terminal.print(s_beaconSpeed);
//    terminal.print(" pBeacon: ");
//    terminal.print(pBeacon);
//    terminal.print(" s_iBeacon:");
//    terminal.print(s_iBeacon);
//    terminal.print(" dBeacon:");
//    terminal.print(dBeacon);
//    terminal.print(" power diff:");
//    terminal.print(power_difference);
//    terminal.println(",");
    
    return;
  }//end if (g_beaconSeen == true && g_obSeen == false)...
  
  terminal.println("FALL THROUGH ################################################");
}
    
    
// #####################  HookTest  ############################################   
void HookTest() {
  terminal.print(" g_modeBeaconKiller: ");
  terminal.print(g_modeBeaconKiller);
  terminal.print(" g_BeaconTurnRight: ");
  terminal.print(g_BeaconTurnRight);
  terminal.print(" g_BeaconHook: ");
  terminal.println(g_BeaconHook);
}
  

// #####################  PixyLookOnceTest  ############################################
// #####################  PixyLookOnceTest  ############################################
// #####################  PixyLookOnceTest  ############################################

void PixyLookOnceTest() {
  PixyLookOnce(false);

  terminal.print("g_obSeen: ");
  terminal.print(g_obSeen);

//  if (g_obSeen) {
//    terminal.print("  obTurnRight:");
//    terminal.print(g_obTurnRight);
//    terminal.print("  g_obError:");
//    terminal.print(g_obError);
//    terminal.print("  g_obHeight:");
//    terminal.print(g_obHeight);
//    terminal.print("  obElevation:");
//    terminal.print(g_obElevation);
//  } else {
//    terminal.print(" ob Not seen ");
//  }
   
//  terminal.print("g_beaconSeen: ");
//  terminal.print(g_beaconSeen);
  
//  g_headingError = constrain (g_headingError - (float(g_obError) * (float(g_obHeight)/100) ), -170, 170);
  
//  if (g_beaconSeen) {
    terminal.print("  g_headingError:");
    terminal.print(g_headingError);
//    terminal.print("  g_obError:");
//    terminal.print(g_obError);
//    terminal.print("  g_obHeight:");
//    terminal.print(g_obHeight);
    terminal.print("  g_beaconWidth: ");
    terminal.print(g_beaconWidth);
//  } else {
//    terminal.print(" beacon Not seen");
//  }
//  
  terminal.println(" ");
  
//  delay(15);
}


// #####################  PixyLookOnce  ############################################
// #####################  PixyLookOnce  ############################################
// #####################  PixyLookOnce  ############################################
// look for beacon and obstacles
              
void PixyLookOnce(boolean _obstaclesOn){
  int i=0;
  int j=0;
  uint16_t blocks;
  char buf[32]; 
  int obError=0;
  boolean seeOb = false;
  int obElevation=0;
  int beaconError=0;
  int beaconWidth=0;
  int beaconElevation=0;
  boolean obTurnRight = false;
  boolean seeBeacon = false;
  int obWidth=0;
  int obHeight=0;
  boolean obClose=false;
  int bestObHeight = 0;
  int bestObJ = -1;
  int closeRange = 55;
  static long s_PixyLookForPipes_millis = millis() - 21;
  int lastBestOb = -1;
    
  if((millis() - s_PixyLookForPipes_millis) < 21){
    return;// Has not been long enough to run this function
  }
  s_PixyLookForPipes_millis = millis();
  
  while(i < 5) {  // chances to see blocks before we give up
    blocks = pixy.getBlocks();
    
    if (blocks) { //if pixy sees blocks:
      for (j=0; j<blocks; j++) {// look through the blocks again for obstacles
        if (pixy.blocks[j].signature == 1) {// sig 1 is obstacle
          // calc block values
          obElevation = pixy.blocks[j].y;
          obHeight = pixy.blocks[j].height;
          
          if ( bestObHeight < obHeight && closeRange < obHeight ) {// pick closest obstacle based on height
            bestObHeight = obHeight;
            bestObJ = j;
          }
        }//end if (pixy.blocks[j].signature == 1...
        
        if (pixy.blocks[j].signature == 19) {// 19 is Hex for sig 23 Dec - beacon
          seeBeacon = true;
          beaconError = pixy.blocks[j].x - 152;
          beaconWidth = pixy.blocks[j].width;
          beaconElevation = pixy.blocks[j].y;
        }
      }//end for(j...
    }//end if blocks...
    
    if (bestObJ > -1 && bestObJ > lastBestOb) { // we saw a good obstacle, pull it
      obElevation = pixy.blocks[bestObJ].y;
      obWidth = pixy.blocks[bestObJ].width; 
      obHeight = pixy.blocks[bestObJ].height;  
      obError = flipHeadingError(pixy.blocks[bestObJ].x - 152); //152 is about the center, right side is neg, left positive
      
      if (obHeight > closeRange) {
        obClose = true;
      } else { 
        obClose = false; 
      }
      
      if (obError > 0) {
        obTurnRight = false;
      } else {
        obTurnRight = true;
      }

      seeOb = true;
    } else {
//      terminal.println("no obstacles seen");
    }      
    
    lastBestOb = bestObJ;
    
//    terminal.print("i: ");
//    terminal.print(i);
//    terminal.print(" lastBestOb: ");
//    terminal.print(lastBestOb);
//    terminal.print(" seeBeacon: ");
//    terminal.println(seeBeacon);
    
    if ( (seeOb == true || _obstaclesOn == false ) && seeBeacon == true) break;// break out of while loop on first valid obstacle and beacon
    
    bestObJ = -1; // reset
    i++;

    delay(21);
  }  //end while loop
  
  if(seeOb) {
    g_obSeen = true;
    g_obError = obError;
    g_obWidth = obWidth;
    g_obElevation = obElevation;
    g_obHeight = obHeight;
    g_obClose = obClose;
    g_obTurnRight = obTurnRight;
  } else {
    g_obSeen = false;
    g_obError = 0;
    g_obWidth = 0;
    g_obElevation = 0;
    g_obHeight = 0;
    g_obClose = false;
  }
  
  if(seeBeacon) {
    g_beaconSeen = true;
    g_headingError = beaconError;
    g_beaconWidth = beaconWidth;
    g_beaconSizeSizeVar = 0;
    g_beaconElevation = beaconElevation;
    if (g_headingError > -1) {
      g_BeaconTurnRight = true;
    } else {
      g_BeaconTurnRight = false;
    }
  } else {
    g_beaconSeen = false;
  }    
}

