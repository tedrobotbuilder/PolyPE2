//// Adafruit_BNO055 IMU
//
//// How to call for heading:
////sensors_event_t event;
////bno.getEvent(&event);
////Serial.print("X: ");
////Serial.print(event.orientation.x, 4);
//
//void imuSendCalToRemote() {
//  String imuHeaderAndStatus="CL";
//  
//  imuHeaderAndStatus.concat( CalStatus() );
//  
//  XBeeSend(imuHeaderAndStatus, 0 );
//  
//  delay(50);
//}
//
//float imuHeading(void) {
//  sensors_event_t imuEvent;
//  imu55.getEvent(&imuEvent);
//  return imuEvent.orientation.x;
//}
//  
//String CalStatus(void) {
//  String calStat="";
//  
//  uint8_t system, gyro, accel, mag;
//  system = gyro = accel = mag = 0;
//  imu55.getCalibration(&system, &gyro, &accel, &mag);
//  
//  calStat.concat("S:");
//  calStat.concat(system);
//  calStat.concat("_G:");
//  calStat.concat(gyro);
//  calStat.concat("_A:");
//  calStat.concat(accel);
//  calStat.concat("_M:");
//  calStat.concat(mag);
////  calStat.concat("....");
//
//  return calStat;
//}
//
//
//void displayCalStatus(void) {
//  /* Get the four calibration values (0..3) */
//  /* Any sensor data reporting 0 should be ignored, */
//  /* 3 means 'fully calibrated" */
//  uint8_t system, gyro, accel, mag;
//  system = gyro = accel = mag = 0;
//  imu55.getCalibration(&system, &gyro, &accel, &mag);
//
//  /* The data should be ignored until the system calibration is > 0 */
//  Serial.print("\t");
//  if (!system) {
//    Serial.print("! ");
//  }
//
//  /* Display the individual values */
//  Serial.print("Sys:");
//  Serial.print(system, DEC);
//  Serial.print(" G:");
//  Serial.print(gyro, DEC);
//  Serial.print(" A:");
//  Serial.print(accel, DEC);
//  Serial.print(" M:");
//  Serial.print(mag, DEC);
//}
