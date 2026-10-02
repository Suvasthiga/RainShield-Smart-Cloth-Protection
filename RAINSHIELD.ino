#include <ESP32Servo.h>

// =====================================================
// RAINSHIELD - SMART RAINWATER HARVESTING AND
// AUTOMATIC CLOTH PROTECTION SYSTEM
// =====================================================

// ---------------- Pin Configuration ----------------
const int rainPin = 34;       // Rain Sensor - Digital Input
const int waterPin = 35;      // Water Level Sensor - Analog Input
const int roofServoPin = 18;  // Servo 1 - Cloth/Roof Protection
const int valveServoPin = 19; // Servo 2 - Water Flow Control

// ---------------- Servo Objects ----------------
Servo roofServo;
Servo valveServo;

// ---------------- Servo Positions ----------------
const int ROOF_NORMAL = 0;       // Clothes exposed / normal position
const int ROOF_PROTECTED = 90;   // Clothes protected from rain

const int VALVE_CLOSED = 0;      // Water flow stopped
const int VALVE_OPEN = 90;       // Water flow allowed

// ---------------- Tank Setting ----------------
const int TANK_FULL_LEVEL = 90;  // Percentage at which tank is considered full

// ---------------- Timing ----------------
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 3000;

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);

  pinMode(rainPin, INPUT);
  pinMode(waterPin, INPUT);

  roofServo.setPeriodHertz(50);
  valveServo.setPeriodHertz(50);

  roofServo.attach(roofServoPin, 500, 2400);
  valveServo.attach(valveServoPin, 500, 2400);

  // Initial positions
  roofServo.write(ROOF_NORMAL);
  valveServo.write(VALVE_CLOSED);

  Serial.println("================================================");
  Serial.println("RAINSHIELD - SMART RAINWATER HARVESTING AND");
  Serial.println("AUTOMATIC CLOTH PROTECTION SYSTEM");
  Serial.println("================================================");
  Serial.println("ESP32 Dev Module");
  Serial.println("System Initializing...");
  Serial.println("Rain Sensor              : GPIO34 (Digital Input)");
  Serial.println("Water Level Sensor       : GPIO35 (Analog Input)");
  Serial.println("Servo 1 (Cloth Protection): GPIO18 (Output)");
  Serial.println("Servo 2 (Water Flow)      : GPIO19 (Output)");
  Serial.println("------------------------------------------------");
  Serial.println("System Initialized Successfully!");
  Serial.println("================================================");
}

// =====================================================
// READ WATER LEVEL
// Converts the analog sensor reading to percentage.
// =====================================================
int readWaterLevel() {
  int sensorValue = analogRead(waterPin);

  // ESP32 ADC range: approximately 0 to 4095
  int percentage = map(sensorValue, 0, 4095, 0, 100);
  percentage = constrain(percentage, 0, 100);

  return percentage;
}

// =====================================================
// DISPLAY SYSTEM STATUS
// =====================================================
void displayStatus(int rainDetected, int waterLevel) {

  bool tankFull = (waterLevel >= TANK_FULL_LEVEL);

  Serial.println();
  Serial.println("---------------- STATUS UPDATE ----------------");

  // Time since ESP32 started
  unsigned long totalSeconds = millis() / 1000;
  int hours = (totalSeconds / 3600) % 24;
  int minutes = (totalSeconds / 60) % 60;
  int seconds = totalSeconds % 60;

  char timeString[10];
  sprintf(timeString, "%02d:%02d:%02d", hours, minutes, seconds);

  Serial.print("Time                   : ");
  Serial.println(timeString);

  Serial.print("Rain Sensor            : ");
  Serial.print(rainDetected);
  Serial.println(rainDetected ? " (Rain Detected)" : " (No Rain)");

  Serial.print("RAIN DETECTED          : ");
  Serial.println(rainDetected ? "YES" : "NO");

  Serial.print("Water Level            : ");
  Serial.print(waterLevel);
  Serial.println(" %");

  Serial.print("Tank Status             : ");
  Serial.println(tankFull ? "FULL" : "NOT FULL");

  if (rainDetected) {
    roofServo.write(ROOF_PROTECTED);

    Serial.println("Servo 1 (GPIO18)       : Activated (Moving to protected position)");
    Serial.println("Clothes Status         : PROTECTED");

    if (!tankFull) {
      valveServo.write(VALVE_OPEN);
      Serial.println("Servo 2 (GPIO19)       : OPEN (Allowing water flow)");
      Serial.println("Water Flow             : OPEN");
    } else {
      valveServo.write(VALVE_CLOSED);
      Serial.println("Servo 2 (GPIO19)       : CLOSED (Tank full)");
      Serial.println("Water Flow             : STOPPED");
    }

  } else {
    roofServo.write(ROOF_NORMAL);
    valveServo.write(VALVE_CLOSED);

    Serial.println("Servo 1 (GPIO18)       : Returned to normal position");
    Serial.println("Clothes Status         : NORMAL POSITION");
    Serial.println("Servo 2 (GPIO19)       : CLOSED (Ready for next rain)");
    Serial.println("Water Flow             : CLOSED");
  }

  Serial.println("-----------------------------------------------");
  Serial.print("OVERALL SYSTEM STATUS : ");

  if (rainDetected && tankFull) {
    Serial.println("TANK FULL - WATER FLOW STOPPED");
  } else if (rainDetected) {
    Serial.println("RAIN PROTECTION ACTIVE");
  } else {
    Serial.println("NORMAL");
  }

  Serial.println("================================================");
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  // Read rain sensor
  int rainState = digitalRead(rainPin);

  // In this project:
// 1 = Rain detected
// 0 = No rain
  bool rainDetected = (rainState == HIGH);

  // Read water level
  int waterLevel = readWaterLevel();

  // Update the system periodically
  if (millis() - lastUpdate >= UPDATE_INTERVAL || lastUpdate == 0) {
    lastUpdate = millis();

    displayStatus(rainDetected, waterLevel);
  }

  delay(100);
}
