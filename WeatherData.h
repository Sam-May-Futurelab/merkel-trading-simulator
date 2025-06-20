#pragma once
#include <string>

class WeatherData
{
public:
    WeatherData(std::string timestamp,
                std::string country,
                double temperature);

    // Getters
    std::string getTimestamp() const;
    std::string getCountry() const;
    double getTemperature() const;
    
    // Extract date components
    std::string getYear() const;
    std::string getMonth() const;
    std::string getDay() const;
    std::string getHour() const;

    // Static utility functions
    static bool compareByTimestamp(const WeatherData& w1, const WeatherData& w2);

private:
    std::string timestamp;
    std::string country;
    double temperature;
};
