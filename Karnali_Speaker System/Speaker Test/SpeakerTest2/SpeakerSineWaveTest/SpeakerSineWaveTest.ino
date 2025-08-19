#include <driver/dac.h>
#include <math.h>

const int sampleRate = 1000; // Sample rate in Hz
const int amplitude = 127;   // Amplitude of the sine wave (0-255)
const int offset = 128;      // Offset to center the sine wave (0-255)
const int frequency = 440;   // Frequency of the sine wave in Hz (A4 note)
const dac_channel_t dacPin = DAC_CHANNEL_1; // DAC1 pin is GPIO 25

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing DAC...");
  dac_output_enable(dacPin);
}

void loop() {
  unsigned long currentTime = micros();
  unsigned long period = 1000000 / sampleRate;

  for (int i = 0; i < sampleRate; i++) {
    // Calculate the sine wave value
    int value = offset + (amplitude * sin(2 * PI * frequency * i / sampleRate));

    // Output the value to the DAC pin
    dac_output_voltage(dacPin, value);

    // Wait until the next sample period
    while (micros() - currentTime < period) {
      // Busy-wait loop
    }
    currentTime += period;
  }

  // Debugging output
  Serial.println("Sine wave generated and output to speaker");
  delay(1000); // Print message every second
}
