#include <Wire.h>
#include <MAX30102_BeatDetector.h>
#include <MAX30102_Filters.h>
#include <MAX30102_PulseOximeter.h>
#include <MAX30102_Registers.h>
#include <MAX30102_SpO2Calculator.h>
#include <VEGA_MAX30102.h>

#define Num_of_Readings 10
// PulseOximeter Object
PulseOximeter pox;

// Shared variables (access with caution using mutexes or semaphores)
int heartRate = 0;
int Readings[Num_of_Readings];  // Array to store 10 heart rate readings
static int ReadingsCnt = 0;
int readingcpy = 0;
//variables
static char HEA_AlertDialog = 0;
static char HOL_AlertDialog = 0;
static char SPE_AlertDialog = 0;
// Mutex to protect shared variables (optional)
SemaphoreHandle_t mutexHeartSensor = xSemaphoreCreateMutex();  // Create mutex

bool TestHeartSensor() {

  pox.update();

  unsigned long irValue = pox.getHeartRate();
  if (irValue < 70) {
    //serial.println("no finger!");
    return false;

  } else {
    //serial.println("finger found!");
    return true;
  }
}

void onBeatDetected() {
  Serial.println("Beat!");
}

//=== Heart Sensor Initialization ===//
void HeartSensor_Init() {
  //serial.println("Initializing Heart Sensor...");

  if (!pox.begin()) {
    //serial.println("MAX30102 was not found. Please check wiring/power. [ERR]");
    while (1)
      ;  // Halt the program
  } else {
    //serial.println("Heart Sensor Initialization success. [OK]");
  }

  pox.setIRLedCurrent(MAX30102_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

//=== Collect HeartSensor Readings ===//
void CheckHeartSensor() {
  // //serial.println("From CheckHeartSensor()");
  pox.update();

  // Access shared variables with mutex (if enabled)
  if (mutexHeartSensor != NULL) {
    xSemaphoreTake(mutexHeartSensor, portMAX_DELAY);  // Acquire mutex
  }

  Readings[ReadingsCnt++] = pox.getHeartRate();

  // Release mutex after access (if enabled)
  if (mutexHeartSensor != NULL) {
    xSemaphoreGive(mutexHeartSensor);  // Release mutex
  }

  if (ReadingsCnt == Num_of_Readings) {

    // Sort the heart rate readings
    std::sort(Readings, Readings + 10);  // Assuming your library has std::sort

    // Access the median value
    // Access shared variables with mutex (if enabled)
    if (mutexHeartSensor != NULL) {
      xSemaphoreTake(mutexHeartSensor, portMAX_DELAY);  // Acquire mutex
    }
    heartRate = Readings[((Num_of_Readings - 1) / 2)];
    readingcpy = heartRate;

    // Release mutex after access (if enabled)
    if (mutexHeartSensor != NULL) {
      xSemaphoreGive(mutexHeartSensor);  // Release mutex
    }
    
    // For Debugging Use
    //serial.print("Average Heart rate: ");
    //serial.println(readingcpy);
    //write The HeartRate to Screen
    writePulse(readingcpy);

    //Access shared variables with mutex (if enabled)
    // if (readingcpy > 130 && speed < 80) {
    //   if (!HEA_AlertDialog) {
    //     Print_Test_MSG("Please breath good", 7);
    //   }
    //   Alarm = true;
    //   if (HEA_AlertDialog == 20)
    //     Home_Screen();
    //   HEA_AlertDialog++;
    // } else {
    //   HEA_AlertDialog = 0;
    //   Alarm = false;
    // }
    /*******************************************************/
    bool touch = (readingcpy < 10 && speed > 20) ? true : false;
    if (touch) {
      if (!HOL_AlertDialog) {
        Print_Test_MSG("Please hold the wheel", 8);
      }
      Alarm = true;
      if (HOL_AlertDialog == 20)
        Home_Screen();
      HOL_AlertDialog++;
    } else {
      HOL_AlertDialog = 0;
      Alarm = false;
    }

    writeTouch(touch);

    //   // Access shared variables with mutex (if enabled)
    // if (mutexHeartSensor != NULL) {
    //   xSemaphoreTake(mutexHeartSensor, portMAX_DELAY);  // Acquire mutex
    // }
    // if (speed > 80 && readingcpy > 80)  //should get speed limit fro firebase
    // {
    //   if (!SPE_AlertDialog) {
    //     Print_Test_MSG("Please Slow down", 6);
    //   }
    //   Alarm = true;
    //   if (SPE_AlertDialog == 20)
    //     Home_Screen();
    //   SPE_AlertDialog++;
    // } else {
    //   SPE_AlertDialog = 0;
    //   Alarm = false;
    // }
    // // Release mutex after access (if enabled)
    //   if (mutexHeartSensor != NULL) {
    //     xSemaphoreGive(mutexHeartSensor);  // Release mutex
    //   }
    //Reset Variables
    ReadingsCnt = 0;
    heartRate = 0;
    readingcpy = 0;
  }

  // Additional processing or data transmission (if needed)
}