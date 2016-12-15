// ====================== Cube Quest ==============================

//UpDownStick: center:508, min:0, max:1022
//LeftRIghtStick: center:511, min:0, max: 1023
//zero-zero is Left-Down

void CubeQuest() {
  static int cubeQuestTimer_old;
  static long s_speed;
  
  if((millis()-cubeQuestTimer_old) < 100) {return;}
  cubeQuestTimer_old = millis();
  
  int UpDownStick = analogRead(A0);
  int LeftRightStick = analogRead(A1);

  if (UpDownStick > 600) {//Stick forward
//    float tempForwardSpeed = map(UpDownStick, 509, 1022, 0, 5000);
    s_speed = s_speed + 500;
    //delay(2);
    writeLCDLine("                ",1,0);
    writeLCDLine(String(s_speed)+" : "+String(UpDownStick),1,0);
    XBeeSend("vl",s_speed);
  }  else if (UpDownStick < 400) {
//    float tempForwardSpeed = map(UpDownStick, 507, 0, 0, -5000);
    s_speed = s_speed - 500;
    //delay(2);
    writeLCDLine("                ",1,0);
    writeLCDLine(String(s_speed)+" : "+String(UpDownStick),1,0);
    XBeeSend("vl",s_speed);
  }

  if (LeftRightStick > 550) {
    //XBeeSend("tr",30);
//    writeLCDLine("RT",2,12);
  } else if(LeftRightStick < 490) {
    //XBeeSend("tr",-30);
//    writeLCDLine("LF",2,12);
  } else {
    //XBeeSend("tr",0);
//    writeLCDLine(" 0",2,12);
  }
}
