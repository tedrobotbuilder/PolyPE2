// Beacon Killer With Obstacles

 boolean g_FoundBeacon = false;
 int g_ObTurnfactor;
  
// #####################  BeaconKillerWO  ############################################
// #####################  BeaconKillerWO  ############################################
// #####################  BeaconKillerWO  ############################################

void BeaconKillerWO(boolean _obstaclesOn) {
  if((millis() - g_BeaconKiller_millis) < 1) return;
  g_BeaconKiller_millis = millis();
  
  int i;
  int j;
  uint16_t blocks;
  char buf[32]; 
//  static int beaconError = 5000;
  int beaconSize;
  int pBeacon = 0;
  int searchSpeed = 1500;
  static float s_iBeacon = 0;
  static float s_last_pBeacon = 0;
  static int s_beaconSpeed = 0;// current bot speed
  int maxBeaconSpeed = 5500;// max speed limit for beacon killer
  int maxBeanonSpeedNoOb = 9500;//
  static float s_iOb=0;
  static float s_last_pOb=0;
  float hookFactor = 50;
  float beaconDistance = 8 * 12 * CLICKS_PER_INCH;
  float holdEncoderValue = 0;
  
//  PixyLookWO();

//  terminal.print(" beaconSeen: ");
//  terminal.print(g_beaconSeen);
//  terminal.print(" g_FoundBeacon: ");
//  terminal.print(g_FoundBeacon);
//  terminal.print(" g_obSeen: ");
//  terminal.print(g_obSeen);
//  terminal.print(" ObOn: ");
//  terminal.println(_obstaclesOn);
    
  
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
//    terminal.println("Search ");
    
    if (g_BeaconTurnRight) {
//      terminal.println(" Search Right ");
      RoboDrive(searchSpeed,-searchSpeed);
    } else {
//      terminal.println(" Search Left ");
      RoboDrive(-searchSpeed,searchSpeed);
    }
    
    return;
  }

  //##################################### found beacon ######################################################################
  
  if (g_beaconSeen == true && g_FoundBeacon == false) {
//    terminal.print(" found beacon ");
//    RoboStop();
    
//    terminal.print(" g_FoundBeacon: ");
//    terminal.print(g_FoundBeacon);
    
    g_FoundBeacon = true;
    
//    terminal.print(" g_FoundBeacon: ");
//    terminal.println(g_FoundBeacon);
  }

  //##################################### avoid obstacle ######################################################################
  if (g_obSeen == true && _obstaclesOn && g_FoundBeacon) {// avoid obstacle
//    terminal.print(" avoid obstacle ");
//    terminal.print(" g_obError: ");
//    terminal.println(g_obError);
    
    //avoidOb(int p_speed, boolean p_turnRight, float p_distance, float p_overRun, float p_turn)
    avoidOb(maxBeaconSpeed, g_obTurnRight, .7*abs(g_obError)/152, .3, g_ObTurnfactor);
 
    return;
  }

  //##################################### Drive ######################################################################
  if (g_beaconSeen == true && (g_obSeen == false || _obstaclesOn == false) ) {// We are tracking beacon = kill it
//    terminal.println("Drive ");

//    terminal.print(" beaconSeen: ");
//    terminal.print(g_beaconSeen);
//    terminal.print(" obSeen: ");
//    terminal.print(g_obSeen);
//    terminal.print(" ObOn: ");
//    terminal.println(_obstaclesOn);
//    terminal.print(" g_BeaconHook: ");
//    terminal.println(g_BeaconHook);
    
//    g_headingError = constrain (g_headingError - (float(g_obError) * (float(g_obHeight)/50) ), -170, 170);

    if (g_BeaconHook != 0) {
      holdEncoderValue = ReturnAvgEnc();
    }
    
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
    
    float power_difference = (pBeacon*8 + s_iBeacon*0 + dBeacon*2);// Multiply by PID constants
    
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
    terminal.print(" beaconSeen: ");
    terminal.print(g_beaconSeen);
    terminal.print(" g_FoundBeacon: ");
    terminal.print(g_FoundBeacon);
    terminal.print(" obSeen: ");
    terminal.print(g_obSeen);
    terminal.print(" ObOn: ");
    terminal.println(_obstaclesOn);
}


// #####################  PixyLookTestWO  ############################################
// #####################  PixyLookTestWO  ############################################v
// #####################  PixyLookTestWO  ############################################

void PixyLookTestWO() {
  static int i = 0;
  i++;
  
  PixyLookWO(false, true);

  if (i%50==0) {
    terminal.print(" g_beaconSeen: ");
    terminal.println(g_beaconSeen);
    
  }
  
}

// #####################  PixyLookWO  ############################################
// #####################  PixyLookWO  ############################################
// #####################  PixyLookWO  ############################################
// look for beacon and obstacles

void PixyLookWO(boolean p_run, boolean p_obOn) {
  static long i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  static long s_millsBetweenBeacon = 0;
  static long s_millsBetweenBlock = 0;
  int obElevation=0;
  int beaconError=0;
  int beaconWidth=0;
  int beaconElevation=0;
  int obWidth=0;
  int obHeight=0;
  int bestObHeight = 0;
  int bestObJ = -1;
  int obAvoidRange = 15;
  int obCloseRange = 25;
  int obVeryCloseRange = 35;
  boolean seeBeacon = false;
  static boolean s_madeTurn = false;

  if (s_madeTurn == false){
    s_madeTurn = true;
    GoToAngle(120, 5000, 500, g_BeaconTurnRight);
//    g_Start = false;
//    return;
  }

//  g_beaconSeen = false;// reset flag

  blocks = pixy.getBlocks();// grab blocks
  
  if (blocks) {// get data from blocks
    i++;

    if (i%2==0) {
      for (j=0; j<blocks; j++) {
        if (pixy.blocks[j].signature == 1) {// sig 1 is obstacle  
          obElevation = pixy.blocks[j].y;
          obHeight = pixy.blocks[j].height;

//          terminal.print(" j: ");
//          terminal.print(j);
//          terminal.print(" obHeight: ");
//          terminal.println(obHeight);
          
//          terminal.print("                           s_millsBetweenBlock: ");
//          terminal.println(millis() - s_millsBetweenBlock);
//          s_millsBetweenBlock = millis();
//          
          if ( bestObHeight < obHeight && obAvoidRange < obHeight ) {// pick closest obstacle based on height
            bestObHeight = obHeight;
            bestObJ = j;
          }
          
//          terminal.print(" 1bestObJ: ");
//          terminal.println(bestObJ);
    
        }//end if (pixy.blocks[j].signature == 1...

        if (pixy.blocks[j].signature == 19) {// 19 is Hex for sig 23 Dec - beacon
//            terminal.print("s_millsBetweenBeacon: ");
//            terminal.println(millis() - s_millsBetweenBeacon);

            s_millsBetweenBeacon = millis();// #### Important  ####  Do not Remark  ####
            
            g_beaconSeen = true;
            g_headingError = float(pixy.blocks[j].x) - 152;
            g_beaconWidth = pixy.blocks[j].width;
            g_beaconElevation = pixy.blocks[j].y;
            if (g_headingError > -1) {
              g_BeaconTurnRight = true;
            } else {
              g_BeaconTurnRight = false;
            }
    
        }//end if (pixy.blocks[j].signature == 19)...
      }// end for (j=0; j<blocks; j++)

//      terminal.print(" 2bestObJ: ");
//      terminal.println(bestObJ);
    
      if (bestObJ > -1) { // if we saw a good obstacle, pull it
//        g_obElevation = pixy.blocks[bestObJ].y;
//        g_obWidth = pixy.blocks[bestObJ].width; 
        g_obHeight = pixy.blocks[bestObJ].height;  
        g_obError = flipHeadingError(pixy.blocks[bestObJ].x - 152); //152 is about the center, right side is neg, left positive
        
        if (obHeight > obCloseRange) {
          g_ObTurnfactor = 1;
        } else {
          g_ObTurnfactor = 2;
        }
        
        if (obHeight > obVeryCloseRange) {
          g_ObTurnfactor = .5;
        } 
        
        if (g_obError < 0) {
          g_obTurnRight = true;
        } else {
          g_obTurnRight = false;
        }
    
        g_obSeen = true;
      } else {
    //      terminal.println("no obstacles seen");
        g_obSeen = false;
      }// end if (bestObJ > -1)
    }// end if (i%2==0)
  }// end if (blocks)


//  if (i%5==0) {
//    terminal.print("g_FoundBeacon: ");
//    terminal.print(g_FoundBeacon);
//    terminal.print(" g_beaconSeen: ");
//    terminal.print(g_beaconSeen);
//    terminal.print(" s_millsBetweenBeacon: ");
//    terminal.println(millis() - s_millsBetweenBeacon);
//  }

  if ((millis() - s_millsBetweenBeacon) > 350) {
     g_beaconSeen = false;// went too long without seeing beacon
     
//    terminal.print(" g_headingError: ");
//    terminal.println(g_headingError);

    if (g_headingError > -1) { // direction to turn if we lose beacon
      g_BeaconTurnRight = true;
    } else {
      g_BeaconTurnRight = false;
    }
  }// end if ((millis() - s_millsBetweenBeacon)...
  
  if (i%2==0 && p_run) { // only run actions on same clock as getblocks
//    Melody(ledPin, 1000, 2);
    if (p_obOn) {
      BeaconKillerWO(true);
    } else {
      BeaconKillerWO(false);
    }
  }
}


// ##################### avoidObTest ############################################
// avoid obstacle when seen

void avoidObTest() {
  avoidOb(2000, false, .8, .8, 2);
  
  RoboDrive(0,0);
  delay(50);
  RoboDuty(0,0);
  while(true) if(Serial2.available()) break;
  g_Start=false;
}

// ##################### avoidOb  ############################################
// avoid obstacle when seen

void avoidOb(int p_speed, boolean p_turnRight, float p_distance, float p_overRun, float p_turn) {
//  terminal.print("avoidOb ");
//  terminal.print(" p_speed: ");
//  terminal.print(p_speed);
//  terminal.print(" p_turnRight: ");
//  terminal.print(p_turnRight);
//  terminal.print("p_distance");
//  terminal.print(p_distance);
//  terminal.print("p_overRun");
//  terminal.print(p_overRun);
//  terminal.print("p_turn");
//  terminal.println(p_turn);

  float travelDistance=0;
  
  travelDistance = p_distance * 12 * CLICKS_PER_INCH;
  travelDistance = travelDistance + ReturnAvgEnc();
  
  while ( travelDistance > ReturnAvgEnc() ) {
//    terminal.println("hit2");
    if (p_turnRight) {
      RoboDrive(p_speed + (p_speed/p_turn) , p_speed - (p_speed/p_turn));
    } else {
      RoboDrive(p_speed - (p_speed/p_turn) , p_speed + (p_speed/p_turn));
    }
    
//    terminal.println("hit3");
    if(Serial2.available())return;
    
//    terminal.print("ReturnAvgEnc(): ");
//    terminal.println(ReturnAvgEnc());
  }
//  terminal.println("end turn 1");
//  Melody(ledPin, 1000, 100);

  travelDistance = p_distance * 12 * CLICKS_PER_INCH;
  travelDistance = travelDistance*.9 + ReturnAvgEnc();  
  
//  terminal.print("travelDistance:");
//  terminal.print(travelDistance);
//  terminal.print(" ReturnAvgEnc():");
//  terminal.println(ReturnAvgEnc());
  
  while ( travelDistance > float( ReturnAvgEnc() ) ) {
    if (p_turnRight) {
      RoboDrive(p_speed - (p_speed/p_turn) , p_speed + (p_speed/p_turn));
    } else {
      RoboDrive(p_speed + (p_speed/p_turn) , p_speed - (p_speed/p_turn));
    }
    
    if(Serial2.available())return;
    
//    terminal.print("ReturnAvgEnc(): ");
//    terminal.println(ReturnAvgEnc());
  }
//  terminal.println("end turn 2");
//  Melody(ledPin, 1000, 100);

  travelDistance = p_overRun * 12 * CLICKS_PER_INCH;
  travelDistance = travelDistance + ReturnAvgEnc();
  
  while ( travelDistance > float( ReturnAvgEnc() ) ) {
    RoboDrive(p_speed, p_speed);
    
    if(Serial2.available())return;
    
//    terminal.print("ReturnAvgEnc(): ");
//    terminal.println(ReturnAvgEnc());
  }
//  terminal.println("end over run");
//  Melody(ledPin, 1000, 100);
}


// ##################### flipHeadingError  ############################################
// for obstacle heading
// give stronger error at center and less error as ob moves off-center, opposite of data given from pixy

int flipHeadingError(int _obError) {
  if (_obError == 0) return 170;// ob is dead ahead, return max error
  
  if (abs(_obError) >= 170) return 0;// range is +/-170 so this is out of range, return min error which is zero
  
  int signOfobError = abs(_obError) / _obError;// find the sign of the error
  
  return (signOfobError*170) - _obError;
}

