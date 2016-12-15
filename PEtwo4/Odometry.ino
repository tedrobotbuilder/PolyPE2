// Odometry

#define WHEEL_BASE 8.25
#define LEFT_CLICKS_PER_INCH 151.85 //29.47
#define RIGHT_CLICKS_PER_INCH 151.65 //29.47
#define TWOPI 6.28
#define DEGREES_PER_INCH 14.8 //the smaller the farther you turn

// =============================================== EncoderTurn
// ===========================================================
// ===========================================================
// turns the robot base on degrees given
/*void EncoderTurn(int _speed, int _degrees, bool _turnRight){
	int turnFactor;
	if(_turnRight == true)
		turnFactor = -1;
	else
		turnFactor = 1;

	float distInches = abs(_degrees) * (1/DEGREES_PER_INCH);
	float distClicks = distInches * LEFT_CLICKS_PER_INCH;

	encodeReset();

	while(abs(nMotorEncoder[left]) <= distClicks && abs(nMotorEncoder[right]) <= distClicks) {
  	  motor[right] = speed*turnFactor;
          motor[left] = -speed*turnFactor;
          nxtDisplayTextLine(2, "LeftEn= %1.0f", abs(nMotorEncoder[left]));
          nxtDisplayTextLine(3, "RightEn= %1.0f", abs(nMotorEncoder[right]));
          nxtDisplayTextLine(4, "distClicks= %1.2f", distClicks);
	}
	motor[right] = 0;
	motor[left] = 0;
	nxtDisplayTextLine(2, "LeftEn= %1.0f", nMotorEncoder[left]);
	nxtDisplayTextLine(3, "RightEn= %1.0f", nMotorEncoder[right]);
	nxtDisplayTextLine(4, "distClicks= %1.2f", distClicks);
}*/


//======================================================
//======================================== Encoder Drive
/*======================================================
void EncoderDrive(int distright, int distleft, int motorright, int motorleft){
	float distrightInches = distright * ClicksPerInch;
	float distleftInches = distleft * ClicksPerInch;

	nMotorEncoder[left] = 0;
	nMotorEncoder[right] = 0;
	while(abs(nMotorEncoder[left]) <= distleftInches && abs(nMotorEncoder[right]) <= distrightInches) {
  	  motor[right] = motorright;
	  motor[left] = motorleft;
	}
	motor[right] = 0;
	motor[left] = 0;
}
*/


// Gets angle, thetaH, for heading when tank turning ONLY!
void CalcAngle() { //======================================================
  int lsamp, rsamp, L_ticks, R_ticks, last_left, last_right;
  float left_inches, right_inches, inches;

  /* sample the left and right encoder counts as close together */
  /* in time as possible */
  lsamp = ReturnRightEnc();
  rsamp = ReturnLeftEnc();

  /* determine how many ticks since our last sampling? */
  L_ticks = lsamp - last_left;
  R_ticks = rsamp - last_right;

  /* and update last sampling for next time */
  last_left = lsamp;
  last_right = rsamp;

  /* convert longs to floats and ticks to inches */
  left_inches = (float)L_ticks / LEFT_CLICKS_PER_INCH;
  right_inches = (float)R_ticks / RIGHT_CLICKS_PER_INCH;

  /* calculate distance we have traveled since last sampling */
  inches = (left_inches - right_inches) / 2.0;

//  terminal.println(inches);

  thetaH = (inches * DEGREES_PER_INCH);
  
  Serial.print( " ReturnAvgEnc: " );
  Serial.println( ReturnAvgEnc() );
}

// =======================================================
long g_last_CalcXY_millis = 0;
int g_last_left = 0;
int g_last_right = 0;
int g_lapCounter;
float g_lapTime;
long g_inches_millis;

void CalcXY() { //==========================================
  long leftEnc;
  long rightEnc;
  long L_ticks;
  long R_ticks;
  float left_inches;
  float right_inches;
  float inches;

  leftEnc = -encoder1();
  rightEnc = -encoder2();

  while (abs(leftEnc) > 100000000) {
    leftEnc = -encoder1();
  }

  while (abs(rightEnc) > 100000000) {
    rightEnc = -encoder2();
  }

  /* determine how many ticks since our last sampling */
  L_ticks = leftEnc - g_last_left;
  R_ticks = rightEnc - g_last_right;

  /* and update last sampling for next time */
  g_last_left = leftEnc;
  g_last_right = rightEnc;

  /* convert longs to floats and ticks to inches */
  left_inches = (float)L_ticks / LEFT_CLICKS_PER_INCH;
  right_inches = (float)R_ticks / RIGHT_CLICKS_PER_INCH;

  /* calculate distance we have traveled since last sampling */
  inches = (left_inches + right_inches) / 2.0; // distance this loop
  g_total_inches += inches; // add to total

  /* accumulate total rotation around our center */
  g_heading += (left_inches - right_inches) / WHEEL_BASE;

  /* clip the rotation to plus or minus 360 degrees (2 PI, in radians)*/
  g_heading -= (float)((int)(g_heading / TWOPI)) * TWOPI;

  /* calculate and accumulate our position in inches */
  X_pos += inches * cos(g_heading);
  Y_pos += inches * sin(g_heading);

  //  if(((millis() - g_inches_millis) > 200) && pushPipeStep == "heading") {
  //    g_inches_millis = millis();
  //    Serial3.print(g_heading);
  //    Serial3.print("\t");
  //    Serial3.print(g_total_inches);
  //    Serial3.print("\t");
  //    Serial3.print(leftEnc);
  //    Serial3.print("\t");
  //    Serial3.println(rightEnc);
  //  }

  // Detect Lap
  if (g_mode == LINEFOLLOW) {
    if (abs(X_pos) < 8 && abs(Y_pos) < 8 && abs(g_total_inches) > 20) { // we aren't still at the start
      if (X_pos > 0) { // we just crossed the starting line
        //        Serial3.println("Hit Lap");
        Melody(ledPin, 1000, 10);
        g_lapTime = (float)(millis() - g_startTime) / 1000;
        float lapSpeed = .0254 * g_total_inches / g_lapTime;
        g_lapCounter++;
        //        g_startLineFollow = true;
        g_startTime = millis();// start the stopwatch to time the lap
        //        XBeeSend("yw",g_lapTime);
        //        HandelSSButton();
        //        Drive_Motors(0);
        //        Turn_Motors(0);
        //        Serial3.print("g_forwardSpeed:");
        //        Serial3.print(g_forwardSpeed);
        //        Serial3.print("\t");
        //        Serial3.print("PID:");
        //        Serial3.print(kp);
        //        Serial3.print(" ");
        //        Serial3.print(ki);
        //        Serial3.print(" ");
        //        Serial3.print(kd);
        //        Serial3.print("\t");
        //        Serial3.print("lap speed:");
        //        Serial3.print(lapSpeed);
        //        Serial3.print("\t");
        //        Serial3.print("lap time:");
        //        Serial3.println(g_lapTime);
        //        for(int i=1; i <= g_recordPointCounter; i++){
        //          Serial3.print(pointX[i]);
        //          Serial3.print(",");
        //          Serial3.println(pointY[i]);
        //        }
        //        delay(1000);// wait for robot to stop
        g_last_total_inches = 0;
        g_total_inches = 0;// keeps next loop from entering this sub
        X_pos = 0;//reset position to starting position
        X_pos = 0;
      }
    }
  }

  //  XBeeSend("xp",X_pos);
  //  XBeeSend("yp",Y_pos);
  // XBeeSend("ya",180*g_heading/3.14);
}

/* ========================================================================
        bearing in radians from current position
        inputs are:  X_target, X_pos, and Y_target, Y_pos
        outputs are: target_distance, heading_error                        */


void locate_target() { //=====================================================
  float x, y;
  x = X_target - X_pos;
  y = Y_target - Y_pos;

  //  Serial.print("#");
  //  Serial.print(x);
  //  Serial.print("#");
  //  Serial.print(y);
  //  Serial.print("#");
  //  Serial3.print("\t");

  target_distance = sqrt((x * x) + (y * y));

  // no divide-by-zero allowed!
  if (x == 0 && y > 0) {
    //    Serial.print(" hit1 ");
    target_bearing = 1.57;
  }
  else if (x == 0 && y < 0) {
    //    Serial.print(" hit2 ");
    target_bearing = -1.57;
  }
  else  {
    //    Serial.print(" hit3 ");
    //    Serial.print(atan(y/x));
    //    Serial.print(" hit3 ");
    target_bearing = atan(y / x);
  }

  // correct the sign of the bearing angle
  if (x < 0 && y > 0)   {
    //    Serial3.println(" hit4 ");
    target_bearing = target_bearing + 3.14;
  }
  else if (x < 0 && y < 0)  {
    //    Serial3.println(" hit5 ");
    target_bearing = target_bearing - 3.14;
  }
  //  if (x > 0) target_bearing = atan(y/x);
  //  else if (x < 0) target_bearing = -atan(y/x);

  //  Serial.print("\t");
  //  Serial.println(target_bearing*180/3.14);

  heading_error = -((target_bearing * 180 / 3.14) - (g_heading * 180 / 3.14));
  if (heading_error > 180.0) heading_error -= 360.0;
  else if (heading_error < -180.0) heading_error += 360.0;
}

