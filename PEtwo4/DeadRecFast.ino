// #####################  StraightToCurve  ############################################

void DeadRecFast() {   
  float p_maxSpeed = 10000;
  float p_distance = 0.5;
  float p_accel = 401;
  float enc=0;
  
  roboclaw.ResetEncoders(0x80);
  
  p_distance = p_distance * 12 * CLICKS_PER_INCH;
  
  // ramp up speed
  terminal.println( " ramp up " );
  for (int i = 0; i < int(p_maxSpeed); i=i+int(p_accel) ) {
    RoboDrive(i,i);
    delay(41);
  }

  // slow for turn
  Serial.println( " slow for turn " );
  while(p_distance*.7 > ReturnAvgEnc()) delay(41);
  
  RoboDrive(p_maxSpeed*.8, p_maxSpeed*.8);

  // continue to end of straight
  while(p_distance > ReturnAvgEnc()) delay(41);

  // #### First curve ################################################################################
  Serial.println( " First curve " );
  p_distance = 3.0;
  p_accel = 40;
  float turnFactor = .75; 
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

  // #### reverse curve ###########################################################################
  Serial.println( " reverse curve A " );
  p_distance = 3.0;
  p_accel = 150;
  turnFactor = .95;
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
  p_distance = 2;
  p_accel = 40;
  turnFactor = .59;
  roboclaw.ResetEncoders(0x80);
  enc = 0;
  p_distance = p_distance * 12 * CLICKS_PER_INCH;

  RoboDrive(4000, 5500);

  RoboDrive(p_maxSpeed*.1*turnFactor, p_maxSpeed*.1);  
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
  while(p_distance*.25 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }
  
  RoboDrive(p_maxSpeed*.7*turnFactor, p_maxSpeed*.7);  
  while(p_distance > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  }

  // #### Final run to finish ############################################################
  Serial.println( " Final run to finish " );
  p_distance = 4.5;
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
  
  while(p_distance*.83 > enc) {
    static int d = 0;
    d++;
    if (d%200 == 0) enc = ReturnAvgEnc();
  } 
  
  RoboDrive(p_maxSpeed*.6,p_maxSpeed*.6);
  
  while(p_distance*.87 > enc) {
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
  
  while(p_distance*1.2 > enc) {
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


