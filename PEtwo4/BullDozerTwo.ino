// BullDozer Two

// #####################  BullDozerTwo  ############################################
// find pipe, push while not falling off table

//boolean g_leftEdgeSensor=false;
//boolean g_rightEdgeSensor=false;
//boolean g_centerEdgeSensor=false;
//boolean g_EdgeSensor=false;

void BullDozerTwo(boolean p_seekRight) {
  static long s_iUnwind_millis = millis() - 6001;
  int millisToUnwindI = 6000;
  int UnwindPercent = .15;
  int maxDozerSpeed = 2800;
  int searchSpeed = 3000;
  int pushSpeed = 2700;
  int pOb = 0;
  static long s_DozerMillis = millis() - 11;
  static boolean s_FoundOb = false;
  static int s_iOb = 0;
  static int s_obSpeed = 0;
  static int s_last_pOb = 0;
  static boolean s_seekRight = p_seekRight;
  static int s_errorBias = 0;
//  static boolean s_followEdgeMode = false;
//  static boolean s_FoundEdge = false;
  
  if((millis() - s_DozerMillis) < 10) return;
  s_DozerMillis = millis();
  
  QTREdgeCal();
  
  PixyLookForPipes();// narrow field for PID
  
  LookForEdge();
  
  // Search
  if (g_obSeen == false) {// search for ob ##############################################
//    terminal.println("Search ");

    s_errorBias = 0;
    
    if (s_seekRight) {
      RoboDrive(searchSpeed,-searchSpeed);
    } else {
      RoboDrive(-searchSpeed,searchSpeed);
    }
    
    return;
  } 
  
  if (g_obSeen == false && s_FoundOb == true) {// Lost ob #####################################################
//    terminal.println("lost ob ");
    
    // might have been pushed off table, back up and search
    
    if (Bot.EdgeSeen == true) {// hopefully we just pushed a pipe off, back up and reset
      BackUpEasy(2000,50,150);
      delay(500);
      RoboDrive(0,0);
      delay(100);
    }

//    EasySlowDown(500,20,150, true);

    RoboDuty(0,0);
    
    s_FoundOb = false;
//    Bot.EdgeSeen = false;//????????????????????
//    s_atEdge = false;

    return;
  }
  
  if (g_obSeen == true && Bot.EdgeSeen == true && !g_obClose) {// bump off edge #####################################################
//    terminal.print("Bump off edge");
//    terminal.print(" g_obSeen: ");
//    terminal.print(g_obSeen);
//    terminal.print(" g_obClose: ");
//    terminal.print(g_obClose);
//    terminal.print(" g_obHeight: ");
//    terminal.print(g_obHeight);
//    terminal.print(" g_obError: ");
//    terminal.print(abs(g_obError) );
//    terminal.print(" Bot.rightEdgeSeen: ");
//    terminal.println(Bot.rightEdgeSeen);
    
    if(Bot.rightEdgeSeen) {//Turn left, keep going
      s_errorBias = 100;
      
      while(Bot.rightEdgeSeen) {
        LookForEdge();
        RoboDrive(maxDozerSpeed/4,maxDozerSpeed*1.2);
////        delay(100);
      }
////      RoboDrive(0,0);
//      RoboDrive(1000,1000);
//      delay(300);
      
    } else {//Turn right, keep going
      s_errorBias = -100;
      
      while(Bot.leftEdgeSeen) {
        LookForEdge();
        RoboDrive(maxDozerSpeed*1.2,maxDozerSpeed/4);
////        delay(100);
      }
//      RoboDrive(0,0);
//      RoboDrive(1000,1000);
//      delay(300);
    }
 
    return;
  } 
  
//  if (s_followEdgeMode == true) {// #######################################################
//    terminal.println(" Following edge");
    
    
  if (g_obSeen == true && Bot.EdgeSeen == true && g_obClose) {//at edge with pipe, push off #################################
//    terminal.print("at edge with pipe, push off. Bot.rightEdgeSeen: ");
//    terminal.println(Bot.rightEdgeSeen);
//    LookForEdgeTest();
    
    //step 1: turn tward edge
    if(Bot.rightEdgeSeen && !Bot.leftEdgeSeen) {
      while(!Bot.leftEdgeSeen) {
        RoboDrive(pushSpeed,-200);//square up with edge
        LookForEdge();
//        PixyLookForPipes(200);
//        terminal.println("push left");
        s_seekRight = true;//turn right after pipe falls
      }
    } else if (!Bot.rightEdgeSeen && Bot.leftEdgeSeen) {
      while(!Bot.rightEdgeSeen) {
        RoboDrive(-200,pushSpeed);//square up with edge
        LookForEdge();
//        PixyLookForPipes(200);
//        terminal.println("push right");
        s_seekRight = false;//left right after pipe falls
      }
    }
    
    RoboDrive(-2000,-2000);
    delay(50);
    RoboDrive(0,0);
    delay(50);
    
//    terminal.println("Square with edge now");
    
    while(g_obSeen && abs(g_obError) < 30) {
      RoboDrive(pushSpeed/12,pushSpeed/12);//corner push
      Melody(ledPin, 5000, 21);
      PixyLookForPipes();
//      terminal.println("Still see pipe at edge");
    }
    
//    terminal.println("Pipe should be pushed off");
    
    RoboDrive(0,0);// stop
    delay(100);
    
//    RoboDuty(0,0);
//    while(true);
    
    BackUpEasy(2000,50,100);// backup ready for new search
//    EasySlowDown(2000,20,150, false);
    RoboDrive(0,0);
    
//    s_atEdge = false;//clear flag
    s_FoundOb = false;//clear flag
//    s_obSpeed = 0; // ramp up speed again

    return;
  }
  
  if (g_obSeen == true && Bot.EdgeSeen == false) { // push ob ##########################
//    terminal.println(" Pushing Obstacle to edge");
    
    int pOb = -g_obError + s_errorBias; // P
    float dOb = pOb - s_last_pOb; // D
    s_iOb = s_iOb + pOb; // I
    
    if((millis() - s_iUnwind_millis) < millisToUnwindI){
      s_iOb = s_iOb * UnwindPercent;
      s_iUnwind_millis = millis();
    }
    
    s_last_pOb = pOb; // save current P for next loop calc of D
    
    float power_difference = (pOb*15 + s_iOb*.1 + dOb*5);// Multiply by PID constants
  
//    if(g_IsBeaconKillerOB == true){// Set top speed
//      g_BeaconKillerSpeed == g_BeaconKillerOBSpeed;
//    }
  
    if(s_obSpeed < maxDozerSpeed){// ramp up to set speed
      s_obSpeed = s_obSpeed + 100;
    } else {
      s_obSpeed = maxDozerSpeed;
    }
    
//    float RightMotorSpeed = ( constrain(s_beaconSpeed + power_difference, -Bot.maxMotorSpeed, Bot.maxMotorSpeed) );
//    float LeftMotorSpeed = ( constrain(s_beaconSpeed - power_difference, -Bot.maxMotorSpeed, Bot.maxMotorSpeed) );
    
    float rightSpeed = s_obSpeed + power_difference;
    float leftSpeed = s_obSpeed - power_difference;
    
    RoboDrive(leftSpeed, rightSpeed);
    
    return;
  }
  
  terminal.println("FALL THROUGH ################################################");
  terminal.print("s_FoundOb: ");
  terminal.print(s_FoundOb);
  terminal.print(" g_obSeen: ");
  terminal.print(g_obSeen);
  terminal.print(" abs(g_obError): ");
  terminal.print(abs(g_obError));
  terminal.print(" Bot.EdgeSeen: ");
  terminal.print(Bot.EdgeSeen);
  terminal.print(" g_obClose: ");
  terminal.println(g_obClose);
  
//  BackUpEasy(2000,50,100);
}

// #####################  EasySlowDownTest  ############################################
void EasySlowDownTest() {
  BackUpEasy(2000,40,150);
  
  EasySlowDown(2000,20,150, false);
  
  delay(2000);
}

// #####################  EasySlowDown  ############################################
void EasySlowDown(int p_currentSpeed, int p_decel, int p_count, boolean p_forward) {//#################################################
  
  for (int i = 0; i < p_count; i++){
    if(p_currentSpeed > 0){// ramp up to set speed
      p_currentSpeed = p_currentSpeed - p_decel;
    } else {
      p_currentSpeed = 0;
    }
    
    if(p_forward){
      RoboDrive(p_currentSpeed, p_currentSpeed);
    } else {
      RoboDrive(-p_currentSpeed, -p_currentSpeed);
    }
  }
}

// #####################  BackUpEasyTest  ############################################
void BackUpEasyTest() {
  BackUpEasy(2000,50,150);
  RoboDrive(0,0);
  delay(2000);
}

// #####################  BackUpEasy  ############################################
void BackUpEasy(int p_maxSpeed, int p_accel, int p_count) {//#################################################
  static int s_currentSpeed=0;
  int i=0;
  
  for (i = 0; i < p_count; i++){
    if(!Bot.EdgeSeen) {//stop backing up if we get back to edge
      RoboDrive(0,0);
      return;
    }
    
    if(s_currentSpeed < p_maxSpeed){// ramp up to set speed
      s_currentSpeed = s_currentSpeed + p_accel;
    } else {
      s_currentSpeed = p_maxSpeed;
    }
    
    s_currentSpeed = constrain (s_currentSpeed, -31900, 31900);
    
    RoboDrive(-s_currentSpeed, -s_currentSpeed);
  }
  
  s_currentSpeed = 0;
}

// #####################  PixyTrackingTest  ############################################
// puppy mode

void PixyTrackingTest() {//######################################################
  digitalWrite(EMITTER_PIN, LOW); // turn off QTR line sensors
  PixyLookForPipes();
  static int s_obSpeed = 0;
  static float s_last_pOb;
  static float s_iOb;
  static long s_iUnwind_millis = millis() - 6001;
  int millisToUnwindI = 6000;
  int UnwindPercent = .15;
  
  int pOb = -g_obError; // P
  float dOb = pOb - s_last_pOb; // D
  s_iOb = s_iOb + pOb; // I
  
  if((millis() - s_iUnwind_millis) < millisToUnwindI){
    s_iOb = s_iOb * UnwindPercent;
    s_iUnwind_millis = millis();
  }
  
  s_last_pOb = pOb; // save current P for next loop calc of D
  
  float power_difference = (pOb*15 + s_iOb*.01 + dOb*1);// Multiply by PID constants
  
  s_obSpeed = (145-g_obElevation)*10;
  
  float rightSpeed = constrain(s_obSpeed + power_difference, -31900, 31900);
  float leftSpeed = constrain(s_obSpeed - power_difference, -31900, 31900);
  
  RoboDrive(leftSpeed, rightSpeed);

//    terminal.print("  obElevation:");
//    terminal.print(g_obElevation);
//    terminal.print(" s_obSpeed: ");
//    terminal.println(s_obSpeed);
}

// #####################  LookForEdgeTest  ############################################
void LookForEdgeTest() {//######################################################

  QTREdgeCal();
  
  LookForEdge();
  
  terminal.print("LeftSeen:");
  terminal.print(Bot.leftEdgeSeen);
  terminal.print(" LeftValue:");
  terminal.print(edgeSensorValues[LEFTEDGE]);
  terminal.print(" RightSeen:");
  terminal.print(Bot.rightEdgeSeen);
  terminal.print(" RightValue:");
  terminal.print(edgeSensorValues[RIGHTEDGE]);
  terminal.print(" BothSeen:");
  terminal.println(Bot.EdgeSeen);
}

// #####################  LookForEdge  ############################################

void LookForEdge() {  
  unsigned int edgeReading = qtrEdge.readLine(edgeSensorValues);
  
  if (edgeSensorValues[LEFTEDGE] < 800) {
    Bot.leftEdgeSeen = false;// dont see edge
  } else {
    Bot.leftEdgeSeen = true;// seen edge
  }
  
  if (edgeSensorValues[RIGHTEDGE] < 800) {
    Bot.rightEdgeSeen = false;
  } else {
    Bot.rightEdgeSeen = true;
  }

  if (Bot.leftEdgeSeen || Bot.rightEdgeSeen) {
    Bot.EdgeSeen = true;
  } else {
    Bot.EdgeSeen = false;
  }
}

// #####################  PixyReportMinMax  ############################################

void PixyReportMinMax() {
  static int s_minWidth=500;
  static int s_minHeight=500;
  static int s_minElevation=500;
  static int s_maxWidth=0;
  static int s_maxHeight=0;
  static int s_maxElevation=0;
  uint16_t blocks;
  static long i=0;
  char buf[32]; 
  
  blocks = pixy.getBlocks();
  
//      sprintf(buf, "Detected %d:\n", blocks);
//      terminal.print(buf);
  
  if (blocks) {
    i++;
    if (i%50==0)    {
//      sprintf(buf, "Detected %d:\n", blocks);
//      terminal.println(buf);
      
      for (int j=0; j<blocks; j++) {// look through the blocks again for obstacles
        if (pixy.blocks[j].signature == 1) {
          int tempElevation = pixy.blocks[j].y; // Elevation
          int tempWidth = pixy.blocks[j].width; // * pixy.blocks[j].height;
          int tempHeight = pixy.blocks[j].height; 
          
          if (tempElevation < s_minElevation) s_minElevation = tempElevation;
          if (tempHeight < s_minHeight) s_minHeight = tempHeight;
          if (tempWidth < s_minWidth) s_minWidth = tempWidth;
          
          if (tempElevation > s_maxElevation) s_maxElevation = tempElevation;
          if (tempHeight > s_maxHeight) s_maxHeight = tempHeight;
          if (tempWidth > s_maxWidth) s_maxWidth = tempWidth;
          
//          terminal.print("minElv: ");
//          terminal.print(s_minElevation);
//          terminal.print(" minHeight: ");
//          terminal.print(s_minHeight);
//          terminal.print(" minWidth: ");
//          terminal.print(s_minWidth);
//          
//          terminal.print(" - maxElv: ");
//          terminal.print(s_maxElevation);
//          terminal.print(" maxHeight: ");
//          terminal.print(s_maxHeight);
//          terminal.print(" maxWidth: ");
//          terminal.println(s_maxWidth);
  
          terminal.print("tempElevation: ");
          terminal.print(tempElevation);
          terminal.print(" tempHeight: ");
          terminal.print(tempHeight);
          terminal.print(" tempWidth: ");
          terminal.println(tempWidth);
        }//end if
      }//end for j
    }// end i
  }//end if(blocks)
    
//  terminal.print("minElv: ");
//  terminal.print(s_minElevation);
//  terminal.print(" minHeight: ");
//  terminal.print(s_minHeight);
//  terminal.print(" minWidth: ");
//  terminal.print(s_minWidth);
//  
//  terminal.print(" - maxElv: ");
//  terminal.print(s_maxElevation);
//  terminal.print(" maxHeight: ");
//  terminal.print(s_maxHeight);
//  terminal.print(" maxWidth: ");
//  terminal.println(s_maxWidth);
}

// #####################  PixyLookForPipesTest  ############################################

void PixyLookForPipesTest() {
  PixyLookForPipes();

  terminal.print("g_obSeen: ");
  terminal.print(g_obSeen);

  if (g_obSeen) {
    terminal.print("  g_obError:");
    terminal.print(g_obError);
    terminal.print("  obWidth:");
    terminal.print(g_obWidth);
    terminal.print("  obHeight:");
    terminal.print(g_obHeight);
    terminal.print("  obElevation:");
    terminal.print(g_obElevation);
    terminal.print("  obElevation/Height:");
    terminal.print( float(g_obElevation)/float(g_obHeight) );
  } else {
    terminal.print("-----ob Not seen------");
  }
  
  terminal.println(" ");
}

// #####################  PixyLookForPipes  ############################################
// look for obstacles

void PixyLookForPipes() {
  int i=0;
  int j=0;
  uint16_t blocks;
  char buf[32]; 
  int obError=0;
  int obWidth=0;
  boolean seeOb = false;
  int obElevation=0;
  int obHeight=0;
  boolean obClose=false;
  int bestObHeight = 0;
  int bestObJ = -1;
  int closeRange = 55;
  float elvToHeight = 0;
  
  static long s_PixyLookForPipes_millis = millis() - 21;
    
  if((millis() - s_PixyLookForPipes_millis) < 21){
    return;// Has not been long enough to run this function
  }
  s_PixyLookForPipes_millis = millis();
  
  while(i < 5) {  // chances to see ob before we give up
    blocks = pixy.getBlocks(); 
    
    if (blocks) { //if pixy sees blocks:
      for (j=0; j<blocks; j++) {// look through the blocks again for obstacles
        if (pixy.blocks[j].signature == 1) {// sig 1 is ob
          obElevation = pixy.blocks[j].y;
          obHeight = pixy.blocks[j].height;
          float elvToHei = float(obElevation) / float(obHeight);
          
          if ( (obElevation <= 66 || (obElevation > 66 && obElevation < 90 && elvToHei < 4) ) && bestObHeight < obHeight ) {// (is above the table) && (closest obstacle)
            bestObHeight = obHeight;
            bestObJ = j;
          }    
        }//end if (pixy.blocks[j].signature == 1 || seeOb != true)
      }//end for(j...
    } 
    
    if (bestObJ > -1) { // we saw a good block, pull it
      // calc block values
      obElevation = pixy.blocks[bestObJ].y;
      obWidth = pixy.blocks[bestObJ].width; 
      obHeight = pixy.blocks[bestObJ].height;  
      obError = pixy.blocks[bestObJ].x - 152; //152 is about the center, right side is neg, left positive
      
      if (obHeight > closeRange) {// if the obstacle is close enough to turn square with edge to push off
        obClose = true;
      } else { 
        obClose = false; 
      }
      
      seeOb = true;
    } else {
//      terminal.println("no obstacles seen on table");
    }      
    
    if (seeOb == true) break;// break out of while loop on first valid obstacle ################
    
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
  } else {
    g_obSeen = false;
    g_obError = 0;
    g_obWidth = 0;
    g_obElevation = 0;
    g_obHeight = 0;
    g_obClose = false;
  }
}
