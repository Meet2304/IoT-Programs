// Include Wire library for I2C
#include<Wire.h>

// Define Slave I2C adress
#define SLAVE_ADDR 9
//Analog Pin for Potentiometer
int Analog_Pin = A0;
// Integer to hold potentiometer value
int val = 0;

void setup() {
  Wire.begin();
}

void loop() {
  delay(50);
  // Read Pot Value
  // Map to range of 1-255 for flash rate
  val = map(analogRead(Analog_Pin), 0, 1023, 255, 1);

  // Write a character to slave
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(val);
  Wire.endTransmission();
}
