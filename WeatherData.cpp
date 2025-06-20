#include "WeatherData.h"
#include <iostream>
#include <sstream>

WeatherData::WeatherData(std::string timestamp,
                        std::string country,
                        double temperature)
    : timestamp(timestamp), country(country), temperature(temperature)
{
}

std::string WeatherData::getTimestamp() const
{
    return timestamp;
}

std::string WeatherData::getCountry() const
{
    return country;
}

double WeatherData::getTemperature() const
{
    return temperature;
}

std::string WeatherData::getYear() const
{
    // Extract year from timestamp format: YYYY-MM-DD HH:MM
    return timestamp.substr(0, 4);
}

std::string WeatherData::getMonth() const
{
    // Extract month from timestamp format: YYYY-MM-DD HH:MM
    return timestamp.substr(5, 2);
}

std::string WeatherData::getDay() const
{
    // Extract day from timestamp format: YYYY-MM-DD HH:MM
    return timestamp.substr(8, 2);
}

std::string WeatherData::getHour() const
{
    // Extract hour from timestamp format: YYYY-MM-DD HH:MM
    return timestamp.substr(11, 2);
}

bool WeatherData::compareByTimestamp(const WeatherData& w1, const WeatherData& w2)
{
    return w1.getTimestamp() < w2.getTimestamp();
}
