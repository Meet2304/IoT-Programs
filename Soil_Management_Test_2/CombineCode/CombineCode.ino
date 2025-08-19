#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 32     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

int _moisture,moisture_sensor_analog;
const int moisture_sensor_pin = 34;  /* Soil moisture sensor O/P pin */
const int ldrPin = 35; // Digital pin D32

DHT dht(DHTPIN, DHTTYPE);

void setup(void){
  Serial.begin(115200);     /* Set the baudrate to 115200*/
  pinMode(ldrPin, INPUT);    // Works on 9600 baud rate!!!!!
  pinMode(moisture_sensor_pin, INPUT);
  dht.begin();               // Works on 9600 baud rate!!!!!
}

void loop(void){
  moisture_sensor_analog = analogRead(moisture_sensor_pin);
  int LDRsensorValue = analogRead(ldrPin);
  _moisture = ( 100 - ( (moisture_sensor_analog/4095.00) * 100 ) );
  Serial.print("Moisture = ");
  Serial.print(_moisture);  /* Print Temperature on the serial window */
  Serial.println("%");
  delay(1000);              /* Wait for 1000mS */

  Serial.print("LDR Sensor value: ");
  Serial.println(LDRsensorValue);
  delay(1000);

  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}



// void setup() {
//   Serial.begin(9600);
//   Serial.println(F("DHTxx test!"));

  
// }

// void loop() {
  // Wait a few seconds between measurements.
  

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
  //  Serial.println(F("Failed to read from DHT sensor!"));
  //  return;
  //}

  // Compute heat index in Fahrenheit (the default)
  
// }
