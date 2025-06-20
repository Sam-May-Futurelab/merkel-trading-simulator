#pragma once
#include "WeatherData.h"
#include <vector>
#include <string>

class WeatherCSVReader
{
public:
    WeatherCSVReader();

    // Read weather data from CSV file
    static std::vector<WeatherData> readWeatherCSV(std::string csvFile);
    
    // Tokenize CSV line
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
    // Convert strings to WeatherData object
    static WeatherData stringsToWeatherData(std::string timestamp,
                                           std::string country,
                                           std::string temperature);

private:
    // Convert vector of strings to WeatherData
    static WeatherData stringsToWeatherData(std::vector<std::string> strings);
};
