// ================== Dead Reckoning ===================
//clockwise direction!

int g_logic_step = 1;

// #####################  DeadReconing  ############################################

void DeadReconing() {  
  DeadRec8000();
  
  RoboStop();
  while(true);
  
}



// #####################  DeadRec8000  ############################################

void DeadRec8000() {   
  float p_maxSpeed = 8000;
  float p_distance = .7;
  float p_accel = 400;
  float enc=0;
  
  roboclaw.ResetEncoders(0x80);
  
  p_distance = p_distance * 12 * CLICKS_PER_INCH;
  
  // ramp up speed
  terminal.println( " ramp up " );
  for (int i = 0; i < int(p_maxSpeed); i=i+int(p_accel) ) {
    RoboDrive(i,i);
    
//    RoboDrive(700,700);
    delay(41);
    terminal.println(i);
  }

//  RoboStop();

  // slow for turn
  Serial.println( " slow for turn " );
  while(p_distance*.95 > ReturnAvgEnc()) delay(41);
  RoboDrive(p_maxSpeed*.8, p_maxSpeed*.8);

  // continue to end of straight
  while(p_distance > ReturnAvgEnc()) delay(41);

  // #### First curve #####
  Serial.println( " First curve " );
  p_distance = 3.6;
  p_accel = 40;
  float turnFactor = .85; 
  roboclaw.ResetEncoders(0x80);  
  p_distance = p_distance * 12 * CLICKS_PER_INCH;

  // slow for stop
  RoboDrive(p_maxSpeed*.8*turnFactor, p_maxSpeed*.8);  
  while(p_distance*.8 > ReturnAvgEnc()) delay(41);
  
  RoboDrive(p_maxSpeed*.7*turnFactor, p_maxSpeed*.7);  
  while(p_distance*.85 > ReturnAvgEnc()) delay(41); 
  
  RoboDrive(p_maxSpeed*.6*turnFactor, p_maxSpeed*.6);
  
  while(p_distance*.87 > ReturnAvgEnc()) delay(41);
  RoboDrive(p_maxSpeed*.5*turnFactor, p_maxSpeed*.5);
  
  while(p_distance*.9 > ReturnAvgEnc()) delay(41);  
   
  RoboDrive(p_maxSpeed*.3*turnFactor, p_maxSpeed*.3);
  while(p_distance*.95 > ReturnAvgEnc()) delay(41);
  
  RoboDrive(p_maxSpeed*.25*turnFactor, p_maxSpeed*.25);
  while(p_distance > ReturnAvgEnc()) delay(41);
  
  // stop
  RoboDrive(0,0);// stop  
//  Melody(ledPin, 4000, 10);

  // #### reverse curve #####
  Serial.println( " reverse curve A " );
  p_distance = 3.4;
  p_accel = 150;
  turnFactor = .73;
  roboclaw.ResetEncoders(0x80);
  p_distance = p_distance * 12 * CLICKS_PER_INCH;
  
  // ramp up speed
//  for (float i = 0; i < p_maxSpeed*.8; i=i+p_accel) {
//    static int d = 0;
//    d++;
//    if (d%200 == 0) RoboDrive(-i, -i*turnFactor);
//  }

  RoboDrive(-p_maxSpeed*.1, -p_maxSpeed*.1*turnFactor);  
  while(p_distance*.05 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 

  RoboDrive(-p_maxSpeed*.2, -p_maxSpeed*.2*turnFactor);  
  while(p_distance*.08 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 

  RoboDrive(-p_maxSpeed*.3, -p_maxSpeed*.3*turnFactor);  
  while(p_distance*.09 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 

  RoboDrive(-p_maxSpeed*.5, -p_maxSpeed*.5*turnFactor);  
  while(p_distance*.1 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 

  RoboDrive(-p_maxSpeed*.7, -p_maxSpeed*.7*turnFactor);  
  while(p_distance*.11 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 

  RoboDrive(-p_maxSpeed*.8, -p_maxSpeed*.8*turnFactor);  
  while(p_distance*.68 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 
  
  Serial.println( " reverse curve B - slowdown " );

  // slow for stop
  RoboDrive(-p_maxSpeed*.75, -p_maxSpeed*.75*turnFactor);  
  while(p_distance*.7 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 
  
  RoboDrive(-p_maxSpeed*.7,-p_maxSpeed*.7*turnFactor);  
  while(p_distance*.75 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 
  
  RoboDrive(-p_maxSpeed*.6,-p_maxSpeed*.6*turnFactor);  
  while(p_distance*.8 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 
  
  RoboDrive(-p_maxSpeed*.5,-p_maxSpeed*.5*turnFactor);  
  while(p_distance*.9 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  }   
  
  RoboDrive(-p_maxSpeed*.3,-p_maxSpeed*.3*turnFactor);  
  while(p_distance*.95 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 
  
  RoboDrive(-p_maxSpeed*.2,-p_maxSpeed*.2*turnFactor);  
  while(p_distance > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = -ReturnAvgEnc();
  } 
  
  // stop
  RoboDrive(0,0);// stop  
//  Melody(ledPin, 4000, 10);

  
  // #### Third curve - final curve ######################################################################
  Serial.println( " Third curve A " );
  p_distance = 3.4;
  p_accel = 40;
  turnFactor = .87;
  roboclaw.ResetEncoders(0x80);
  enc = 0;
  p_distance = p_distance * 12 * CLICKS_PER_INCH;

  RoboDrive(4000, 5500);

  //RoboDrive(p_maxSpeed*.1*turnFactor, p_maxSpeed*.05);  
  while(p_distance*.05 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.2*turnFactor, p_maxSpeed*.2);  
  while(p_distance*.1 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.3*turnFactor, p_maxSpeed*.3);  
  while(p_distance*.15 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }

  RoboDrive(p_maxSpeed*.5*turnFactor, p_maxSpeed*.5);  
  while(p_distance*.17 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.6*turnFactor, p_maxSpeed*.6);  
  while(p_distance*.2 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.7*turnFactor, p_maxSpeed*.7);  
  while(p_distance*.23 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.8*turnFactor, p_maxSpeed*.8);  
  while(p_distance > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }

  // #### Final run to finish ############################################################
  Serial.println( " Final run to finish " );
  p_distance = 3.3;
  p_accel = 40;
  enc = 0;
  roboclaw.ResetEncoders(0x80);
  p_distance = p_distance * 12 * CLICKS_PER_INCH;
  
  RoboDrive(p_maxSpeed,p_maxSpeed);
  
  while(p_distance*.8 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.8,p_maxSpeed*.8);
  
  while(p_distance*.81 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  } 
  
  RoboDrive(p_maxSpeed*.6,p_maxSpeed*.6);
  
  while(p_distance*.84 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  } 
  
  RoboDrive(p_maxSpeed*.5,p_maxSpeed*.5);
  
  while(p_distance*.9 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  } 
  
  RoboDrive(p_maxSpeed*.3,p_maxSpeed*.3);
  
  while(p_distance*.95 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  } 
  
  RoboDrive(p_maxSpeed*.1,p_maxSpeed*.1);
  
  while(p_distance > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  Serial.println( " End " );
  
  // stop
  RoboDrive(0,0);// stop  
  Melody(ledPin, 4000, 100);
  RoboStop();
  while(true) delay(41);
}


// #####################  DeadReconingOldStyle  ############################################

void DeadReconingOldStyle() {   
  digitalWrite(EMITTER_PIN, LOW); // turn off QTR line sensors

  GoToX(2.89,4500,40);// run 1
  
  GoToAngle(30, 800, 20, false);// turn 30 left
  
  GoToX(3,4500,40);// run 2
  
  GoToAngle(130, 800, 20, true);// turn 120 right
  
  GoToX(3,4500,40);// run 3 
  
  GoToAngle(130, 800, 20, true);// turn 120 right
  
  GoToX(3,4500,40);// run 3 
  
  GoToAngle(35, 800, 20, false);// turn 30 left

  GoToX(2.89,4500,40);// final run to finish
  
  RoboDuty(0,0);
  while(1) delay(100);
}

// #####################  GoToAngle  ############################################
// #####################  GoToAngle  ############################################
// #####################  GoToAngle  ############################################

void GoToAngle(float p_angle, int p_maxSpeed, int p_accel, boolean turnRight) {
  CalcAngle();
  
  roboclaw.ResetEncoders(0x80);
  
  for (int i = 0; i < p_maxSpeed; i=i+p_accel) {
    CalcAngle();
    
    if (!turnRight) {
      RoboDrive(-i,i);
    } else {
      RoboDrive(i,-i);
    }
  }
  
//  RoboDrive(0,0);
      
  if (!turnRight) {    
    // stay at 1000 speed for p_delay
    while(p_angle*.6 > abs(thetaH) ) CalcAngle();; 
    
    RoboDrive(-p_maxSpeed*.8,p_maxSpeed*.8);
    
    while(p_angle*.7 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(-p_maxSpeed*.6,p_maxSpeed*.6);
    
    while(p_angle*.8 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(-p_maxSpeed*.5,p_maxSpeed*.5);
    
    while(p_angle*.9 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(-p_maxSpeed*.3,p_maxSpeed*.3);
    
    while(p_angle*.95 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(-p_maxSpeed*.1,p_maxSpeed*.1);
    
    while(p_angle > abs(thetaH) ) CalcAngle(); 
  } else {
    // stay at 1000 speed for p_delay
    while(p_angle*.6 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(p_maxSpeed*.8,-p_maxSpeed*.8);
    
    while(p_angle*.7 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(p_maxSpeed*.6,-p_maxSpeed*.6);
    
    while(p_angle*.8 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(p_maxSpeed*.5,-p_maxSpeed*.5);
    
    while(p_angle*.9 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(p_maxSpeed*.3,-p_maxSpeed*.3);
    
    while(p_angle*.95 > abs(thetaH) ) CalcAngle(); 
    
    RoboDrive(p_maxSpeed*.1,-p_maxSpeed*.1);
    
    while(p_angle > abs(thetaH) ) CalcAngle(); 
  }
  
  // stop
  RoboDrive(0,0);// stop  
  Melody(ledPin, 4000, 10);
  
  
  roboclaw.ResetEncoders(0x80);
}

// #####################  GoToX  ############################################
// #####################  GoToX  ############################################
// #####################  GoToX  ############################################

void GoToX(float p_distance, int p_maxSpeed, int p_accel) {
  roboclaw.ResetEncoders(0x80);
  
  Serial.print( "Start ReturnLeftEnc: " );
  Serial.println( ReturnLeftEnc() );
  Serial.print( "ReturnRightEnc: " );
  Serial.println( ReturnRightEnc() );
  Serial.print( "ReturnAvgEnc: " );
  Serial.println( ReturnAvgEnc() );
  
  Melody(ledPin, 4000, 10);
  
  p_distance = p_distance * 12 * CLICKS_PER_INCH;
  Serial.print( "ReturnAvgEnc: " );
  Serial.println( p_distance );
  
  // ramp up speed
  for (int i = 0; i < p_maxSpeed; i=i+p_accel) {
    RoboDrive(i,i);
  }
  Melody(ledPin, 4000, 10);
  
  // stay at 1000 speed for p_delay
  while(p_distance*.6 > ReturnAvgEnc()) CalcAngle(); 
  
  RoboDrive(p_maxSpeed*.8,p_maxSpeed*.8);
  
  while(p_distance*.7 > ReturnAvgEnc()) CalcAngle(); 
  
  RoboDrive(p_maxSpeed*.6,p_maxSpeed*.6);
  
  while(p_distance*.8 > ReturnAvgEnc()) CalcAngle(); 
  
  RoboDrive(p_maxSpeed*.5,p_maxSpeed*.5);
  
  while(p_distance*.9 > ReturnAvgEnc()) CalcAngle(); 
  
  RoboDrive(p_maxSpeed*.3,p_maxSpeed*.3);
  
  while(p_distance*.95 > ReturnAvgEnc()) CalcAngle(); 
  
  RoboDrive(p_maxSpeed*.1,p_maxSpeed*.1);
  
  while(p_distance > ReturnAvgEnc()) CalcAngle(); 
  
  // stop
  RoboDrive(0,0);// stop  
  Melody(ledPin, 4000, 10);
  roboclaw.ResetEncoders(0x80);
  Serial.print( "Start ReturnLeftEnc: " );
  Serial.println( ReturnLeftEnc() );
  Serial.print( "ReturnRightEnc: " );
  Serial.println( ReturnRightEnc() );
  Serial.print( "ReturnAvgEnc: " );
  Serial.println( ReturnAvgEnc() );
}

// #####################  calEncoders  ############################################
// #####################  calEncoders  ############################################
// #####################  calEncoders  ############################################
// run bot for a few seconds then show encoder values to calibrate distance/click

void calEncoders() {
  static bool hasRun = false;
  
  if(hasRun) {// we only want calEncoders to run once, not keep looping
    g_Start = false;
    return;
  }
  
  Serial.print( "Start ReturnLeftEnc: " );
  Serial.println( ReturnLeftEnc() );
  Serial.print( "ReturnRightEnc: " );
  Serial.println( ReturnRightEnc() );
  Serial.print( "ReturnAvgEnc: " );
  Serial.println( ReturnAvgEnc() );
  
  // ramp up speed
  for (int i = 0; i < 1000; i++) {
    RoboDrive(i,i);
    delay(5);
  }
  
  // stay at 1000 speed for 2sec
  delay(2000);
  
  for (int i = 1000; i > 0; i--) {
    RoboDrive(i,i);
    delay(5);
  }
  
  // stop
  RoboDrive(0,0);// stop
  
  delay(1000);//let bot stop
  
  // stop and display both encoder values
  Serial.print( "End ReturnLeftEnc: " );
  Serial.println( ReturnLeftEnc() );
  Serial.print( "ReturnRightEnc: " );
  Serial.println( ReturnRightEnc() );
  Serial.print( "ReturnAvgEnc: " );
  Serial.println( ReturnAvgEnc() );
  
  hasRun = true;
}

