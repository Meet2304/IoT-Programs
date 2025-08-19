#include <WiFi.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

// Volume control
int volume = 10; // Initial volume level
const int maxVolume = 21; // Maximum volume level
const int minVolume = 0; // Minimum volume level

// Audio objects
AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;

// State variables
bool isPoweredOn = false;

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Initialize I2S output
  out = new AudioOutputI2S();
  out->SetPinout(26, 25, 27);
  out->SetGain(volume / 21.0);

  Serial.println("Type 'on' to power on and play audio, 'off' to stop audio, 'up' to increase volume, 'down' to decrease volume.");
}

void loop() {
  if (isPoweredOn) {
    if (mp3 && mp3->isRunning()) {
      if (!mp3->loop()) {
        mp3->stop();
        isPoweredOn = false;
        Serial.println("Audio playback finished");
      }
    }
  }

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "on") {
      powerOn();
    } else if (command == "off") {
      powerOff();
    } else if (command == "up") {
      increaseVolume();
    } else if (command == "down") {
      decreaseVolume();
    }
  }
}

void powerOn() {
  if (!isPoweredOn) {
    // Turn on and play audio
    file = new AudioFileSourceSPIFFS("/data/Husn.mp3"); // Load "Husn.mp3" from the "data" folder
    mp3 = new AudioGeneratorMP3();
    mp3->begin(file, out);
    isPoweredOn = true;
    Serial.println("Powered on and playing audio");
  }
}

void powerOff() {
  if (isPoweredOn) {
    // Turn off and stop audio
    if (mp3) {
      mp3->stop();
      delete mp3;
      mp3 = nullptr;
    }
    isPoweredOn = false;
    Serial.println("Powered off and stopped audio");
  } 
}

void increaseVolume() {
  if (isPoweredOn && volume < maxVolume) {
    volume++;
    out->SetGain(volume / 21.0);
    Serial.print("Volume increased to: ");
    Serial.println(volume);
  }
}

void decreaseVolume() {
  if (isPoweredOn && volume > minVolume) {
    volume--;
    out->SetGain(volume / 21.0);
    Serial.print("Volume decreased to: ");
    Serial.println(volume);
  }
}
