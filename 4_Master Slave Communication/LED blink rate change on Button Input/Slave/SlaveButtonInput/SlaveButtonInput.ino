#include <Wire.h>

#define SLAVE_ADDR 9
int LED_PIN = 7;
int rd = 0; // received data

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Wire.begin(SLAVE_ADDR);              // join I2C bus as slave
  Wire.onReceive(receiveEvent);        // register event handler

  Serial.begin(9600);
  Serial.println("I2C Master-Slave Communication Started");
}

void receiveEvent(int howMany) {
  if (Wire.available()) {
    rd = Wire.read();  // read one byte (0 or 1)
    Serial.print("Received: ");
    Serial.println(rd);
  }
}

void loop() {
  digitalWrite(LED_PIN, rd == 1 ? HIGH : LOW);
}
