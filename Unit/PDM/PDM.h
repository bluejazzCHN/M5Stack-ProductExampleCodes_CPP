#include <M5Stack.h>
#include <driver/i2s.h>
#include "fft.h"

class i2sQueueMsg_t
{
public:
    uint8_t state;
    void *audioPtr;
    uint32_t audioSize;
};

class PDM
{
private:
    /* data */
    static uint16_t SEP_NUM;
    static uint8_t PIN_CLK;
    static uint8_t PIN_DATA;
    static int MODE_MIC;
    static QueueHandle_t fftvalueQueue;
    static QueueHandle_t i2sstateQueue;
    static i2sQueueMsg_t QueueMsg;
    M5Stack _M5;
    TFT_eSprite DisFFTbuff = TFT_eSprite(&_M5.Lcd);
    static bool InitI2SSpakerOrMic(int mode);
    static void i2sMicroFFTtask(void *arg);

public:
    PDM();
    PDM(uint8_t clk, uint8_t data, uint16_t number, uint8_t mic);
    void header(const char *string, uint16_t color);
    void microPhoneSetup();
    void microPhoneFFT();
    ~PDM();
};
