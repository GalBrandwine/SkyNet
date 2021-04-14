#pragma once
/**
 * @brief Temperature
 * Month	Min (°C)	Max (°C)	Mean (°C)
 * January	10	        18	        14	        
 * February	10	        18	        14	        
 * March	12	        20	        16	        
 * April	14	        23	        18.5	    
 * May	    17	        25	        21	        
 * June	    20	        27	        23.5        
 * July	    23	        29	        26	        
 * August	23	        30	        26.5	    
 * September	        22	        29	        
 * October	19	        27	        23	        
 * November	15	        24	        19.5	    
 * December	12          20	        16
*/
#define January_mean_temp 14
#define February_mean_temp 14
#define March_mean_temp 16
#define April_mean_temp 18.5
#define May_mean_temp 21
#define June_mean_temp 3.5
#define July_mean_temp 26
#define August_mean_temp 26.5
#define September_mean_temp 29
#define October_mean_temp 23
#define November_mean_temp 19.5
#define December_mean_temp 16

namespace Weather
{
    enum WeatherType
    {
        Automatic,
        Sunny,
        PartiallyCloudy,
        Cloudy,
        Overcast,
        Rain,
        Drizzle,
        Snow,
        Stormy,
        NoSuchWeather
    };

    const WeatherType to_weather(String weather_string)
    {
        if (weather_string == "Automatic")
            return WeatherType::Automatic;
        if (weather_string == "Sunny")
            return WeatherType::Sunny;
        if (weather_string == "PartiallyCloudy")
            return WeatherType::PartiallyCloudy;
        if (weather_string == "Cloudy")
            return WeatherType::Cloudy;
        if (weather_string == "Overcast")
            return WeatherType::Overcast;
        if (weather_string == "Rain")
            return WeatherType::Rain;
        if (weather_string == "Drizzle")
            return WeatherType::Drizzle;
        if (weather_string == "Snow")
            return WeatherType::Snow;
        if (weather_string == "Stormy")
            return WeatherType::Stormy;
        return WeatherType::NoSuchWeather;
    };

    const String to_string(WeatherType weather)
    {
        switch (weather)
        {
        case WeatherType::Automatic:
            return String("Automatic");
        case WeatherType::Sunny:
            return String("Sunny");
        case WeatherType::Cloudy:
            return String("Cloudy");
        case WeatherType::Drizzle:
            return String("Drizzle");
        case WeatherType::Overcast:
            return String("Overcast");
        case WeatherType::PartiallyCloudy:
            return String("PartiallyCloudy");
        case WeatherType::Rain:
            return String("Rain");
        case WeatherType::Snow:
            return String("Snow");
        case WeatherType::Stormy:
            return String("Stormy");
        default:
            return String("NO such weather");
        }
    };

    /**
     * @brief Rainfall:
     * 
     * Month	Millimeters	Inches	Days
     * January	    145	5.7	13
     * February	    110	4.3	11
     * March	    60	2.4	8
     * April	    15	0.6	2
     * May	        4	0.2	1
     * June	        0	0	0
     * July	        0	0	0
     * August	    0	0	0
     * September	1	0	0
     * October	    35	1.4	4
     * November	    80	3.1	7
     * December	    125	4.9	10
     * Year	        585	23	56
     * 
    */

    const WeatherType get_next_weather(const ts &timestamp)
    {

        switch (timestamp.mon)
        {
        case 1: // Januar
            return WeatherType::Rain;
            // skyNetStruct.current_weather = WeatherType::Rain;
            // skyNetStruct.current_temperature = January_mean_temp;
            // break;
        case 2:
            return WeatherType::Stormy;
            // skyNetStruct.current_weather = WeatherType::Stormy;
            // skyNetStruct.current_temperature = February_mean_temp;
            // break;
        case 3:
            return WeatherType::Overcast;
            // skyNetStruct.current_weather = WeatherType::Overcast;
            // skyNetStruct.current_temperature = March_mean_temp;
            // break;
        case 4:
            return WeatherType::PartiallyCloudy;
            // skyNetStruct.current_weather = WeatherType::PartiallyCloudy;
            // skyNetStruct.current_temperature = April_mean_temp;
            // break;
        case 5:
            return WeatherType::PartiallyCloudy;
            // skyNetStruct.current_weather = WeatherType::PartiallyCloudy;
            // skyNetStruct.current_temperature = May_mean_temp;
            // break;
        case 6:
            return WeatherType::Sunny;
            // skyNetStruct.current_weather = WeatherType::Sunny;
            // skyNetStruct.current_temperature = June_mean_temp;
            // break;
        case 7:
            return WeatherType::Sunny;
            // skyNetStruct.current_weather = WeatherType::Sunny;
            // skyNetStruct.current_temperature = July_mean_temp;
            // break;
        case 8:
            return WeatherType::Sunny;
            // skyNetStruct.current_weather = WeatherType::Sunny;
            // skyNetStruct.current_temperature = August_mean_temp;
            // break;
        case 9:
            return WeatherType::PartiallyCloudy;
            // skyNetStruct.current_weather = WeatherType::PartiallyCloudy;
            // skyNetStruct.current_temperature = September_mean_temp;
            // break;
        case 10:
            return WeatherType::Cloudy;
            // skyNetStruct.current_weather = WeatherType::Cloudy;
            // skyNetStruct.current_temperature = October_mean_temp;
            // break;
        case 11:
            return WeatherType::Rain;
            // skyNetStruct.current_weather = WeatherType::Rain;
            // skyNetStruct.current_temperature = November_mean_temp;
            // break;
        case 12:
            return WeatherType::Rain;
            // skyNetStruct.current_weather = WeatherType::Rain;
            // skyNetStruct.current_temperature = December_mean_temp;
            // break;
        default:
            return WeatherType::NoSuchWeather;
        }
    };
} // namespace Weather
