#include "WeatherMain.h"
#include "WeatherCSVReader.h"
#include <iostream>
#include <string>
#include <set>

WeatherMain::WeatherMain()
{
}

void WeatherMain::init()
{
    int userOption = 0;
    printHelp();
    
    while (true)
    {
        printMenu();
        userOption = getUserOption();
        processUserOption(userOption);
    }
}

void WeatherMain::printMenu()
{
    std::cout << "=====================================" << std::endl;
    std::cout << "  Weather Analysis Toolkit" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "1: Compute candlestick data" << std::endl;
    std::cout << "2: Plot candlestick data (text)" << std::endl;
    std::cout << "3: Filter and plot data" << std::endl;
    std::cout << "4: Predict temperature" << std::endl;
    std::cout << "5: Help" << std::endl;
    std::cout << "6: Exit" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "Type in 1-6" << std::endl;
}

void WeatherMain::printHelp()
{
    std::cout << "Welcome to the Weather Analysis Toolkit!" << std::endl;
    std::cout << "This program analyzes weather data and creates candlestick charts." << std::endl;
    std::cout << "Data is loaded automatically when you select analysis options." << std::endl;
    std::cout << std::endl;
}

void WeatherMain::computeCandlestickData()
{
    std::cout << "Loading weather data for analysis..." << std::endl;
    
    std::string country = getCountryFromUser();
    TimeFrame timeFrame = getTimeFrameFromUser();
    
    // Load only the data we need for this country and timeframe
    std::cout << "Processing weather data for " << country << "..." << std::endl;
    weatherData = WeatherCSVReader::readWeatherCSV("weather_data.csv");
    
    if (weatherData.empty()) {
        std::cout << "Failed to load weather data. Please check weather_data.csv exists." << std::endl;
        return;
    }
    
    std::cout << "Loaded " << weatherData.size() << " weather records." << std::endl;
    
    // Filter by country first to reduce processing
    std::vector<WeatherData> countryData = analyser.filterByCountry(weatherData, country);
    
    if (countryData.empty()) {
        std::cout << "No data found for country: " << country << std::endl;
        return;
    }
    
    std::cout << "Found " << countryData.size() << " records for " << country << std::endl;
    
    currentCandlesticks = analyser.computeCandlestickData(countryData, country, timeFrame);
    
    std::cout << "Computed " << currentCandlesticks.size() << " candlesticks for " << country << std::endl;
    
    // Show first few candlesticks
    int showCount = std::min(5, static_cast<int>(currentCandlesticks.size()));
    std::cout << "First " << showCount << " candlesticks:" << std::endl;
    for (int i = 0; i < showCount; ++i)
    {
        std::cout << currentCandlesticks[i].toString() << std::endl;
    }
}

void WeatherMain::plotCandlestickData()
{
    if (currentCandlesticks.empty())
    {
        std::cout << "Please compute candlestick data first (option 2)." << std::endl;
        return;
    }
    
    std::cout << "Text-based candlestick plot:" << std::endl;
    std::cout << "=============================" << std::endl;
    
    // Simple text plot implementation
    for (const auto& candlestick : currentCandlesticks)
    {
        std::cout << candlestick.getDate() << " ";
        
        // Simple representation: + for positive, - for negative
        if (candlestick.isPositive())
        {
            std::cout << "[+] ";
        }
        else
        {
            std::cout << "[-] ";
        }
        
        // Show range with characters
        std::cout << "Range: " << candlestick.getLow() << " |";
        
        // Body representation
        int bodySize = static_cast<int>(candlestick.getBody() * 2);
        for (int i = 0; i < bodySize && i < 10; ++i)
        {
            std::cout << (candlestick.isPositive() ? "+" : "-");
        }
        
        std::cout << "| " << candlestick.getHigh() << std::endl;
    }
}

void WeatherMain::filterAndPlotData()
{
    std::cout << "Loading weather data for filtering..." << std::endl;
    weatherData = WeatherCSVReader::readWeatherCSV("weather_data.csv");
    
    if (weatherData.empty()) {
        std::cout << "Failed to load weather data. Please check weather_data.csv exists." << std::endl;
        return;
    }
    
    std::cout << "Loaded " << weatherData.size() << " weather records." << std::endl;
    
    std::cout << "Filter options:" << std::endl;
    std::cout << "1: Filter by date range" << std::endl;
    std::cout << "2: Filter by temperature range" << std::endl;
    std::cout << "3: Filter by country" << std::endl;
    
    int choice = getUserOption();
    std::vector<WeatherData> filteredData = weatherData;
    
    switch (choice)
    {
        case 1:
        {
            std::cout << "Enter start date (YYYY-MM-DD): ";
            std::string startDate;
            std::getline(std::cin, startDate);
            std::cout << "Enter end date (YYYY-MM-DD): ";
            std::string endDate;
            std::getline(std::cin, endDate);
            filteredData = analyser.filterByDateRange(weatherData, startDate, endDate);
            break;
        }
        case 2:
        {
            std::cout << "Enter minimum temperature: ";
            double minTemp;
            std::cin >> minTemp;
            std::cout << "Enter maximum temperature: ";
            double maxTemp;
            std::cin >> maxTemp;
            std::cin.ignore(); // Clear buffer
            filteredData = analyser.filterByTemperatureRange(weatherData, minTemp, maxTemp);
            break;
        }
        case 3:
        {
            std::string country = getCountryFromUser();
            filteredData = analyser.filterByCountry(weatherData, country);
            break;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
            return;
    }
    
    std::cout << "Filtered data contains " << filteredData.size() << " records." << std::endl;
    
    // Show sample of filtered data
    int showCount = std::min(10, static_cast<int>(filteredData.size()));
    for (int i = 0; i < showCount; ++i)
    {
        std::cout << filteredData[i].getTimestamp() << " " 
                  << filteredData[i].getCountry() << " " 
                  << filteredData[i].getTemperature() << "°C" << std::endl;
    }
}

void WeatherMain::predictTemperature()
{
    std::cout << "Temperature prediction feature coming soon!" << std::endl;
    std::cout << "This will implement moving average and linear regression predictions." << std::endl;
}

int WeatherMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "user> ";
    std::getline(std::cin, line);
    
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception& e)
    {
        // Invalid input, return 0
    }
    
    return userOption;
}

void WeatherMain::processUserOption(int userOption)
{
    switch (userOption)
    {
        case 1:
            computeCandlestickData();
            break;
        case 2:
            plotCandlestickData();
            break;
        case 3:
            filterAndPlotData();
            break;
        case 4:
            predictTemperature();
            break;
        case 5:
            printHelp();
            break;
        case 6:
            std::cout << "Goodbye!" << std::endl;
            exit(0);
            break;
        default:            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

TimeFrame WeatherMain::getTimeFrameFromUser()
{
    std::cout << "Select time frame:" << std::endl;
    std::cout << "1: Hourly" << std::endl;
    std::cout << "2: Daily" << std::endl;
    std::cout << "3: Monthly" << std::endl;
    std::cout << "4: Yearly" << std::endl;
    
    int choice = getUserOption();
    
    switch (choice)
    {
        case 1: return TimeFrame::HOURLY;
        case 2: return TimeFrame::DAILY;
        case 3: return TimeFrame::MONTHLY;
        case 4: return TimeFrame::YEARLY;
        default: return TimeFrame::DAILY;
    }
}

std::string WeatherMain::getCountryFromUser()
{
    std::cout << "Enter country code (e.g., GB, FR, DE): ";
    std::string country;
    std::getline(std::cin, country);
    return country;
}
