// Polyathlon Robot

//#include <PID_v1.h> // autotune PID
//#include <PID_AutoTune_v0.h> // autotune PID
#include <SPI.h> // Pixy
#include <Pixy.h> // Pixy
#include "BMSerial.h" // RoboClaw
#include "RoboClaw.h" // RoboClaw
//#include <Wire.h> // I2C: IMU and old motor driver
#include <QTRSensors.h> // Pololu QTR sensors
//#include <Adafruit_Sensor.h> // IMU
//#include <Adafruit_BNO055.h> // IMU
//#include <utility/imumaths.h> // IMU

// ================== Constants ====================
#define ledPin         13
#define BATTBOT        10 // battery voltage for robot [mode]
#define BATTREM        11 // battery voltage for remote [mode]
#define LINEFOLLOW     12 // Line follow [mode]
#define CUBEQUEST      13 // Cube quest [mode]
#define DEADREC        14 // DEADREC [mode]
#define PLAYLINEBACK   15 // follow recored line data [mode]
#define BEACONKILLER   16 // Beacon Killer [mode]
#define BEACONKILLERWO 17 // Beacon Killer with obsticals [mode]
#define BULLDOZER      18 // [mode]
#define IMUCAL         19 // Show calibration state on remote
#define LEFTEDGE       1
#define RIGHTEDGE      0 
#define LEFTLINE       23 
#define RIGHTLINE      53 
#define address        0x80      //Roboclaw Address

#define CLICKS_PER_INCH 151.85 //29.47

// IMU
//Adafruit_BNO055 imu55 = Adafruit_BNO055(55);

//Setup communcaitions with roboclaw motor controler
BMSerial terminal(0,1); // serial print replacement
RoboClaw roboclaw(19,18,10000);// RX, TX, Timeout (ms)

// This is the main Pixy cam object 
Pixy pixy;

// ================== C class for this robot ===========
class PERobot{
 public:
 unsigned int maxMotorSpeed=1000;
 boolean BattReadingGood=false;
 boolean leftEdgeSeen=false;
 boolean rightEdgeSeen=false;
 boolean EdgeSeen=false;
 boolean QTREdgeIsCal=false;

 PERobot(){ //the constructor
   //i=0;
 }
};

PERobot Bot = PERobot();// instance is Bot

// ================== QTR Line =========================
#define NUM_SENSORS 24 // number of sensors used
#define TIMEOUT 2500   // waits for max 2500 micro sec for sensor outputs to go low
#define EMITTER_PIN 8  // pin used to switch sensors on or off

QTRSensorsRC qtrLine(// setup digital line sensors
(unsigned char[]){
  46,44,42,40,38,36,34,32, 22,23,24,25,26,27,28,29, 47,45,43,41,39,37,35,33
}
  //22,23,24,25,26,27,28,29} center
  //47,45,43,41,39,37,35,33} right
  //46,44,42,40,38,36,34,32} left
,
NUM_SENSORS, TIMEOUT, EMITTER_PIN
);

unsigned int lineSensorValues[NUM_SENSORS];

// ================== QTR Edge =========================
QTRSensorsAnalog qtrEdge(// setup analog edge sensors
(unsigned char[]){
  0,1},// sensors on analog pins 0 and 1
2, // # sensors
15, // # readings to avg
QTR_NO_EMITTER_PIN // no on/off pin
);

unsigned int edgeSensorValues[2];

// ================== Globals ===================
// Line follow globals
float g_MaxFollowSpeed = 12000;//10200
float kp= 2.8;//2.8
float ki= .02;//.14
float kd= .5;

/* odometer maintains these global variables: */
float thetaH;          /* bot heading */
float X_pos;          /* bot X position in inches */
float Y_pos;          /* bot Y position in inches */
boolean PcDataComplete = false;  // whether the string is complete

float g_adjustVar=0; // number sent from remote to adjust variables on the fly

int g_mode=0;
int g_modeLineFollow=1;//sub-mode for Linefollowing, change parameters

float g_speed=0;

float g_heading;
long g_startTime;
int g_wiiAngle = 4000;
long g_printTime;
boolean g_motorModeSet = false;

boolean g_seeObLeft = false;
boolean g_seeObRight = false;
boolean g_seePipe = false;

boolean g_StopBullDozer = false;
boolean g_endOfTable = false;
boolean g_isPushing = false;

long g_mem_millis;
String pushPipeStep;
float g_total_inches=0;
float g_last_total_inches=0;

float target_distance;         	/* distance in inches from position */
float heading_error;            /* heading error in degrees */
float target_bearing; 

// Robot's Start/Stop state holder
boolean g_Start = false;

//int freeRam(){
//  extern int __heap_start, *__brkval; 
//  int v; 
//  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
//}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// ================== Setup =====================================================================
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void setup() { 
  delay(100); //wait for everythign to power up 

  //Serial.begin(115200); // USB and Bluefruit, was just USB 
  
  terminal.begin(9600);
  delay(50);  
  
  Init_Motors();
  
//  Serial.print( "Start ReturnLeftEnc: " );
//  Serial.println( ReturnLeftEnc() );
//  Serial.print( "ReturnRightEnc: " );
//  Serial.println( ReturnRightEnc() );
//  Serial.print( "ReturnAvgEnc: " );
//  Serial.println( ReturnAvgEnc() );

//  Wire.begin();    //Init the I2C    
//  delay(500);  

//  if(!imu55.begin()) {
//    /* There was a problem detecting the BNO055 ... check your connections */
//    Serial.print("BNO055 IMU Failed to Start");
//    while(1);
//  }
    
//  imu55.setExtCrystalUse(true);
  
  pixy.init();
 
  // QTR Setup
  pinMode(EMITTER_PIN, OUTPUT);// turns QTR line sensors on and off
//  qtrLine.emittersOff();
  digitalWrite(EMITTER_PIN, LOW);
  
  pinMode(ledPin, OUTPUT); // Speaker and LED Pin 13

//  Serial2.begin(9600); // Wii CAM  
//  delay(50);
  //Serial3.begin(115200); // Roboclaw, was Bluetooth
//  delay(50);  
//  Serial1.begin(9600); // not used yet, was XBee
//  delay(50);  
  Serial2.begin(9600); // XBee, was not used before on PE1
  delay(50);  

  terminal.println("Begin Setup");   
  delay(50);

  terminal.print( "Battery: " );  //show batt volts
  float battReading;
  battReading = ReadBattVolts();
  terminal.println(battReading);
  
  if( (battReading < 10.5) && (battReading > 6) ){ //bellow 6 means we are on USB power
    terminal.println( "Battery Low!" );  //show batt volts USB
//    Melody(ledPin, 500, 2000);
  } 

  RequestMode(); //ask remote for current mode

  terminal.println("End Setup"); 
//  delay(50);  

  Melody(ledPin, 1000, 10);
}

// #####################  Main Loop  ############################################
// #####################  Main Loop  ############################################
// #####################  Main Loop  ############################################
// #####################  Main Loop  ############################################

void loop() {
  static boolean s_hasBeenStarted = false;
  static int32_t s_batCheckMillis=0; 
  
  if((millis() - s_batCheckMillis) > 240000) Melody(ledPin, 500, 1);//4 min alarm
  s_batCheckMillis = millis();

//  static float xtempx = .01;
//  xtempx++;
//  terminal.print(xtempx);
//  terminal.println(",");
//  delay(50);
    
  if(g_Start){
    s_hasBeenStarted = true;

    if(g_mode==LINEFOLLOW) DoLineFollowSetup();
    if(g_mode==DEADREC) DeadReconing();
    if(g_mode==PLAYLINEBACK) PixyTrackingTest();//LookForEdgeTest();//PIDsetup();
    if(g_mode==BEACONKILLER) PixyLookWO(true, false);
    if(g_mode==BEACONKILLERWO) PixyLookWO(true, true);// WO - with obstacles
//    if(g_mode==BEACONKILLERWO) PrintQTR();
//    if(g_mode==IMUCAL) imuSendCalToRemote();// show IMU cal numbers on remote
    if(g_mode==BULLDOZER) BullDozerTwo(true);
    if(g_mode==CUBEQUEST); //--This is done in TwoWaySerial: "tr"
    
  } else if (s_hasBeenStarted) {//stopped and has been started before
    RoboDrive(0,0);
    delay(500);
    RoboDuty(0,0);
    s_hasBeenStarted=false;// kill this loop
    terminal.println("Stopped");
    CheckBotBattery();
  }
}    

// #####################  CheckBotBattery  ############################################
// #####################  CheckBotBattery  ############################################
// #####################  CheckBotBattery  ############################################

void CheckBotBattery() {//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  float battReading=0;
  battReading = ReadBattVolts();
  
  if( (battReading < 10.3) && (battReading > 6) ){
    terminal.print( battReading ); 
//    g_Start = false;
    terminal.println( " Battery Low!" );  //show batt volts USB
    Melody(ledPin, 500, 1);
  } 
}
  
// #####################  HandelStartButton  ############################################
// #####################  HandelStartButton  ############################################
// #####################  HandelStartButton  ############################################
// ================== Toggle Start/Stop Flag ===================
// Called by TwoWaySerial code, still in use!

void HandelStartButton() {//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
  //  Serial.print(g_Start);
  //  Serial.print(":");

  g_Start = 1; // Start = Yes
  Melody(ledPin, 1000, 1);
  delay(1);
  XBeeSend("ss",1);//Tell remote start
}

// #####################  HandelStopButton  ############################################
// #####################  HandelStopButton  ############################################
// #####################  HandelStopButton  ############################################
void HandelStopButton() { 
  g_Start = 0; // Start = No
  Melody(ledPin, 200, 1);
  delay(1);
  XBeeSend("ss",0);//Tell remote stop

  //  Serial.println(g_Start);
}

// #####################  BotReset  ############################################
// =================== Reset if Remote requests it =====================
// Not in use yet

//void BotReset(){
//  asm volatile ("  jmp 0");
//}


