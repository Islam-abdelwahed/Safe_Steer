

//=== Alcohol Sensor Initialization ===//
void AlcoholSensor_Init() {

  //serial.println("Initializing Alcohol Sensor...");

  pinMode(AlcoholSensorPin, INPUT);  // Set the sensor pin as an input
  //serial.println("Alcohol Sensor Initialization success. [OK]");
}


//=== Check Alcohol State ===//
bool CheckAlcoholState() {
  int alcoholDetected = digitalRead(AlcoholSensorPin);  //Read Value OF The Sensor Pin

  if (alcoholDetected == LOW) {
    //serial.println("Alcohol detected!, You Can't Drive!!");
  }
  else {
    //serial.println("No alcohol detected.");
  }
  // FireBase_SendData(alcoholDetected,"/Alcohol");
  // delay(1000);  // Delay for 1 second
  //delay(300);
  return !alcoholDetected;
}
