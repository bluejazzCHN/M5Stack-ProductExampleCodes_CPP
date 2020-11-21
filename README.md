# M5Stack-ProductExample_CPP

  Easy Learn Series @ Refactor. 
  
  The easy way to learn  C++  and M5 ESP32 programming is to refactor existing code.
  
  The original version can be donload from https://github.com/m5stack/M5-ProductExampleCodes
# Example Unit/PDM  
## Class List

### PDM Class 

   Encaplsulate M5, TFT_eSPI, FFT into class PDM. Build PDM spectrum analysor so clearly and simply.
   ```
   #include "PDM.h"

    PDM pdm;

    void setup()
    {
      pdm.header("PDM Unit", BLACK);
      pdm.microPhoneSetup();
    }

    void loop()
    {
      pdm.microPhoneFFT();
    }
   ```
   Hardware requirement: M5Stack core,PDM microphone
# Example M5_lvgl_Demo  
  Migrate Lvgl example to M5Core2
# Example Atom_rainbow
  Neopixel in M5Atom
# Example InkCore
  inkClock in InkCore with two Timers to update NTP and RTC
# Original Author
    
  songjiangzhang@hotmail.com
  

