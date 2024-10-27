#include <esp32-hal-gpio.h>
#include <Arduino.h>
#include <FirebaseESP32.h>
#include <WiFi.h>
// Digital input pin for the Buzzer
#define AlcoholSensorPin 16  // Digital output pin for the MQ-3 sensor
#define BUZZER_PIN 17        // Digital output pin for Buzzer
#define speed_pin 33         // Potentiometer pin For Testing
#define battery_pin 32       // Potentiometer pin For Testing

#define FIREBASE_HOST "https://safesteer-3b5e6-default-rtdb.firebaseio.com"  // Your Firebase Project URL
#define FIREBASE_AUTH "ztkBj5BM3mXtYyg6x3ePkf5zJAqrDVojDvHtg5nP"             // Your Firebase Database Secret

// Wi-Fi credentials
#define WIFI_SSID "Dr.Elsayed"
#define WIFI_PASSWORD "S10101972"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
// extern int heartRate;
static int rpm = 0, speed = 0, battery = 0;
// extern SemaphoreHandle_t mutexHeartSensor;
volatile bool SpeedLimitExceeded = false;
volatile bool AlcoholDetected = false;
volatile bool Alarm = false;
volatile bool touch = false;

TaskHandle_t HeartSensorTaskHandle = NULL;
TaskHandle_t TouchSensorTaskHandle = NULL;
TaskHandle_t AlcoholSensorTaskHandle = NULL;
TaskHandle_t GyroscopeSensorTaskHandle = NULL;
TaskHandle_t Refresh_ScreenTaskHandle = NULL;
TaskHandle_t SendDataTaskHandle = NULL;
TaskHandle_t CheckAlarmTaskHandle = NULL;


void Wheel_Test() {

  Print_Test_MSG("Please put your hand to test sensor", 5);
  HeartSensor_Init();  //!!!! Remove################################################
  while (!TestHeartSensor())
    ;

  Print_Test_MSG("DONE", 4);
  delay(2000);

  // Print_Test_MSG("Please rotate wheel right and left to test sensor",5);
  // while ( !Test_Y_Gyroscope() );

  // Print_Test_MSG("DONE",4);
  // delay(1000);
  Print_Test_MSG("Test alert", 5);
  Buzzer_Long_Beeb();
  delay(500);
  Print_Test_MSG("DONE", 4);
  delay(2000);
  Home_Screen();
}

void Wheel_Init() {
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Buzzer_Init();
  AlcoholSensor_Init();
  // GyroscopeSensor_Init();
  // calibrateGyroscope(100);
  Screen_Init();
  endCommand();
  delay(2000);
}

void SendDataTask(void *pvParameters) {
  while (1) {
    
    String health = AlcoholDetected ? "Drunk" : "Sober";
    String touch = touch ? "Touched" : "Not touched";
    Firebase.setString(firebaseData, "/Cars/-O3oWxm5_fZeaSjIuPsr/Health", health);
    Firebase.setString(firebaseData, "/Cars/-O3oWxm5_fZeaSjIuPsr/Status", touch);
    Firebase.setString(firebaseData, "/Cars/-O3oWxm5_fZeaSjIuPsr/Speed", speed);
    Firebase.setString(firebaseData, "/Cars/-O3oWxm5_fZeaSjIuPsr/Battery", battery);

    vTaskDelay(pdMS_TO_TICKS(50));  //(NEW) Adjust the delay as needed
    //vTaskDelay(pdMS_TO_TICKS(50));  // Adjust the delay as needed
  }
}
bool xx= false;
void Refresh_ScreenTask(void *pvParameters) {
  // static char SPE_AlertDialog = 0;
  while (1) {
    /********************************************************/
    if (digitalRead(speed_pin) && speed < 220) {
      speed++;
      (speed % 5 == 0) ? rpm++ : rpm;  //
      if (rpm == 8)
        rpm = 3;
    } else if (speed > 0) {
      speed--;
      if (rpm > 0)
        rpm--;
    }

    if (digitalRead(battery_pin) && battery < 100) {
      battery++;
    } else if (battery > 0) {
      battery--;
    }
    /********************************************************/
    //     // Access shared variables with mutex (if enabled)
    //   if (mutexHeartSensor != NULL) {
    //     xSemaphoreTake(mutexHeartSensor, portMAX_DELAY);  // Acquire mutex
    //   }
    //     if (speed > 80 && heartRate > 80)  //should get speed limit fro firebase
    //     {
    //       if (!SPE_AlertDialog) {
    //         Print_Test_MSG("Please Slow down", 6);
    //       }
    //       Alarm = true;
    //       if (SPE_AlertDialog == 20)
    //         Home_Screen();
    //       SPE_AlertDialog++;
    //     } else {
    //       SPE_AlertDialog = 0;
    //       Alarm = false;
    //     }
    // // Release mutex after access (if enabled)
    //   if (mutexHeartSensor != NULL) {
    //     xSemaphoreGive(mutexHeartSensor);  // Release mutex
    //   }

    if(speed>90)
    {
      if(!xx) Print_Test_MSG("Slow Down!!", 6);
      xx=true;
      Alarm = true;
    }
    else
    {
      if(xx) Home_Screen();
      xx=false;
      Alarm = false;
    }
    writeAlcohol(AlcoholDetected);
    Refresh_Screen(rpm, battery, speed);
    // vTaskDelay(pdMS_TO_TICKS(1));  //(NEW) Adjust the delay as needed
    vTaskDelay(pdMS_TO_TICKS(50));  // Adjust the delay as needed
  }
}

void HeartSensorTask(void *pvParameters) {

  Serial.println("Heart Sensor Task started [OK]");
  while (1) {
    // Check the Heart Sensor state
    CheckHeartSensor();
    // Access shared variables with mutex (if enabled)
    // if (mutexHeartSensor != NULL) {
    //   xSemaphoreTake(mutexHeartSensor, portMAX_DELAY);  // Acquire mutex
    // }
    //   if (heartRate > 130 && speed < 80) {
    //     if (!HEA_AlertDialog) {
    //       Print_Test_MSG("Please breath good", 7);
    //     }
    //     Alarm = true;
    //     if (HEA_AlertDialog == 20)
    //       Home_Screen();
    //     HEA_AlertDialog++;
    //   } else {
    //     HEA_AlertDialog = 0;
    //     Alarm = false;
    //   }
    //   /*******************************************************/
    //   bool touch = (heartRate < 10 && speed > 20) ? true : false;
    //   if (touch) {
    //     if (!HOL_AlertDialog) {
    //       Print_Test_MSG("Please hold the wheel", 8);
    //     }
    //     Alarm = true;
    //     if (HOL_AlertDialog == 20)
    //       Home_Screen();
    //     HOL_AlertDialog++;
    //   } else {
    //     HOL_AlertDialog = 0;
    //     Alarm = false;
    //   }
    //   // Release mutex after access (if enabled)
    // if (mutexHeartSensor != NULL) {
    //   xSemaphoreGive(mutexHeartSensor);  // Release mutex
    // }

    // writeTouch(touch);
    // Reset variables for the next averaging period (optional)
    // heartRate = 0;
    // spO2 = 0;
    // Additional Heart Sensor processing here
    vTaskDelay(pdMS_TO_TICKS(100));  // Adjust delay as needed
  }
}

void AlcoholSensorTask(void *pvParameters) {
  static char ALC_AlertDialog = 0;
  while (1) {
    // Check the Alcohol Sensor state
    AlcoholDetected = CheckAlcoholState();
    if (AlcoholDetected) {
      if (!ALC_AlertDialog) {
        Print_Test_MSG("Alcohol Detected", 9);
      }
      Alarm = true;
      if (ALC_AlertDialog == 10)
        Home_Screen();
      ALC_AlertDialog++;
    } else {
      ALC_AlertDialog = 0;
      Alarm = false;
    }
    // vTaskDelay(pdMS_TO_TICKS(15));  // (NEW) Adjust the delay as needed
    vTaskDelay(pdMS_TO_TICKS(350));  // Adjust the delay as needed
  }
}

void GyroscopeSensorTask(void *pvParameters) {
  Serial.println("Gyroscope Sensor Task started [OK]");
  while (1) {
    //Check For Gyroscope Sensor Readings
    if (Gyroscope_Read())
      digitalWrite(17, HIGH);
    else
      digitalWrite(17, LOW);

    // delay(50);
    vTaskDelay(pdMS_TO_TICKS(200));  // Adjust the delay as needed
  }
}

void CheckAlarmTask(void *pvParameters) {
  while (1) {
    if (Alarm) {
      // Alarm
      tone(BUZZER_PIN, 2000);
      delay(200);
      noTone(BUZZER_PIN);
      delay(100);
    }
    // vTaskDelay(pdMS_TO_TICKS(50));  // (NEW) Adjust the delay as needed
    vTaskDelay(pdMS_TO_TICKS(80));  // Adjust the delay as needed
  }
}


void setup() {
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  Serial.begin(9600);
  

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase with the configuration and authentication
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Don't Try to adjust these Lines !!!
  Wheel_Init();
  Wheel_Test();
  HeartSensor_Init();
  pinMode(speed_pin, INPUT);
  pinMode(battery_pin, INPUT);
  HeartSensor_Init();
  interrupts();

  // Create tasks with priorities
  xTaskCreate(HeartSensorTask, "HeartSensorTask", 5000, NULL, 7, &HeartSensorTaskHandle);
  //xTaskCreate(GyroscopeSensorTask, "GyroscopeSensorTask", 5000, NULL, 6, &GyroscopeSensorTaskHandle);
  xTaskCreate(CheckAlarmTask, "CheckAlarmTask", 5000, NULL, 6, &CheckAlarmTaskHandle);
  xTaskCreate(Refresh_ScreenTask, "Refresh_ScreenTask", 5000, NULL, 5, &Refresh_ScreenTaskHandle);
  xTaskCreate(AlcoholSensorTask, "AlcoholSensorTask", 5000, NULL, 6, &AlcoholSensorTaskHandle);
  xTaskCreate(SendDataTask, "SendDataTask", 10000, NULL, 3, &SendDataTaskHandle);
}
void loop() {
  //This Is The Super Loop!!
}
