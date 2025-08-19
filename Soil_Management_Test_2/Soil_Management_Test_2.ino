// Include necessary libraries
#include <Arduino.h>

// Define pin for LDR sensor
const int ldrPin = 35; // Digital pin D32

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set LDR pin as input
  pinMode(ldrPin, INPUT);
}

void loop() {
  // Read the sensor value
  // int sensorValue = digitalRead(ldrPin);
  int sensorValue = analogRead(ldrPin);

  // Print the sensor value to the serial monitor
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);

  // Wait for a short delay before reading the sensor again
  delay(1000);
}
