#include "WeatherCSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

WeatherCSVReader::WeatherCSVReader()
{
}

std::vector<WeatherData> WeatherCSVReader::readWeatherCSV(std::string csvFilename)
{
    std::vector<WeatherData> weatherRecords;
    std::ifstream csvFile{csvFilename};
    std::string line;
    
    if (!csvFile.is_open())
    {
        std::cout << "WeatherCSVReader::readWeatherCSV could not open file " << csvFilename << std::endl;
        return weatherRecords;
    }
    
    // Read header line to find column indices
    std::getline(csvFile, line);
    std::vector<std::string> headers = tokenise(line, ',');
    
    // Map country codes to their temperature column indices
    std::map<std::string, int> countryColumns;
    for (size_t i = 0; i < headers.size(); ++i)
    {
        std::string header = headers[i];
        if (header.length() >= 12 && header.substr(header.length() - 11) == "temperature")
        {
            // Extract country code (e.g., "GB_temperature" -> "GB")
            std::string countryCode = header.substr(0, 2);
            countryColumns[countryCode] = static_cast<int>(i);
        }
    }
    
    std::cout << "Found temperature data for " << countryColumns.size() << " countries." << std::endl;
    
    // Read data lines
    int lineCount = 0;
    while (std::getline(csvFile, line) && lineCount < 10000) // Limit to first 10,000 records for testing
    {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = tokenise(line, ',');
        if (tokens.size() < headers.size()) continue;
        
        std::string timestamp = tokens[0]; // First column is utc_timestamp
        
        // Create WeatherData objects for each country in this timestamp
        for (const auto& pair : countryColumns)
        {
            std::string country = pair.first;
            int columnIndex = pair.second;
            
            if (columnIndex < static_cast<int>(tokens.size()))
            {
                try
                {
                    double temperature = std::stod(tokens[columnIndex]);
                    weatherRecords.emplace_back(timestamp, country, temperature);
                }
                catch (const std::exception& e)
                {
                    // Skip invalid temperature values
                }
            }
        }
        
        lineCount++;
        if (lineCount % 1000 == 0)
        {
            std::cout << "Processed " << lineCount << " lines..." << std::endl;
        }
    }
    
    std::cout << "WeatherCSVReader::readWeatherCSV read " << weatherRecords.size() << " weather records" << std::endl;
    return weatherRecords;
}

std::vector<std::string> WeatherCSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    std::stringstream ss(csvLine);
    std::string token;
    
    while (std::getline(ss, token, separator))
    {
        tokens.push_back(token);
    }
    
    return tokens;
}
