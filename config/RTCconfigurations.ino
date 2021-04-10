#include "/home/gal/dev/SkyNet/include/settings.h"
#include "/home/gal/dev/SkyNet/include/RTCsettings.h"

/*----------------------------------------------------------------------------
  In order to synchronies your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
const int RTCsettingHour = 17;
const int RTCsettingMin = 25;
const int RTCsettingSec = 0;
const int RTCsettingMonthday = 7;
const int RTCsettingMonth = 4;
const int RTCsettingYear = 2021;

void setup()
{
  Serial.begin(115200);
  // RTC initiations
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  rtc_timestamp.hour = RTCsettingHour;
  rtc_timestamp.min = RTCsettingMin;
  rtc_timestamp.sec = RTCsettingSec;
  rtc_timestamp.mday = RTCsettingMonthday;
  rtc_timestamp.mon = RTCsettingMonth;
  rtc_timestamp.year = RTCsettingYear;
  DS3231_set(rtc_timestamp);
}

void loop()
{
  DS3231_get(&rtc_timestamp);
  Serial.print("Date : ");
  Serial.print(rtc_timestamp.mday);
  Serial.print("/");
  Serial.print(rtc_timestamp.mon);
  Serial.print("/");
  Serial.print(rtc_timestamp.year);
  Serial.print("\t Hour : ");
  Serial.print(rtc_timestamp.hour);
  Serial.print(":");
  Serial.print(rtc_timestamp.min);
  Serial.print(".");
  Serial.println(rtc_timestamp.sec);

  delay(1000);
}