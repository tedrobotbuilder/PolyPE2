// Motors

//  roboclaw.ResetEncoders(0x80);

//  max motor settable speed: ????


// ================== Initialize Motors ===================

void Init_Motors() {  
  roboclaw.begin(38400);
  delay(50);
  roboclaw.ResetEncoders(0x80);
  delay(50);
  RoboDuty(0,0);
}

// ================== ReturnLeftEnc ===================

int32_t ReturnLeftEnc() {
  int32_t tempEncRead=0;
  int tempI=0;
  bool validRC=false;
  uint8_t statusEnc=0;

  do{
    tempEncRead = roboclaw.ReadEncM1(0x80, &statusEnc, &validRC);
    tempI++;
  }while(!validRC && tempI<1000);

  return -tempEncRead;
}

// ================== ReturnRightEnc ===================

int32_t ReturnRightEnc() {
  int32_t tempEncRead;
  int tempI=0;
  bool validRC;
  uint8_t statusEnc;
  
  do{
    tempEncRead = roboclaw.ReadEncM2(0x80, &statusEnc, &validRC);
    tempI++;
  }while(!validRC && tempI<1000);
  
  return tempEncRead;
}

// ================== ReturnAvgEnc ===================

float ReturnAvgEnc() {
  return ( float( ReturnRightEnc() ) + float( ReturnLeftEnc() ) ) /2;
}

// ================== RoboDrive - Speed Control ===================

// Drive both at a given speed motors with Roboclaw - corrects for sign on reversed motor
void RoboDrive(int _Left, int _Right) {  
  roboclaw.SpeedM1(0x80,-_Left);
  roboclaw.SpeedM2(0x80,_Right);
}

// ================== RoboDuty - Voltage Control ===================

// Drive both motors at a voltage/duty level with Roboclaw - corrects for sign on reversed motor
void RoboDuty(int _Left, int _Right) {  
  roboclaw.DutyM1(0x80,-_Left);
  roboclaw.DutyM2(0x80,_Right);
}

// ================== ReadBattVolts ===================

// Read battery volts from Roboclaw
float ReadBattVolts() {
  float tempBattReading;
  int i=0;
  bool validRC;
  
  do{
    tempBattReading = float(roboclaw.ReadMainBatteryVoltage(address, &validRC))/10;
    i++;
//    delay(1);
//    terminal.print("batt: ");
//    terminal.print(tempBattReading);
//    terminal.print(" flag:");
//    terminal.println(validRC);
    if (i > 3) {
      tempBattReading = -1;
      Bot.BattReadingGood = false;
      break;// exit if we dont get it in 3 tries
    }
  }while(!validRC && i<10);
  
  if(validRC) Bot.BattReadingGood = true;

  return tempBattReading;
}



// ================== RoboStop ===================

void RoboStop() {
  RoboDrive(0,0);
  delay(600);
  RoboDuty(0,0);
  while(true);
}


// ================== RoboBrake ===================

void RoboBrake() {
  RoboDrive(0,0);
  delay(50);
}

// ================== OLD DRIVER STUFF ===================


#define mvCsr 0x02
#define md25Address 0x58  // Address of the MD25
#define softwareReg 0x0D  // Byte to read the software version
#define speed1 0x00       // Byte to write speed to both motors
#define speed2 0x01       // Byte to write turn rate
#define cmdByte 0x10      // Command byte
#define encoderOne 0x02   // Byte to read motor encoder 1
#define encoderTwo 0x06   // Byte to read motor encoder 2
#define voltRead 0x0A     // Byte to read battery volts
#define ampsRead1 0x0B    // Byte to read battery volts
#define ampsRead2 0x0C    // Byte to read battery volts
#define Motor_Mode 0x0F   // Byte to set motor mode
#define Accel_Mode 0x0E   // Byte to set motor Acceleration mode

//#define CLICKS_PER_INCH 29.69  //29.69 Calibrated encoder clicks per inch


// ================== Drive_Motors_Joystick ===================
void Drive_Motors_Joystick(int _throttle, int _turn) {
  //SetMotorMode(3);  -Default in Init Motors
  
//  Serial.print(_throttle);
//  Serial.print(" : ");  
//  _throttle = constrain(_throttle, -120, 120);
//  Serial.println(_turn);
  
  // Always do both drive and turn!
  Drive_Motors(_throttle);
  Turn_Motors(_turn); 
}


// ================== Drive_Motors ==================
long g_Drive_Motors_millis=0;
long g_Turn_Motors_millis=0;

void Drive_Motors(byte _throttle) {
}

// ========================== Turn ======================
void Turn_Motors(byte iTurn) {
}

void SetMotorMode(byte _mode){ 
}

void RightMotorSpeed(byte _throttle){
  Drive_Motors(-_throttle);
}

void LeftMotorSpeed(byte _throttle){
  Turn_Motors(-_throttle);
}

// ================== GoTo Set Distance ===================

long g_last_GoTo_millis=0;
float g_iGoTo=0;
float g_last_dGoTo=0;
float g_last_pGoTo=0;
float g_pSet=.5;
float g_iSet=0;
float g_dSet=0;
float g_motorSpeed=0;
int g_dwellCounter=0;
boolean g_PIDtakeover = false; 
float g_speedRampCounter=0;

void GoTo(float _Distance, float _speed) {
//, int _turn=0, int _distance=0) { 
  if((millis() - g_last_GoTo_millis) < 50) {
    // Has not been long enough to run this function
    return;
  }
  g_last_GoTo_millis = millis();
  
  float dGoTo;
  float KpGoTo;
  float KiGoTo;
  float KdGoTo;
  
  _Distance = _Distance * 12 * CLICKS_PER_INCH;
  
  float displacement = -(encoder1() + encoder2()) / 2;
    
  // PID
  // P - Error
  float pGoTo = _Distance - displacement;

  // ramp up speed slowly
  if(abs(pGoTo) >= 500){
    g_speedRampCounter += .1;
    
    if(g_motorSpeed < _speed){
      g_motorSpeed += g_speedRampCounter;
    }
  }  
  if(abs(pGoTo) < 500) g_motorSpeed = _speed/1.5;
  if(abs(pGoTo) < 200) g_motorSpeed = _speed/2;
  if(abs(pGoTo) < 100) g_motorSpeed = _speed/5;
  if(abs(pGoTo) < 50) g_motorSpeed = _speed/9;
  if(abs(pGoTo) < 10) g_motorSpeed = _speed/_speed; 
  if(abs(pGoTo) < 5) g_PIDtakeover = true; 
  
  if(g_PIDtakeover){ 
//    Melody(ledPin, 1000, 10);delay(10);
    // I - Accumulated Error
    g_iGoTo += pGoTo;
    // D - Error's rate of change
    dGoTo = pGoTo - g_last_pGoTo;
    
    KpGoTo = pGoTo * g_pSet;
    KiGoTo = g_iGoTo * g_iSet;
    KdGoTo = dGoTo * g_dSet;
  //  float KaGoTo = aGoTo * 0;
    g_motorSpeed = constrain((KpGoTo + KiGoTo + KdGoTo), -5, 5);
    g_last_pGoTo = pGoTo;
  }
  
  if(g_motorSpeed < 1 && g_motorSpeed > 0)g_motorSpeed = 1;
  if(g_motorSpeed > -1 && g_motorSpeed < 0)g_motorSpeed = -1;
  
  Drive_Motors(-g_motorSpeed);
  Turn_Motors(0);  
  
  XBeeSend("ya",(pGoTo));
  XBeeSend("yw",(g_motorSpeed));
  
//  Serial3.print(_Distance);
//  Serial3.print("\t");
//  Serial3.print(displacement);
//  Serial3.print("\t");
//  Serial3.print(pGoTo);
//  Serial3.print("\t");
//  Serial3.print(KpGoTo);
//  Serial3.print("\t");
//  Serial3.print(g_motorSpeed);
//  Serial3.print("\t");
//  Serial3.print(_speed);
//  Serial3.print("\t");
//  Serial3.print(g_speedRampCounter);
//  Serial3.print("\t");
//  Serial3.println(g_PIDtakeover);  
  
  // Are we there yet? Yes? Move on to next Logic Step!
  if (abs(pGoTo) < 1) {    
    g_dwellCounter +=1;
    if(g_dwellCounter > 15){
      Drive_Motors(0);
      Turn_Motors(0);  
      Melody(ledPin, 500, 500);delay(100);
      g_logic_step += 1;// -------------------- NEXT STEP!
      encodeReset();
      Clear_Globals();
//      return;
    }
  }else{
    g_dwellCounter=0;
  }
}

// ================== GoTo given Angle ===================

long g_last_GoToAngle_millis;
float g_iGoToAngle;
float g_last_dGoToAngle;
float g_last_pGoToAngle;
float g_pSetGoToAngle=1;
float g_iSetGoToAngle=0;
float g_dSetGoToAngle=0;
//float g_motorSpeed;
int g_speedRamp=10;
int dwellCounter=0;

char GoToAngle(float _Angle, int _speed) {
//, int _turn=0, int _distance=0) { 
  if((millis() - g_last_GoToAngle_millis) < 20) {
    // Has not been long enough to run this function
    return 0;
  }
  g_last_GoToAngle_millis = millis();
  
  CalcAngle();
//  XBeeSend("ya",thetaH);
  
//  _Angle = _Angle * 12 * CLICKS_PER_INCH;
  
  long displacement = (encoder1() + encoder2()) / 2;
  
  // PID =================
  // P - Error
  float pGoToAngle = _Angle - thetaH;
  // I - Accumulated Error
  g_iGoToAngle += pGoToAngle;
  // D - Error's rate of change
  float dGoToAngle = pGoToAngle - g_last_pGoToAngle;
  
  float KpGoToAngle = pGoToAngle * g_pSetGoToAngle;
  float KiGoToAngle = g_iGoToAngle * g_iSetGoToAngle;
  float KdGoToAngle = dGoToAngle * g_dSetGoToAngle;
//  float KaGoTo = aGoTo * 0;


  // slowly ramp up speed at beginning
  g_speedRampCounter = g_speedRampCounter + 1;
  if(g_speedRampCounter > 10 && g_speedRamp > 1){
    g_speedRamp = g_speedRamp -1;
    g_speedRampCounter = 0;
  }
  
  // ramp down speed at end
  if(pGoToAngle < 30) g_speedRamp = 2;
  if(pGoToAngle < 30) g_speedRamp = 3;
  if(pGoToAngle < 20) g_speedRamp = 5;
  if(pGoToAngle < 10) g_speedRamp = 5;
  
//  g_motorSpeed = constrain(_speed, -_speed/g_speedRamp, _speed/g_speedRamp);
  g_motorSpeed = constrain(KpGoToAngle + KiGoToAngle + KdGoToAngle, -_speed/g_speedRamp, _speed/g_speedRamp);
//  g_motorSpeed = constrain(KpGoToAngle + KiGoToAngle + KdGoToAngle, -_speed, _speed);

  if(g_motorSpeed < 1 && g_motorSpeed > 0)g_motorSpeed = 1;
  if(g_motorSpeed > -1 && g_motorSpeed < 0)g_motorSpeed = -1;

  g_last_pGoToAngle = pGoToAngle;
  
  Drive_Motors(0);
  Turn_Motors(g_motorSpeed);
    
//  Serial3.print(pGoToAngle);
//  Serial3.print("\t");
//  Serial3.print(displacement);
//  Serial3.print("\t");
//  Serial3.print(KpGoToAngle);
//  Serial3.print("\t");
//  Serial3.println(g_motorSpeed);
  
//  Serial.print(millis());
//  Serial.print("\t");
//  Serial.print(_Distance);
//  Serial.print("\t");
//  Serial.print(displacement);
//  Serial.print("\t");
//  Serial.print(KpGoTo);
//  Serial.print("\t");
//  Serial.print(KiGoTo);
//  Serial.print("\t");
//  Serial.print(KdGoTo);
//  Serial.print("\t");
//  Serial.print(thetaH);
//  Serial.print("\t");
//  Serial.println(g_motorSpeed);
  
  // Are we there yet? Yes? Move on to next Logic Step!
  if (abs(pGoToAngle) < .2) {   
    g_dwellCounter +=1;
    if(g_dwellCounter > 20){ 
      g_logic_step += 1;
      encodeReset();
      Clear_Globals();
    }
  }
}

// ========================== Clear_Globals ==========================
void Clear_Globals() {
  g_iGoToAngle=0;
  g_last_dGoToAngle=0;
  g_last_pGoToAngle=0;
  g_iGoTo=0;
  g_last_dGoTo=0;
  g_last_pGoTo=0;
  g_speedRampCounter=0;
  g_speedRamp=10;
  g_dwellCounter=0;
  g_PIDtakeover = false;
}

// ================== display battery volts as a single byte ==================
float volts(){  
}

// =================== display battery amps as a single byte ==============
float motor1_amps(){   
}

// ==================== display battery amps as a single byte ===============
float motor2_amps(){         
}


// ================== encoder reset ==================
void encodeReset(){        
}


// ========================== Turn_Motor1 ======================
//void Turn_Motor1(int iTurn) {
//  Wire.beginTransmission(md25Address);                    
//  Wire.write((uint8_t)speed1);
//  Wire.write(iTurn);                                           
//  Wire.endTransmission();
//}

// ========================== Right Encoder ==========================
long encoder1(){                                                // returns value of encoder 1 position as a long
}

// ========================== Left Encoder ==========================
long encoder2(){                                            // Returns encoder 2 position as a long
}


