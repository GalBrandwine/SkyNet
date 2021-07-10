#ifndef HEADERFILE_H
#define HEADERFILE_H
// Setting today's time and date.
#include <Wire.h>
#include <ds3231.h>

namespace TimeParsing
{
    /**
 * @brief Convert timestamp to string.
 * Separator will be ":"
 * 
 * @param[in] t 
 * @param[out] time_string 
 */
    void to_string(const ts &t, String &time_string)
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

    /**
      * @brief Expected date_substring separator will be "/" and in format day/mon/year
   * 
   * @param[in] date_string 
   * @param[out] time 
   */
    void date_string_to_date(String date_string, ts &time)
    {
        uint8_t temp = 0;
        int num_of_digit = 0;
        char *pch = (char *)date_string.c_str();

        Serial.printf("Parsing Date:\n");
        Serial.printf(date_string.c_str());

        /**
         * @brief Advance to Time_Date seperator
         * 
         */
        while (*pch != '_')
        {
            pch++;
        }
        pch++;

        Serial.printf("\n");
        Serial.printf("Parsing days: \n");
        while (*pch != '/')
        {
            if (num_of_digit++ >= 1)
            {
                temp *= 10;
            }
            temp += uint8_t(*pch - '0');
            pch++;
        }
        time.mday = temp;
        Serial.print(temp);
        Serial.print(" ");
        pch++;
        temp = 0;
        num_of_digit = 0;

        Serial.printf("\n");
        Serial.printf("Parsing months: \n");
        while (*pch != '/')
        {
            if (num_of_digit++ >= 1)
            {
                temp *= 10;
            }
            temp += uint8_t(*pch - '0');
            pch++;
        }
        time.mon = temp;
        Serial.print(temp);
        Serial.print(" ");
        pch++;
        temp = 0;
        num_of_digit = 0;

        int16_t years = 0;
        Serial.printf("\n");
        Serial.printf("Parsing Years: \n");
        while (*pch != 0)
        {
            if (num_of_digit++ >= 1)
            {
                years *= 10;
            }
            years += int16_t(*pch - '0');
            pch++;
        }
        time.year = years;
        Serial.print(years);
        Serial.print(" ");
        // int seperator_counter = 0;
        // int temp, temp2;

        // Serial.printf("Parsing Date:\n");
        // Serial.printf(date_string);
        // Serial.printf("\n");

        // char *command = strtok(date_string, "/");
        // while (command != 0)
        // {

        //     // Serial.printf(command);
        //     // Serial.printf("\n");

        //     /**
        // * @brief Seconds has maximum of 2 digits
        // *
        // */
        //     temp = uint8_t(*command) - '0';
        //     temp2 = (uint8_t(*(command + 1)) - '0');

        //     if (temp2 >= 0)
        //     {
        //         temp *= 10;
        //         temp += temp2;
        //         Serial.printf("Parsing two digit number: %i.\n", temp);
        //     }
        //     else
        //     {
        //         Serial.printf("Parsing single digit number: %i.\n", temp);
        //     }

        //     switch (seperator_counter++)
        //     {
        //     case 0:
        //         Serial.printf("Parsing day\n");
        //         time.mday = temp;
        //         break;
        //     case 1:
        //         Serial.printf("Parsing monnth\n");
        //         time.mon = temp;
        //         break;
        //     case 2:
        //         Serial.printf("Parsing year\n");
        //         time.year = temp;
        //         break;
        //     default:
        //         break;
        //     }

        //     command = strtok(0, "/");
        // }
    }

    /**
   * @brief Expected date_substring separator will be ":" in format hr:min:sec
   * 
   * @param[in] time_string 
   * @param[out] time 
   */
    void time_string_to_time(String time_string, ts &time)
    {

        uint8_t temp = 0;
        int num_of_digit = 0;
        char *pch = (char *)time_string.c_str();

        Serial.printf("Parsing Time:\n");
        Serial.printf(time_string.c_str());

        Serial.printf("\n");
        Serial.printf("Parsing hours: \n");
        while (*pch != ':')
        {
            if (num_of_digit++ >= 1)
            {
                temp *= 10;
            }
            temp += uint8_t(*pch - '0');
            pch++;
        }
        time.hour = temp;
        Serial.print(temp);
        Serial.print(" ");
        pch++;

        num_of_digit = 0;
        temp = 0;

        Serial.printf("\n");
        Serial.printf("Parsing minutes: \n");
        while (*pch != ':')
        {
            if (num_of_digit++ >= 1)
            {
                temp *= 10;
            }
            temp += uint8_t(*pch - '0');
            pch++;
        }
        time.min = temp;
        Serial.print(temp);
        Serial.print(" ");
        pch++;

        num_of_digit = 0;
        temp = 0;
        Serial.printf("\n");
        Serial.printf("Parsing seconds: \n");
        while (*pch != '_')
        {
            if (num_of_digit++ >= 1)
            {
                temp *= 10;
            }
            temp += uint8_t(*pch - '0');
            pch++;
        }
        time.sec = temp;
        Serial.print(temp);
        Serial.printf(" \n");
    }

    /**
   * @brief Convert string to timestamp.
   * 
   * A string example should look like this: "21:43:38_9/7/2021"
   * 
   * So using a "_" seperator, two substirng will be received
   * * Expected time_substring separator will be ":" in format hr:min:sec
   * * Expected date_substring separator will be "/" in format day/month/year
   * 
   * @param[in] time_string 
   * @param[out] time
   */
    void to_time(const String &time_string, ts &time)
    {
        time_string_to_time(time_string, time);
        date_string_to_date(time_string, time);
    }
} // namespace TimeParsing

#endif HEADERFILE_H