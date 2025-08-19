#include <driver/i2s.h>
#include <math.h> // Include math library for sin()

const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number
const int sample_rate = 44100;
const int freq = 440;
const int samples = 1024;
int16_t buffer[samples];

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("Initializing I2S and generating sine wave...");

    // i2s configuration
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = sample_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    // i2s pin configuration
    i2s_pin_config_t pin_config = {
        .bck_io_num = 27,       // Ensure this is connected to the Bit Clock (BCK) pin on your I2S device
        .ws_io_num = 26,        // Ensure this is connected to the Word Select (WS) pin on your I2S device
        .data_out_num = 25,     // Ensure this is connected to the Data Out (DATA) pin on your I2S device
        .data_in_num = I2S_PIN_NO_CHANGE // Data Input (not used here)
    };

    // configure i2s driver
    esp_err_t err;
    err = i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        Serial.println("Failed to install I2S driver");
    } else {
        Serial.println("I2S driver installed successfully");
    }

    err = i2s_set_pin(i2s_num, &pin_config);
    if (err != ESP_OK) {
        Serial.println("Failed to set I2S pins");
    } else {
        Serial.println("I2S pins set successfully");
    }

    // generate a simple sine wave
    for (int i = 0; i < samples; i++) {
        buffer[i] = (int16_t)(32767.0 * sin(2.0 * M_PI * freq * ((float)i / sample_rate)));
    }

    Serial.println("Starting audio playback...");
}

void loop() {
    size_t bytes_written;
    i2s_write(i2s_num, buffer, samples * sizeof(int16_t), &bytes_written, portMAX_DELAY);
    Serial.print("Playing audio: ");
    Serial.print(bytes_written);
    Serial.println(" bytes written");
    delay(1000); // Adjust the delay as needed
}
