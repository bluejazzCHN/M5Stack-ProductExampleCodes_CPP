/*
  Easy Learn Series @ refactor. Original Author: songjiangzhang@hotmail.com
  
  The easy way to learn  C++  and M5 ESP32 programming is to refactor existing code.


  class PDM
  =========

  This class is refactored from M5Stack products example codes /Unit/PDM using C++ by songjiangzhang@hotmail.com

  The original version can be donload from https://github.com/m5stack/M5-ProductExampleCodes/blob/master/Unit/PDM/
   
*/

#include "PDM.h"

uint16_t PDM::SEP_NUM = 24;
uint8_t PDM::PIN_CLK = 22;
uint8_t PDM::PIN_DATA = 21;
int PDM::MODE_MIC = 0;

QueueHandle_t PDM::fftvalueQueue = NULL;
QueueHandle_t PDM::i2sstateQueue = NULL;

void PDM::header(const char *string, uint16_t color)
{
    _M5.begin(true, true, true, true);
    _M5.Lcd.fillScreen(color);
    _M5.Lcd.setTextSize(1);
    _M5.Lcd.setTextColor(WHITE, BLACK);
    _M5.Lcd.fillRect(0, 0, 320, 30, BLACK);
    _M5.Lcd.setTextDatum(TC_DATUM);
    _M5.Lcd.drawString(string, 160, 3, 4);
}
void PDM::microPhoneFFT()
{
    uint8_t FFTValueBuff[SEP_NUM];
    xQueueReceive(fftvalueQueue, (void *)&FFTValueBuff, portMAX_DELAY);
    DisFFTbuff.fillRect(0, 0, 320, 54, DisFFTbuff.color565(0x00, 0x00, 0x00));
    uint32_t colorY = DisFFTbuff.color565(0xff, 0x9c, 0x00);
    uint32_t colorG = DisFFTbuff.color565(0x66, 0xff, 0x00);
    uint32_t colorRect;
    for (int x = 0; x < SEP_NUM; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            if (y < FFTValueBuff[23 - x])
            {
                colorRect = colorY;
            }
            else if (y == FFTValueBuff[23 - x])
            {
                colorRect = colorG;
            }
            else
            {
                continue;
            }
            DisFFTbuff.fillRect(x * 12, 54 - y * 6 - 5, 10, 5, colorRect);
        }
    }
    DisFFTbuff.pushSprite(20, 120);
}

void PDM::microPhoneSetup()
{
    fftvalueQueue = xQueueCreate(5, SEP_NUM * sizeof(uint8_t));
    if (fftvalueQueue == 0)
    {
        return;
    }

    i2sstateQueue = xQueueCreate(5, sizeof(i2sQueueMsg_t));
    if (i2sstateQueue == 0)
    {
        return;
    }

    InitI2SSpakerOrMic(MODE_MIC);
    xTaskCreatePinnedToCore(i2sMicroFFTtask, "microPhoneTask", 4096, NULL, 3, NULL, 0);

    DisFFTbuff.createSprite(320, 54);
}

void PDM::i2sMicroFFTtask(void *arg)
{
    uint8_t FFTDataBuff[128];
    uint8_t FFTValueBuff[SEP_NUM];
    uint8_t *microRawData = (uint8_t *)calloc(2048, sizeof(uint8_t));
    size_t bytesread;
    int16_t *buffptr;
    double data = 0;
    float adc_data;
    uint16_t ydata;
    uint32_t subData;

    uint8_t state = MODE_MIC;
    i2sQueueMsg_t QueueMsg;
    while (1)
    {
        if (xQueueReceive(i2sstateQueue, &QueueMsg, (TickType_t)0) == pdTRUE)
        {
            //Serial.println("Queue Now");
            if (QueueMsg.state == MODE_MIC)
            {
                InitI2SSpakerOrMic(MODE_MIC);
                state = MODE_MIC;
            }
        }
        else if (state == MODE_MIC)
        {
            fft_config_t *real_fft_plan = fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
            i2s_read(I2S_NUM_0, (char *)microRawData, 2048, &bytesread, (100 / portTICK_RATE_MS));
            buffptr = (int16_t *)microRawData;

            for (int count_n = 0; count_n < real_fft_plan->size; count_n++)
            {
                adc_data = (float)map(buffptr[count_n], INT16_MIN, INT16_MAX, -2000, 2000);
                real_fft_plan->input[count_n] = adc_data;
            }
            fft_execute(real_fft_plan);

            for (int count_n = 1; count_n < real_fft_plan->size / 4; count_n++)
            {
                data = sqrt(real_fft_plan->output[2 * count_n] * real_fft_plan->output[2 * count_n] + real_fft_plan->output[2 * count_n + 1] * real_fft_plan->output[2 * count_n + 1]);
                if ((count_n - 1) < 128)
                {
                    data = (data > 2000) ? 2000 : data;
                    ydata = map(data, 0, 2000, 0, 255);
                    FFTDataBuff[128 - count_n] = ydata;
                }
            }

            for (int count = 0; count < SEP_NUM; count++)
            {
                subData = 0;
                for (int count_i = 0; count_i < 5; count_i++)
                {
                    subData += FFTDataBuff[count * 5 + count_i];
                }
                subData /= 5;
                FFTValueBuff[count] = map(subData, 0, 255, 0, 8);
            }
            xQueueSend(fftvalueQueue, (void *)&FFTValueBuff, 0);
            fft_destroy(real_fft_plan);
            //Serial.printf("mmp\r\n");
        }
        else
        {
            delay(10);
        }
    }
}

bool PDM::InitI2SSpakerOrMic(int mode)
{

    //    i2s_driver_uninstall(I2S_NUM_0);
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };
    if (mode == MODE_MIC)
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    }

    //Serial.println("Init i2s_driver_install");

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = PIN_DATA;

    //Serial.println("Init i2s_set_pin");
    i2s_set_pin(I2S_NUM_0, &pin_config);
    //Serial.println("Init i2s_set_clk");
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    return true;
}

PDM::PDM()
{
}

PDM::PDM(uint8_t clk, uint8_t data, uint16_t number, uint8_t mic)
{
    SEP_NUM = number;
    PIN_CLK = clk;
    PIN_DATA = data;
    MODE_MIC = mic;
}

PDM::~PDM()
{
}
