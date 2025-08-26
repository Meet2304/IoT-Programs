// Include Wire library for I2C
#include<Wire.h>

// Define Slave I2C adress
#define SLAVE_ADDR 9

// Define LED pin
int LED_PIN = 7;

// Variable for recieved data
int rd;

// Variable for Blink Rate
int br;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  // Initialize I2C Communication at Slave
  Wire.begin(SLAVE_ADDR);
  //Function to run when data recieved from master
  Wire.onReceive(recieveEvent);
  // Setup Serial Monitor
  Serial.begin(9600);
  Serial.println("I2C Master Slave Communication");
}

void recieveEvent(){
  // Read one character from I2C
  rd = Wire.read();
  // Print value of incomming data
  Serial.println(rd);

}

void loop() {
  delay(50);

  // Calculate blink value
  br = map(rd, 1, 255, 100, 2000);
  digitalWrite(LED_PIN, HIGH);
  delay(br);
  digitalWrite(LED_PIN, LOW);
  delay(br);
}
