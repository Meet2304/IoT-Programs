#include "I2SOutput.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/i2s.h>

#define NUM_FRAMES_TO_SEND 512  // Define the number of frames to send

void i2sWriterTask(void *param) {
    I2SOutput *output = (I2SOutput *)param;
    int availableBytes = 0;
    int buffer_position = 0;
    Frame_t *frames = (Frame_t *)malloc(sizeof(Frame_t) * NUM_FRAMES_TO_SEND);

    while (true) {
        i2s_event_t evt;
        if (xQueueReceive(output->m_i2sQueue, &evt, portMAX_DELAY) == pdPASS) {
            if (evt.type == I2S_EVENT_TX_DONE) {
                size_t bytesWritten = 0;
                do {
                    if (availableBytes == 0) {
                        output->m_sampleGenerator->getFrames(frames, NUM_FRAMES_TO_SEND);
                        availableBytes = NUM_FRAMES_TO_SEND * sizeof(Frame_t);
                        buffer_position = 0;
                    }
                    if (availableBytes > 0) {
                        i2s_write(output->m_i2sPort, buffer_position + (uint8_t *)frames,
                                  availableBytes, &bytesWritten, portMAX_DELAY);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                        Serial.print("Written bytes: ");
                        Serial.println(bytesWritten);
                    }
                } while (bytesWritten > 0);
            }
        }
    }
}

void I2SOutput::start(i2s_port_t i2sPort, i2s_pin_config_t &i2sPins, SampleSource *sampleGenerator) {
    m_sampleGenerator = sampleGenerator;

    // I2S configuration
    i2s_config_t i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = m_sampleGenerator->sampleRate(),
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024
    };

    m_i2sPort = i2sPort;
    // Install and start the I2S driver
    i2s_driver_install(m_i2sPort, &i2sConfig, 4, &m_i2sQueue);
    // Set up the I2S pins
    i2s_set_pin(m_i2sPort, &i2sPins);
    // Clear the DMA buffers
    i2s_zero_dma_buffer(m_i2sPort);
    // Start a task to write samples to the I2S peripheral
    xTaskCreate(i2sWriterTask, "i2sWriterTask", 4096, this, 1, &m_i2sWriterTaskHandle);
}
