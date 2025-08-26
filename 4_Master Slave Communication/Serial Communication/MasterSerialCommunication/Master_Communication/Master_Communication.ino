# include <Wire.h>

#define SLAVE_ADDR 9

#define ANSWERSIZE 5

void setup() {
  Wire.begin();

  Serial.begin(9600);
  Serial.println("I2C Master Demonstration");
}

void loop() {
  delay(50);
  Serial.println("Write data to slave");

  // Write character to Slave
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println("Recieve data");

  // Read response back from Slave
  // Read back 5 characters
  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  // Add characters to string
  String response = "";
  while(Wire.available()) {
    char b = Wire.read();
    response += b;
  }

  Serial.println(response);
}
