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
  ![](https://github.com/bluejazzCHN/M5Stack-ProductExampleCodes_CPP/blob/master/InkCore/InkClock.jpg)
# TFT_eSPI on M5Stack Core2
  
  How to use native TFT_eSPI in Core2?
  
  ### Step1: TFT_eSPI.h include User_setup_Select.h to find TFT drivers
  
  ```
  /***************************************************************************************
  **                         Section 2: Load library and processor specific header files
  ***************************************************************************************/
  // Include header file that defines the fonts loaded, the TFT drivers
  // available and the pins to be used, etc, etc
  #include <User_Setup_Select.h>  
  ```
  ### Step2: Modify User_Setup_Select.h
  
      *Disable default pin setting (#include <User_Setup.h>)
      ```
      // Only ONE line below should be uncommented.  Add extra lines and files as needed.

      //#include <User_Setup.h>           // Default setup is root library folder
      ```
      
      *Enable M5Stack setting (#include <setup12_M5Stack.h>)
      ```
      //#include <User_Setups/Setup11_RPi_touch_ILI9486.h> // Setup file configured for ESP32 and RPi TFT with touch
      #include <User_Setups/Setup12_M5Stack.h>           // Setup file for the ESP32 based M5Stack
      //#include <User_Setups/Setup13_ILI9481_Parallel.h>  // Setup file for the ESP32 with parallel bus TFT
      //#include <User_Setups/Setup14_ILI9341_Parallel.h>  // Setup file for the ESP32 with parallel bus TFT
      ```
      
      *Confirm below highlight that is used by core2
      ```
      // Load the right driver definition - do not tinker here !
      #if   defined (ILI9341_DRIVER)
        #include <TFT_Drivers/ILI9341_Defines.h>
        #define  TFT_DRIVER 0x9341
      ```
  ### Step3: Copy pin setting from In_eSPI_Setup.h to Setup12_M5Stack.h
      ```
      // For the M5Stack module use these #define lines
      #define TFT_MISO 38
      #define TFT_MOSI 23
      #define TFT_SCLK 18
      #define TFT_CS   5      // Chip select control pin
      #define TFT_DC   15     // Data Command control pin
      #define TFT_RST  -1     // Reset pin (could connect to Arduino RESET pin)
      #define TFT_BL   -1     // LED back-light (required for M5Stack)
      ```
  ### Step4: run TFT_eSPI [viewport demo](https://github.com/Bodmer/TFT_eSPI/blob/master/examples/Generic/Viewport_Demo/Viewport_Demo.ino)
  
  
# Original Author
    
  songjiangzhang@hotmail.com
  

