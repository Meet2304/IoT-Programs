#include <Arduino.h>
#include <driver/i2s.h>
#include <SPIFFS.h>
#include <FS.h>

#include "SampleSource.h"
#include "I2SOutput.h"

// Pin Configuration
#define I2S_LRCK_PIN 26   // LRC (Left/Right Clock)
#define I2S_BCLK_PIN 27   // BCLK (Bit Clock)
#define I2S_DATA_PIN 25   // DIN (Data Input)

// File Path for WAV File
#define WAV_FILE_PATH "/data/Audio.wav"

// Create instances of SampleSource and I2SOutput
SampleSource sampleSource;
I2SOutput i2sOutput;

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing...");

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed!");
        return;
    }

    sampleSource.init(WAV_FILE_PATH);

    i2s_pin_config_t i2sPinConfig = {
        .bck_io_num = I2S_BCLK_PIN,
        .ws_io_num = I2S_LRCK_PIN,
        .data_out_num = I2S_DATA_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE // Data Input (not used)
    };
    i2sOutput.start(I2S_NUM_0, i2sPinConfig, &sampleSource);
}

void loop() {
    Serial.println("Playing audio...");
    delay(1000);
}
