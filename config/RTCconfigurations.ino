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

SkyNetStruct sky_net;

void setup()
{
  Serial.begin(115200);
  // RTC initiations
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  sky_net.rtc_timestamp.hour = RTCsettingHour;
  sky_net.rtc_timestamp.min = RTCsettingMin;
  sky_net.rtc_timestamp.sec = RTCsettingSec;
  sky_net.rtc_timestamp.mday = RTCsettingMonthday;
  sky_net.rtc_timestamp.mon = RTCsettingMonth;
  sky_net.rtc_timestamp.year = RTCsettingYear;
  DS3231_set(sky_net.rtc_timestamp);
}

void loop()
{
  DS3231_get(&sky_net.rtc_timestamp);
  Serial.print("Date : ");
  Serial.print(sky_net.rtc_timestamp.mday);
  Serial.print("/");
  Serial.print(sky_net.rtc_timestamp.mon);
  Serial.print("/");
  Serial.print(sky_net.rtc_timestamp.year);
  Serial.print("\t Hour : ");
  Serial.print(sky_net.rtc_timestamp.hour);
  Serial.print(":");
  Serial.print(sky_net.rtc_timestamp.min);
  Serial.print(".");
  Serial.println(sky_net.rtc_timestamp.sec);

  delay(1000);
}