#ifndef __sample_source_h__
#define __sample_source_h__

#include <Arduino.h>
#include <FS.h>

typedef struct
{
    int16_t left;
    int16_t right;
} Frame_t;

class SampleSource
{
private:
    File audioFile;

public:
    void init(const char *filePath);

    int sampleRate(); // Implement if needed

    void getFrames(Frame_t *frames, int numberFrames); // Implement if needed
};

#endif
