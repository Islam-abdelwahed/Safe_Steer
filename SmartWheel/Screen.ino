
int adc1, adc2, adc3, er = 5;
unsigned long time1 = 0;

void Screen_Init() {
  endCommand();
  Serial.print("page 0");
  endCommand();
}

void Refresh_Screen(int rpm, int battery, int speed) {
  writeRpm(rpm);
  writeBattery(battery);
  writeSpeed(speed);
  writeAlcohol(AlcoholDetected);
  // writeMood();
}

void Home_Screen() {
  endCommand();
  Serial.print("page 1");
  endCommand();
}


void endCommand() {
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~Write on screen~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void writeSpeed(int num) {
  endCommand();
  Serial.print("Speed.val=");
  Serial.print(num);
  endCommand();
}

void writeAlcohol(bool State) {
  endCommand();
  // Serial.print("page 1");
  // endCommand();
  if (State) {
    Serial.printf("Sober.pic=%d", 12);
    endCommand();
  } else {
    Serial.printf("Sober.pic=%d", 13);
    endCommand();
  }
}

void writeTouch(bool State) {
  endCommand();
  // Serial.print("page 1");
  // endCommand();
  if (State) {
    Serial.printf("Hold.pic=%d", 10);
    endCommand();
  } else {
    Serial.printf("Hold.pic=%d", 11);
    endCommand();
  }
}

void writeBattery(int num) {
    Serial.print("Battery.val=");
    Serial.print(num);
    endCommand();

    Serial.print("j0.val=");
    Serial.print(num);
    endCommand();
}

void writePulse(int hr) {
  Serial.print("I'm inside writePulse() [OK]:");
  Serial.println(hr);
  endCommand();
  Serial.print("Pulse.val=");
  Serial.print(hr);
  endCommand();

  if (hr < 70) {
    Serial.printf("p1.pic=%d", 11);
    endCommand();
  } else {
    Serial.printf("p1.pic=%d", 10);
    endCommand();
  }
}

void writeRpm(int num) {
    Serial.print("va2.val=");
    Serial.print(num);
    endCommand();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TEST MSG~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Print_Test_MSG(String msg, char img) {
  endCommand();
  Serial.print("page 3");
  endCommand();
  Serial.printf("t0.txt=\"%s\"", msg.c_str());
  endCommand();
  Serial.printf("p0.pic=%d", img);
  endCommand();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//