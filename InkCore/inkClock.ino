#include "M5CoreInk.h"
#include <Ticker.h>
#include <WiFi.h>
#include "ezTime.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);
const char *ssid = "ZSJ_HOME";
const char *password = "QQqq11!!";
Timezone myTZ;

RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

char timeStrbuff[32];
char dateStrbuff[32];

Ticker inkDatefresh;
Ticker inkDisplayfresh;

void setDatebuff()
{
  M5.rtc.GetData(&RTCDate);
  sprintf(dateStrbuff, "Date == %d/%02d/%02d",
          RTCDate.Year, RTCDate.Month, RTCDate.Date);
}

void setTimebuff()
{
  M5.rtc.GetTime(&RTCtime);
  sprintf(timeStrbuff, "Time == %02d:%02d:%02d",
          RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
}
void changeTimeSecondValue()
{
  RTCtime.Seconds = (RTCtime.Seconds + 15) % 60;
  M5.rtc.SetTime(&RTCtime);
  setTimebuff();
}

void changeTimeMinuteValue()
{
  RTCtime.Minutes = (RTCtime.Minutes + 1) % 60;
  RTCtime.Seconds = 00;
  M5.rtc.SetTime(&RTCtime);
  setTimebuff();
}

void changeTimeHourValue()
{
  RTCtime.Hours = (RTCtime.Hours + 1) % 24;
  RTCtime.Seconds = 00;
  M5.rtc.SetTime(&RTCtime);
  setTimebuff();
}

void flushTimebuff()
{
  setDatebuff();
  setTimebuff();
}
void refreshDisplay()
{
  InkPageSprite.drawString(20, 100, dateStrbuff);
  InkPageSprite.drawString(20, 120, timeStrbuff);
  InkPageSprite.pushSprite();
}

void syncUpTime()
{
  waitForSync();
  myTZ.setLocation(F("Asia/Shanghai"));
  RTCtime.Hours = (uint16_t)myTZ.hour();
  RTCtime.Minutes = (uint16_t)myTZ.minute();
  RTCtime.Seconds = (uint16_t)myTZ.second();
  M5.rtc.SetTime(&RTCtime);

  RTCDate.Year = (uint16_t)myTZ.year();
  RTCDate.Month = (uint16_t)myTZ.month();
  RTCDate.Date = (uint16_t)myTZ.day();
  M5.rtc.SetData(&RTCDate);
}

void setup()
{

  M5.begin();
  if (!M5.M5Ink.isInit())
  {
    Serial.printf("Ink Init faild");
    while (1)
      delay(100);
  }
  M5.M5Ink.clear();
  delay(1000);

  //creat ink refresh Sprite
  if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
  {
    Serial.printf("Ink Sprite creat faild");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  myTZ.setLocation(F("Asia/Shanghai"));

  syncUpTime();

  inkDatefresh.attach(1, flushTimebuff);
  inkDisplayfresh.attach(15, refreshDisplay);

  InkPageSprite.drawString(40, 40, "Clock", &AsciiFont24x48);

  InkPageSprite.pushSprite();
}

void loop()
{
  M5.update();
  if (M5.BtnUP.wasPressed())
  {
    inkDatefresh.detach();
    inkDisplayfresh.detach();
    do
    {
      changeTimeHourValue();
      delay(1000);
      flushTimebuff();
      refreshDisplay();
    } while (M5.BtnUP.read());
    inkDatefresh.attach(1, flushTimebuff);
    inkDisplayfresh.attach(15, refreshDisplay);
  }
  if (M5.BtnDOWN.wasPressed())
  {
    inkDatefresh.detach();
    inkDisplayfresh.detach();
    do
    {
      changeTimeMinuteValue();
      delay(1000);
      flushTimebuff();
      refreshDisplay();
    } while (M5.BtnDOWN.read());
    inkDatefresh.attach(1, flushTimebuff);
    inkDisplayfresh.attach(15, refreshDisplay);
  }
  if (M5.BtnMID.wasPressed())
  {
    inkDatefresh.detach();
    inkDisplayfresh.detach();
    do
    {
      syncUpTime();
      delay(1000);
      flushTimebuff();
      refreshDisplay();
    } while (M5.BtnMID.read());
    inkDatefresh.attach(1, flushTimebuff);
    inkDisplayfresh.attach(15, refreshDisplay);
  }
  delay(500);
}