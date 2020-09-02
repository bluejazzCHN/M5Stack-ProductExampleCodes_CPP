//Remove the M5GO base when using this example, otherwise it will not work properly,PDM Unit connected PORT-A
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
