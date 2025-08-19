#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 32             // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11         // DHT 11

#define SOIL_MOISTURE_PIN 34  // Soil moisture sensor pin
#define LDR_PIN 35            // LDR sensor pin

#define WIFI_SSID "Meet"
#define WIFI_PASSWORD "Tobham@2304"

#define SMTP_SERVER "smtp.gmail.com"
#define SMTP_PORT 587

#define SENDER_EMAIL "cropmaster63@gmail.com"
#define SENDER_PASSWORD "ytgz xaip avos ffja"
#define RECIPIENT_EMAIL "meetbhatt2304@gmail.com"
#define RECIPIENT_NAME "GOD"

SMTP_Client smtp;

// Define thresholds for sensor readings
const float TEMPERATURE_THRESHOLD = 20.0;   // Example thresholds, adjust as needed
const float HUMIDITY_THRESHOLD = 70.0;      // Example thresholds, adjust as needed
const int LIGHT_THRESHOLD = 10;            // Example thresholds, adjust as needed
const int SOIL_MOISTURE_THRESHOLD = 30;     // Example thresholds, adjust as needed

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to WiFi...");

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize SMTP session
  ESP_Mail_Session session;
  session.server.host_name = SMTP_SERVER;
  session.server.port = SMTP_PORT;
  session.login.email = SENDER_EMAIL;
  session.login.password = SENDER_PASSWORD;
  session.login.user_domain = "";

  // Connect to SMTP server
  if (!smtp.connect(&session)) {
    Serial.println("Failed to connect to SMTP server");
    return;
  }

  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  dht.begin();
}

void loop() {
  // Read sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightIntensity = analogRead(LDR_PIN);
  int soilMoisture = (100 - (analogRead(SOIL_MOISTURE_PIN) / 4095.00) * 100);

  // Display sensor readings in the serial monitor
  Serial.println("Temperature: " + String(temperature) + "°C");
  Serial.println("Humidity: " + String(humidity) + "%");
  Serial.println("Light Intensity: " + String(lightIntensity));
  Serial.println("Soil Moisture: " + String(soilMoisture));

  // Check if any sensor reading exceeds the threshold and send email
  if (temperature > TEMPERATURE_THRESHOLD ||
      humidity > HUMIDITY_THRESHOLD ||
      lightIntensity > LIGHT_THRESHOLD ||
      soilMoisture < SOIL_MOISTURE_THRESHOLD) {
    // Initialize the message
    SMTP_Message message;
    message.sender.name = "ESP32";
    message.sender.email = SENDER_EMAIL;
    message.subject = "Sensor Threshold Exceeded";

    // Create message content
    String content = "Sensor readings exceeded threshold:\n";
    content += "Temperature: " + String(temperature) + "°C\n";
    content += "Humidity: " + String(humidity) + "%\n";
    content += "Light Intensity: " + String(lightIntensity) + "\n";
    content += "Soil Moisture: " + String(soilMoisture) + "\n";

    // Set message content
    message.text.content = content.c_str();

    // Add recipient
    message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);

    // Send email
    if (!smtp.sendMail(&message)) {
      Serial.println("Error sending Email, " + smtp.errorReason());
    } else {
      Serial.println("Email sent successfully!");
    }
  }

  // Delay before next reading
  delay(5000); // Delay for 5 seconds before checking again
}