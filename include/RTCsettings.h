#ifndef HEADERFILE_H
#define HEADERFILE_H
// Setting today's time and date.
#include <Wire.h>
#include <ds3231.h>

/**
 * @brief Convert timestamp to string.
 * Separator will be ":"
 * 
 * @param[in] t 
 * @param[out] time_string 
 */
void time_to_string(const ts &t, String &time_string)
{
  time_string = (int)t.hour;
  time_string += ":";
  time_string += (int)t.min;
  time_string += ":";
  time_string += (int)t.sec;
  time_string += "_";
  time_string += (int)t.mday;
  time_string += "/";
  time_string += (int)t.mon;
  time_string += "/";
  time_string += (int)t.year;
}
#endif HEADERFILE_H