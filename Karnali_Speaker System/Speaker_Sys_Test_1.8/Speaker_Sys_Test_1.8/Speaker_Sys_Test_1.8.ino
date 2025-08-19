#include <WiFi.h>
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <FS.h>

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
String audioFileName = "/Husn.mp3"; // Default audio file name

void listSPIFFSFiles() {
  Serial.println("SPIFFS files:");
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.print("FILE: ");
    Serial.println(file.name());
    file = root.openNextFile();
  }
  Serial.println();
}

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount file system");
    return;
  }

  // List all files in SPIFFS
  listSPIFFSFiles();

  // Initialize I2S output
  out = new AudioOutputI2S();
  out->SetPinout(26, 25, 27);
  out->SetGain(volume / 21.0);

  // Wait for Serial Monitor to open
  while (!Serial) {
    delay(10);
  }

  Serial.println("Type 'on' to power on and play audio, 'off' to stop audio, 'up' to increase volume, 'down' to decrease volume.");
  Serial.print("Audio file to be played: ");
  Serial.println(audioFileName);
}

void loop() {
  // Print instructions and current status in a loop
  Serial.println("Type 'on' to power on and play audio, 'off' to stop audio, 'up' to increase volume, 'down' to decrease volume.");
  if (isPoweredOn) {
    Serial.println("Current status: Powered on and playing audio");
  } else {
    Serial.println("Current status: Powered off");
  }
  delay(5000); // Delay to avoid spamming the Serial Monitor

  if (isPoweredOn) {
    if (mp3 && mp3->isRunning()) {
      Serial.println("Playing audio..."); // Continuous print while audio is playing
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
    // Verify if the file exists and has a non-zero size
    if (!SPIFFS.exists(audioFileName)) {
      Serial.println("Audio file does not exist");
      return;
    }
    File testFile = SPIFFS.open(audioFileName);
    if (!testFile || testFile.size() == 0) {
      Serial.println("Audio file is empty or cannot be opened");
      testFile.close();
      return;
    }
    testFile.close();

    // Turn on and play audio
    file = new AudioFileSourceSPIFFS(audioFileName.c_str()); // Load the audio file from SPIFFS
    if (!file->open(audioFileName.c_str())) {
      Serial.println("Failed to open audio file");
      return;
    }

    // Read the first few bytes to check if the file is read correctly
    uint8_t buffer[10]; // Increase buffer size to read more of the header
    if (file->read(buffer, sizeof(buffer)) != sizeof(buffer)) {
      Serial.println("Failed to read the start of the audio file");
      return;
    }

    // Print the audio file name and the first few bytes for debugging
    Serial.print("Audio file: ");
    Serial.println(audioFileName);
    Serial.print("First 10 bytes of the file: ");
    for (int i = 0; i < sizeof(buffer); i++) {
      Serial.printf("%02X ", buffer[i]);
    }
    Serial.println();

    Serial.println("Initializing MP3 playback...");
    mp3 = new AudioGeneratorMP3();
    if (!mp3->begin(file, out)) {
      Serial.println("Failed to begin MP3 playback");
      return;
    }

    isPoweredOn = true;
    Serial.println("Powered on and playing audio");
  }
}

void powerOff() {
  if (isPoweredOn) {
    Serial.println("Stopping audio playback...");
    // Turn off and stop audio
    if (mp3) {
      mp3->stop();
      delete mp3;
      mp3 = nullptr;
    }
    if (file) {
      delete file;
      file = nullptr;
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
