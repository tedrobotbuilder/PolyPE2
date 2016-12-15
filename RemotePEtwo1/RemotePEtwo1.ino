// Remote

#include <SoftwareSerial.h>
SoftwareSerial XBee(8, 9); // RX, TX digital pins

// ================== Constants ===================
#define START      7  // Start/Stop Button
#define LEFT       6  // Left Button
#define UP         5  // Up Button
#define DOWN       4  // Down Button
#define RIGHT      3  // Right Button
#define STOP       2  // Select Button

#define RIGHTADJ  20  // Use Right Button for variable adjustment
#define LEFTADJ   21  // Use Left Button for variable adjustment

#define FALSE      0  // Before I became comforatable with Arduino's boolean I made my own, havent changed it yet
#define TRUE       1

#define BATTBOT         10 // battery voltage for robot mode
#define BATTREM         11 // battery voltage for remote mode
#define LINEFOLLOW      12 // Line follow mode
#define CUBEQUEST       13 // Cubequest mode
#define DEADREC         14 // DEADREC
#define PLAYLINEBACK    15 // follow recored line data
#define BEACONKILLER    16 // Beacon Killer
#define BEACONKILLERWO  17 // Beacon Killer with obsticals
#define BULLDOZER       18 // Bull Dozer
#define IMUCAL          19 // Show calibration state on remote

// ================== Globals ===================
char ESC = 0xFE; 
boolean g_robotStarted = false;
int g_mode=11; 
int g_modeToBot=11;
boolean g_modeSelected = false;
boolean g_PcDataComplete = false;  // is the string is complete?
float g_yaw=0;
int ButtonPressed=FALSE;
int ButtonPressed_old=FALSE;
int clearButtonTimer = 1000;
long clearButtonTimer_old=0;
int modeLineFollow=1;
int modeBEACONKILLER=1;
int varAdjustment=0;

// ================== setup ===================
void setup() { 
  Serial.begin(9600); // LCD  
  delay(10);

  XBee.begin(9600); // XBee
  delay(10);

  // listen(), Only really needed if there is more then one soft serial device
  // only one can listen at a time
  XBee.listen(); 
  delay(10);

  initLCD();
  delay(10);

  writeLCDLine("Remote Ready",1,0);
  delay(10);

  char tempChar[10];
  dtostrf(BattVolts(),2,1,tempChar);// convert float to string
  writeLCDLine(tempChar,2,0);
  delay(10);

  tone(13, 1000, 1);
  delay(10);

  //  OnModeChange();
}


// ================== Main Loop ===================
void loop() {
  BattCheck();
  Buttons(); // Handle Buttons
  ModeWork(); // Handle mode work

    // Load incoming data
  XBeeEvent();
}

long BattCheckTimer_old;

void BattCheck(){  // Warn with tone if batt bellow 8.8V ==========
  if(((millis()-BattCheckTimer_old) < 5000) || (g_mode != BATTREM))return;
  BattCheckTimer_old = millis();

  // Update LCD with current batt pack voltage
  char tempChar[10];
  dtostrf(BattVolts(),2,1,tempChar);// convert float to string
  writeLCDLine(tempChar,2,0);
  delay(10);

  // check if total batt pack voltage below 8.8v
  if(BattVolts() < 8.8 && BattVolts() > 5){
    tone(13, 500, 2000);
    //delay(20);
  }
}

float BattVolts(){ // Returns battery pack's total voltage =======
  float Vout = analogRead(A2);//read Vout from votage divider
  Vout = 5*(Vout/1024);//convert 0-1024 reading to volts
  float Vin = Vout*(9.8+14.75)/9.8; //find Vin given Vout

  writeLCDLine("volts",2,15);

  return Vin;
}


// ============= Handle Buttons ====================================================


void Buttons() {// Read and respond to button presses
  // Which button was pressed?
  if (digitalRead(START) == LOW && (!g_robotStarted)) {
    ButtonPressed = START; 
    //    writeLCDLine("S",2,15);
  } 
  else if ((digitalRead(STOP) == LOW) && (g_robotStarted)) {
    ButtonPressed = STOP;  
    //    writeLCDLine("T",2,15);
  } 
  else if ((digitalRead(RIGHT) == LOW) && (!g_robotStarted)) {
    ButtonPressed = RIGHT;  
    //    writeLCDLine("R",2,15);
  }  
  else if ((digitalRead(RIGHT) == LOW) && (g_robotStarted)) {
    ButtonPressed = RIGHTADJ;  
    //    writeLCDLine("RA",2,15);
  } 
  else if ((digitalRead(LEFT) == LOW) && (!g_robotStarted)) {
    ButtonPressed = LEFT;
    //    writeLCDLine("L",2,15);
  } 
  else if ((digitalRead(LEFT) == LOW) && (g_robotStarted)) {
    ButtonPressed = LEFTADJ;
    //    writeLCDLine("LA",2,15);
  } 
  else if ((digitalRead(DOWN) == LOW) && (!g_robotStarted)) {
    ButtonPressed = DOWN;
    //    writeLCDLine("D",2,15);
  } 
  else if ((digitalRead(UP) == LOW) && (!g_robotStarted)) {
    ButtonPressed = UP;
    //    writeLCDLine("U",2,15);
  } 
  else {    
    ButtonPressed=FALSE;
  }

  // Debounce
  // If button pressed is not the same and too little time has passed it could be a bounce
  if (((millis()-clearButtonTimer_old) < 50) && (ButtonPressed != ButtonPressed_old)){
    return;
  }
  clearButtonTimer_old = millis();

  //  Serial.print(ButtonPressed);
  //    Serial.print(" : ");   
  //  Serial.println(ButtonPressed_old);

  // Start
  // Only act on BUTTON START event by looking at the last button pressed
  if (ButtonPressed==START 
    && (g_mode==LINEFOLLOW || g_mode==CUBEQUEST || g_mode==DEADREC 
    || g_mode==BEACONKILLER || g_mode==BULLDOZER || g_mode==PLAYLINEBACK 
    || g_mode==BEACONKILLERWO || g_mode==IMUCAL) ) {
    // START button released: Do StartStop
    XBeeSend("ss",1);// send start to Robot
  }

  // Stop
  // Only act on BUTTON STOP event by looking at the last button pressed
  if (ButtonPressed==STOP 
    && (g_mode==LINEFOLLOW || g_mode==CUBEQUEST || g_mode==DEADREC 
    || g_mode==BEACONKILLER || g_mode==BULLDOZER || g_mode==PLAYLINEBACK 
    || g_mode==BEACONKILLERWO || g_mode==IMUCAL) ) {
    // STOP button released: Do Stop
    XBeeSend("ss",0);// send stop to Robot
  } 

  // UP - LINEFOLLOW
  // Linefollow mode has 6 sub-modes
  // Only act on BUTTON UP event by looking at the last button pressed
  if (ButtonPressed==FALSE && ButtonPressed_old==UP && (g_mode==LINEFOLLOW) ) {
    // DOWN button released: Do UP
    modeLineFollow--;
    if(modeLineFollow < 1) modeLineFollow=1;
    XBeeSend("ml",modeLineFollow);
    //writeLCDLine(String(modeLineFollow),2,0);
  } 

  // DOWN - LINEFOLLOW
  // Only act on BUTTON DOWN event by looking at the last button pressed
  if (ButtonPressed==FALSE && ButtonPressed_old==DOWN && (g_mode==LINEFOLLOW) ) {
    // DOWN button released: Do DOWN
    modeLineFollow++;
    if(modeLineFollow > 6) modeLineFollow=6;
    XBeeSend("ml",modeLineFollow);
    //writeLCDLine(String(modeLineFollow),2,0);
  } 

  // UP - BEACONKILLER
  // Beaconkiller has 8 sub-modes
  // Only act on BUTTON UP event by looking at the last button pressed
  if (ButtonPressed==FALSE && ButtonPressed_old==UP && (g_mode==BEACONKILLER || g_mode==BEACONKILLERWO) ) {
    // DOWN button released: Do UP
    modeBEACONKILLER--;
    if(modeBEACONKILLER < 1) modeBEACONKILLER=8;
    XBeeSend("mb",modeBEACONKILLER);
    writeLCDLine(String(modeBEACONKILLER),2,0);
  } 

  // DOWN - BEACONKILLER
  // Only act on BUTTON DOWN event by looking at the last button pressed
  if (ButtonPressed==FALSE && ButtonPressed_old==DOWN && (g_mode==BEACONKILLER || g_mode==BEACONKILLERWO) ) {
    // DOWN button released: Do DOWN
    modeBEACONKILLER++;
    if(modeBEACONKILLER > 8) modeBEACONKILLER=1;
    XBeeSend("mb",modeBEACONKILLER);
    writeLCDLine(String(modeBEACONKILLER),2,0);
  } 

  // RIGHT - Change bot polyathlon event mode
  if (ButtonPressed==FALSE && ButtonPressed_old==RIGHT) {
    // RIGHT button released: Do RIGHT
    g_modeToBot = g_modeToBot + 1; // change mode up
    if ( g_modeToBot == 20 ) g_modeToBot = 10;
    g_modeSelected = false;// ??? why do this here?
    XBeeSend("mo",g_modeToBot);
    //    Serial.println("right hit");
  }

  // RIGHTADJ - adjust variable up for tuning
  if (ButtonPressed==FALSE && ButtonPressed_old==RIGHTADJ) {
    // RIGHT button released while runing: Do RIGHTADJ
    varAdjustment = varAdjustment +1;
    XBeeSend("va",varAdjustment);
  }

  // LEFT - Change bot polyathlon event mode
  if (ButtonPressed==FALSE && ButtonPressed_old==LEFT) {
    // LEFT button released: Do LEFT
    g_modeToBot = g_modeToBot - 1; // change mode down
    if ( g_modeToBot == 9 ) g_modeToBot = 19;
    g_modeSelected = false;// ??? why do this here?
    XBeeSend("mo",g_modeToBot);
  }

  // LEFTADJ - Adjust variable down for tuning
  if (ButtonPressed==FALSE && ButtonPressed_old==LEFTADJ) {
    // LEFT button released while runing: Do LEFTADJ
    varAdjustment = varAdjustment -1;
    XBeeSend("va",varAdjustment);
  }

  //writeLCDLine((String)g_modeToBot,2,8);
  ButtonPressed_old = ButtonPressed;
}    

// ===================== Mode Work ===========================
// Runs sub for the selected mode (only Cubequest is needed for now)
// Runs every loop!
void ModeWork() {
  //  if (g_robotStarted==TRUE && g_mode==LINEFOLLOW) {
  //    BasicLineFollow();
  //  }
  if (g_robotStarted==TRUE && g_mode==CUBEQUEST) {
    CubeQuest();
  }
//  if (g_mode == BATTBOT) {
//    XBeeSend("bb",g_modeToBot);
//    delay(1000);
//  }
  //  if (g_robotStarted==TRUE && g_mode==DEADREC) {
  //    DEADREC();
  //  }  
}

// ============= Handle Mode Change =======================
//void OnModeChange() {
//  if (g_modeToBot == BATTBOT) {
//    XBeeSend("mo",g_mode);// Set Robot to mode = BATTBOT
//  }
//  if (g_modeToBot == BATTREM) { // display remote's battery voltage
//    XBeeSend("mo",g_modeToBot);// Set Robot to mode = BATTREM
//  }
//  if (g_modeToBot == LINEFOLLOW) {
//    XBeeSend("mo",g_modeToBot);// Set Robot to mode = LINEFOLLOW
//  }
//  if (g_modeToBot == CUBEQUEST) {
//    XBeeSend("mo",g_modeToBot);// Set Robot to mode = CUBEQUEST
//  }
//  if (g_modeToBot == XBeeTest) {
//    XBeeSend("mo",g_modeToBot);// Set Robot to mode = CUBEQUEST
//  }
//}

// ======================= Reset ===========================
// Robot sent reset request after it either turned on or reset
void Reset(){ 
  g_robotStarted = false;
  g_mode = 11; 
  g_modeToBot = 11;
  g_modeSelected = false;
  XBeeSend("mo",g_modeToBot);
  delay(20);
  writeLCDLine("Bot Ready",1,0);
  delay(20);
  //  OnModeChange();
}

