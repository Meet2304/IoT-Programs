/*
 * Author:  Akshay Momaya
 * Channel: Mission Critical
 * Topic:   ESP32 SPIFFS Data test.
 */
#include "SPIFFS.h"

void setup() 
{
  // Small delay to ensure proper initialization
  delay(1000);

  // Start Serial communication
  Serial.begin(115200); 

  // Wait for Serial communication to be initialized
  while (!Serial) {
    delay(10); // Wait for the Serial Monitor to open
  }

  Serial.println("Mission Critical");
  Serial.println("SPIFFS File Content Tester");

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) { 
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println("SPIFFS mounted successfully");
}

void loop() {
  // Open the root directory
  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("Failed to open root directory");
    delay(5000); // Wait before retrying
    return;
  }

  if (!root.isDirectory()) {
    Serial.println("Root is not a directory");
    delay(5000); // Wait before retrying
    return;
  }

  // Open next file in the root directory
  File file = root.openNextFile();

  while (file) {
    Serial.print("FILE: ");
    Serial.println(file.name());

    // Read and print the file content
    Serial.println("CONTENT:");
    while (file.available()) {
      Serial.write(file.read());
    }
    Serial.println(); // Add a newline after the file content

    // Open the next file
    file = root.openNextFile();
  }

  Serial.println("Finished listing files. Waiting for 5 seconds before retrying...");
  delay(5000); // Wait for 5 seconds before listing files again
}
