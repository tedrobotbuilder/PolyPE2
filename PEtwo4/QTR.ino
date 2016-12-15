// QTR Line Sensors

// Example reading of sensors:
//  unsigned int position = qtrLine.readLine(lineSensorValues);
//  if(lineSensorValues[Pin#] > 200){
//    
//  unsigned int edgePosition = qtrEdge.readLine(edgeSensorValues);
//  if(edgeSensorValues[Pin#] > 200){
  
void QTREdgeCalTest() {//==================================================================
  QTREdgeCal();
  
  LookForEdgeTest();
}

void QTREdgeCal(){//==================================================================  
  int i;
  int calSpeedQTR = 400;
  int calTime = 25;
  
  //digitalWrite(EMITTER_PIN, HIGH);
  
  if(Bot.QTREdgeIsCal == true){
    return;
  } 

  for (i = 0; i < calTime; i++){
    qtrEdge.calibrate();       // reads all sensors at 2500 us per read (i.e. ~25 ms per call)
    // half turn left
    RoboDrive(-calSpeedQTR,-calSpeedQTR);
  }
  
  RoboDrive(0, 0);
  delay(200);
  RoboDuty(0, 0);
  delay(100);
  
  terminal.print("calibratedMinimumOn[RIGHTEDGE]:");
  terminal.println(qtrEdge.calibratedMinimumOn[RIGHTEDGE]);
  terminal.print("calibratedMinimumOn[LEFTEDGE]:");
  terminal.println(qtrEdge.calibratedMinimumOn[LEFTEDGE]);
  terminal.println(" ");
  terminal.print("calibratedMaximumOn[RIGHTEDGE]:");
  terminal.println(qtrEdge.calibratedMaximumOn[RIGHTEDGE]);
  terminal.print("calibratedMaximumOn[LEFTEDGE]:");
  terminal.println(qtrEdge.calibratedMaximumOn[LEFTEDGE]);
  
  // Cal-Is-Done-flag set to true:
  Bot.QTREdgeIsCal = true;
}

void QTRCal() {//==============================================================================
  int i;
  int calSpeedQTR = 2100;
  int calTime = 25;
  
  digitalWrite(EMITTER_PIN, HIGH);
    
  for (i = 0; i < calTime/2; i++){
    qtrLine.calibrate();       // reads all sensors at 2500 us per read (i.e. ~25 ms per call)
    // half turn left
    RoboDrive(-calSpeedQTR,calSpeedQTR);
  }
  
  for (i = 0; i < calTime; i++){
    qtrLine.calibrate();  
    // full turn right  
    RoboDrive(calSpeedQTR,-calSpeedQTR);
  }
  
  for (i = 0; i < calTime; i++){
    qtrLine.calibrate();         
    // full turn left  
    RoboDrive(-calSpeedQTR,calSpeedQTR);
  }

  for (i = 0; i < calTime/2; i++){
    qtrLine.calibrate();      
    // half turn right  
    RoboDrive(calSpeedQTR,-calSpeedQTR);
  }
  
  RoboDrive(0, 0);
  delay(200);
  RoboDuty(0, 0);
  delay(100);

  // print the calibration minimum values measured when emitters were on

  for (i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtrLine.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (i = 0; i < NUM_SENSORS; i++) {
    Serial.print(qtrLine.calibratedMaximumOn[i]);
    Serial.print(' ');
    if(i==7)Serial.println(" ");
    if(i==15)Serial.println(" ");
    if(i==23)Serial.println(" ");    
  }
  Serial.println();
  Serial.println();
  
  Melody(ledPin, 4000, 200);
  //delay(1000);
}

// ============================== print uncalibrated the sensor values ===================================
// prints uncalibrated values from sensors
void PrintQTR() {
  static long g_printQTRtime;
  
  if((millis() - g_printQTRtime) < 200)return;
  g_printQTRtime = millis();
  
  //unsigned int position = 
  qtrLine.read(lineSensorValues);
  
  unsigned char i;
  for (i = 0; i < NUM_SENSORS; i++) {
    Serial.print(lineSensorValues[i] * 10 / 1001);
    //Serial.print(lineSensorValues[i]);
    Serial.print(' ');
//    if (i==3)
//      Serial.print('- ');
  }
  Serial.println("    ");
  //Serial.println(position);
}

// ============================== print calibrated the sensor values ===================================
void PrintCalQTR() {
  unsigned int position = qtrLine.readLine(lineSensorValues);
  
  unsigned char i;
  for (i = 0; i < NUM_SENSORS; i++) {
    Serial.print(lineSensorValues[i] * 10 / 1001);
    Serial.print(' ');
//    if (i==15)
//      Serial.print('-');
  }
  
  Serial.print("    ");
  Serial.println(position);
}
