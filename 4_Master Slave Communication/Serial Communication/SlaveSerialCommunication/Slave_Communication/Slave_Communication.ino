# include <Wire.h>

#define SLAVE_ADDR 9

#define ANSWERSIZE 5

String answer = "Hello";

void setup() {
  // Initialize I2C communication as Slave
  Wire.begin(SLAVE_ADDR);

  // Function to run when data requested from master
  Wire.onRequest(requestEvent);

  // Function to run when data recieved from master
  Wire.onReceive(recieveEvent);

  Serial.begin(9600);
  Serial.println("I2C Slave Demonstration");
}

void recieveEvent() {

  // Read while data received
  while(0 < Wire.available()) {
    byte x = Wire.read();
  }

  Serial.println("Recieve event");
}

void requestEvent() {

  // Setup byte variable in the correct size
  byte response[ANSWERSIZE];

  // Format answer as array
  for (byte i=0; i<ANSWERSIZE; i++) {
    response[i] = (byte)answer.charAt(i);
  }

  // Send response back to Master
  Wire.write(response, sizeof(response));

  // Print to Serial Monitor
  Serial.println("Request event");
}

void loop() {

  // Time delay in loop
  delay(50);
}
