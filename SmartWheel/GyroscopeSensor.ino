
#include <Wire.h>
#include <L3G.h>

#define THRESHOLD 600       // Adjust this threshold according to your requirements
#define Num_of_Readings 10  // Number of Readings

// L3G Object
L3G gyro;

// Calibration offsets for X, Y, and Z axes
float xOffset = 0.0;
float yOffset = 0.0;
float zOffset = 0.0;

int16_t previousY = 0;
int GyroReadings_Y[Num_of_Readings];
int GyroReadingsCnt = 0;
bool significantChangeFlag = false;

//=== Gyroscope Sensor Initialization ===//
void GyroscopeSensor_Init() {
  // // Serial.begin(115200);
  Serial.begin(9600);

  Serial.println("Initializing Gyroscope Sensor...");

  Wire.begin();

  if (!gyro.init(gyro.device_4200D)) {
    Serial.println("Failed to autodetect gyro type! [ERR]");

    while (1)
      ;

  } else {
    Serial.println("Gyroscope Sensor Initialization success. [OK]");
  }

  gyro.enableDefault();
}


//=== Get Gyroscope Sensor Values ===//
bool Gyroscope_Read() {

  gyro.read();

  int16_t currentX = gyro.g.x - xOffset;
  int16_t currentY = gyro.g.y - yOffset;
  int16_t currentZ = gyro.g.z - zOffset;


  GyroReadings_Y[GyroReadingsCnt++] = currentY;

  if (GyroReadingsCnt == Num_of_Readings) {

    // Sort the readings array
    std::sort(GyroReadings_Y, GyroReadings_Y + Num_of_Readings);

    currentY = GyroReadings_Y[((Num_of_Readings - 1) / 2)];
    Serial.print("->Y: ");
    Serial.println(currentY);

    // Check for significant change on Y-axis (left/right)
    int16_t diffY = abs(currentY - previousY);

    // Update previous readings
    previousY = currentY;

    // Reset Counter
    GyroReadingsCnt = 0;

    // Check if any axis has a significant change
    if (diffY > THRESHOLD) {
      significantChangeFlag = true;
      Serial.println("Significant change detected!");
    } else {
      significantChangeFlag = false;
    }

    Serial.print("G ");
    Serial.print("X: ");
    Serial.print(currentX);
    Serial.print(" Y: ");
    Serial.print(currentY);
    Serial.print(" Z: ");
    Serial.println(currentZ);
  }
  // delay(200);
  return significantChangeFlag;
}



//=== Gyroscope Sensor Calibration ===//
void calibrateGyroscope(int numSamples) {
  float xSum = 0.0;
  float ySum = 0.0;
  float zSum = 0.0;

  for (int i = 0; i < numSamples; i++) {
    gyro.read();
    xSum += gyro.g.x;
    ySum += gyro.g.y;
    zSum += gyro.g.z;
    delay(10);  // Delay between samples
  }

  xOffset = xSum / numSamples;
  yOffset = ySum / numSamples;
  zOffset = zSum / numSamples;
}
