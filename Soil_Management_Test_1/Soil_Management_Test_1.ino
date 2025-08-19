#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

#define DHTPIN 2        // Pin where the DHT11 is connected
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

const int photoresistorPin = 34;  // Pin where the photoresistor is connected
const int soilMoisturePin = 35;   // Pin where the soil moisture sensor is connected

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  dht.begin(); // Initialize DHT sensor
}

void loop() {
  // Reading temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Reading analog values from photoresistor and soil moisture sensor
  int photoresistorValue = analogRead(photoresistorPin);
  int soilMoistureValue = analogRead(soilMoisturePin);

  // Sending data to server
  sendData(temperature, humidity, photoresistorValue, soilMoistureValue);

  // Wait for some time before sending data again
  delay(5000);
}

void sendData(float temperature, float humidity, int photoresistorValue, int soilMoistureValue) {
  if (WiFi.status() == WL_CONNECTED) { // Check if connected to WiFi
    HTTPClient http;
    String data = "temperature=" + String(temperature) +
                  "&humidity=" + String(humidity) +
                  "&photoresistor=" + String(photoresistorValue) +
                  "&soilmoisture=" + String(soilMoistureValue);
    Serial.println("Sending data: " + data);

    http.begin("http://your-server-url.com/data"); // Change this to your server URL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send HTTP POST request
    int httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error sending POST request");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
