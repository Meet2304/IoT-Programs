#include <FS.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

AudioFileSourceSPIFFS *file;
AudioGeneratorMP3 *audio;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing SPIFFS...");
  
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS filesystem.");
    while (true) {} // Loop indefinitely
  }

  Serial.println("SPIFFS filesystem mounted successfully.");

  Serial.println("Initializing Audio...");
  file = new AudioFileSourceSPIFFS("/audio.mp3");
  audio = new AudioGeneratorMP3();
  out = new AudioOutputI2S();
  
  audio->begin(file, out);
  
  Serial.println("Audio Initialized. Starting playback...");

  // Print initial message to Serial Monitor
  Serial.println("Audio Playback Started.");

  // Print debugging messages in a loop
  while (true) {
    if (!audio->isRunning()) {
      Serial.println("Audio Playback Finished.");
      while (true) {} // Loop indefinitely
    }

    delay(1000); // Add a delay to reduce Serial Monitor output frequency
  }
}

void loop() {
  // This function will not be used in this example
}
