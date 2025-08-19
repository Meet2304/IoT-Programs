#include "SampleSource.h"
#include <SPIFFS.h>

void SampleSource::init(const char *filePath) {
    audioFile = SPIFFS.open(filePath, "r");
    if (!audioFile) {
        Serial.println("Failed to open audio file!");
    } else {
        Serial.println("Audio file opened successfully!");
        Serial.print("File name: ");
        Serial.println(filePath);
        
        // Print the first few bytes of the file for debugging
        Serial.print("First few bytes: ");
        for (int i = 0; i < 16 && audioFile.available(); ++i) {
            uint8_t byte = audioFile.read();
            Serial.print(byte, HEX);
            Serial.print(" ");
        }
        Serial.println();
        
        // Reset the file to the beginning
        audioFile.seek(0);
    }
}

int SampleSource::sampleRate() {
    // Return a fixed sample rate or parse it from the WAV header
    return 44100; // Example value
}

void SampleSource::getFrames(Frame_t *frames, int numberFrames) {
    for (int i = 0; i < numberFrames; ++i) {
        if (audioFile.available()) {
            audioFile.read((uint8_t *) &frames[i], sizeof(Frame_t));
            Serial.print("Read frame: ");
            Serial.print(frames[i].left);
            Serial.print(", ");
            Serial.println(frames[i].right);
        } else {
            frames[i].left = 0;
            frames[i].right = 0;
        }
    }
}
