#include <Wire.h>

#define SLAVE_ADDR 9
int BUTTON_PIN = 7;

bool lastButtonState = HIGH;   // store previous state (pull-up idle HIGH)
bool ledState = LOW;           // store LED state to toggle

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // enable pull-up for stable button
  Wire.begin(); // join I2C bus as master
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  // detect a button press (HIGH → LOW transition for pull-up)
  if (lastButtonState == HIGH && buttonState == LOW) {
    ledState = !ledState; // toggle state

    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(ledState ? 1 : 0); // send new state
    Wire.endTransmission();

    delay(50); // debounce
  }

  lastButtonState = buttonState; // update previous state
}
