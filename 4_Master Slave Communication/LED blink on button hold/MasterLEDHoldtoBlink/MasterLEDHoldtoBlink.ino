#include <Wire.h>

#define SLAVE_ADDR 9
int BUTTON_PIN = 7;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button with pull-up
  Wire.begin(); // join I2C bus as master
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(buttonState == LOW ? 1 : 0); // active LOW button: send 1 if pressed
  Wire.endTransmission();

  delay(50); // small debounce
}
