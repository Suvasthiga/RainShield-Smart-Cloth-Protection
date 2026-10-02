RAInSHIELD - SMART CLOTH PROTECTION AND RAINWATER HARVESTING SYSTEM

Project Description:
RainShield is an ESP32-based embedded system designed to protect clothes from unexpected rainfall while collecting and managing rainwater for harvesting.

Hardware:
- ESP32 Dev Module
- Rain Sensor
- Water Level Sensor
- Servo Motor 1 for cloth protection
- Servo Motor 2 for water-flow control

Pin Configuration:
- Rain Sensor -> GPIO34
- Water Level Sensor -> GPIO35
- Servo 1 -> GPIO18
- Servo 2 -> GPIO19

Software:
- Arduino IDE
- Embedded C/C++
- ESP32Servo library

Main Operation:
The rain sensor detects rainfall and sends the condition to the ESP32. When rain is detected, the first servo moves the cloth protection mechanism to a protected position. The water-level sensor monitors the harvesting tank. The second servo controls the water-flow mechanism according to the tank condition.

Source Code:
RAINSHIELD.ino
