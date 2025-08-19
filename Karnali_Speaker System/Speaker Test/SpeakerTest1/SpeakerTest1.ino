#include <Arduino.h>
#include <driver/i2s.h>

// Pin definitions
#define LRC_PIN 26
#define BCLK_PIN 27
#define DIN_PIN 25

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  // I2S configuration
  i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX), // Only TX
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // Interrupt level 1
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true // Auto clear tx descriptor on underflow
  };

  // I2S pin configuration
  i2s_pin_config_t pin_config = {
    .bck_io_num = BCLK_PIN,
    .ws_io_num = LRC_PIN,
    .data_out_num = DIN_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  // Install and start I2S driver
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
  Serial.println("I2S configured");

  // Generate a sine wave
  const int sampleRate = 44100;
  const int amplitude = 10000;  // Change this value to adjust volume
  const int frequency = 440;    // A4 note
  int16_t sample;
  size_t bytes_written;

  for (int i = 0; i < sampleRate; i++) {
    sample = amplitude * sin(2.0 * PI * frequency * i / sampleRate);
    i2s_write(I2S_NUM_0, &sample, sizeof(sample), &bytes_written, portMAX_DELAY);
  }
}

void loop() {
  // Do nothing in loop
}
