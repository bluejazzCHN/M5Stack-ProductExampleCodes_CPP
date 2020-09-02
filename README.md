# M5Stack-ProductExample_CPP

  Easy Learn Series @ Refactor. 
  
  The easy way to learn  C++  and M5 ESP32 programming is to refactor existing code.
  
  The original version can be donload from https://github.com/m5stack/M5-ProductExampleCodes
  
## Class List

### 1. class PDM 

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
   
  ## Original Author
    
    songjiangzhang@hotmail.com
  

