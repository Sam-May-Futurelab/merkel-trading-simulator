#include "WeatherMain.h"
#include "WeatherCSVReader.h"
#include <iostream>
#include <string>
#include <set>
#include <iomanip>
#include <algorithm>

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
        std::cout << "Please compute candlestick data first (option 1)." << std::endl;
        return;
    }
    
    std::cout << "\nSimple ASCII Candlestick Chart:" << std::endl;
    std::cout << "===============================\n" << std::endl;
    
    // Limit to first 20 candlesticks for readability
    size_t maxCandles = std::min(static_cast<size_t>(20), currentCandlesticks.size());
    
    // Find temperature range for scaling
    double minTemp = currentCandlesticks[0].getLow();
    double maxTemp = currentCandlesticks[0].getHigh();
    
    for (size_t i = 0; i < maxCandles; ++i) {
        const auto& candle = currentCandlesticks[i];
        minTemp = std::min(minTemp, candle.getLow());
        maxTemp = std::max(maxTemp, candle.getHigh());
    }
    
    const int chartHeight = 15;
    double tempRange = maxTemp - minTemp;
    if (tempRange == 0) tempRange = 1; // Avoid division by zero
    
    std::cout << "Temperature Range: " << std::fixed << std::setprecision(1) 
              << minTemp << "C to " << maxTemp << "C" << std::endl;
    std::cout << "Date        Chart                              Open  High  Low   Close Trend" << std::endl;
    std::cout << "----------- ---------------------------------- ----- ----- ----- ----- -----" << std::endl;
    
    // Create simple horizontal chart for each candlestick
    for (size_t i = 0; i < maxCandles; ++i) {
        const auto& candle = currentCandlesticks[i];
        
        // Print date (first 11 chars)
        std::string date = candle.getDate();
        if (date.length() > 11) date = date.substr(0, 11);
        std::cout << std::setw(11) << std::left << date << " ";
        
        // Create 34-character wide chart
        std::string chart(34, ' ');
        
        // Calculate positions (0 to 33)
        double range = maxTemp - minTemp;
        if (range == 0) range = 1;
        
        int lowPos = static_cast<int>((candle.getLow() - minTemp) / range * 33);
        int highPos = static_cast<int>((candle.getHigh() - minTemp) / range * 33);
        int openPos = static_cast<int>((candle.getOpen() - minTemp) / range * 33);
        int closePos = static_cast<int>((candle.getClose() - minTemp) / range * 33);
        
        // Ensure positions are within bounds
        lowPos = std::max(0, std::min(33, lowPos));
        highPos = std::max(0, std::min(33, highPos));
        openPos = std::max(0, std::min(33, openPos));
        closePos = std::max(0, std::min(33, closePos));
        
        // Draw the wick (line from low to high)
        for (int j = lowPos; j <= highPos; ++j) {
            chart[j] = '-';
        }
        
        // Draw the body (between open and close)
        int bodyStart = std::min(openPos, closePos);
        int bodyEnd = std::max(openPos, closePos);
        
        char bodyChar = candle.isPositive() ? '#' : 'O';
        for (int j = bodyStart; j <= bodyEnd; ++j) {
            chart[j] = bodyChar;
        }
        
        // Mark exact points
        if (lowPos < 34) chart[lowPos] = 'L';
        if (highPos < 34) chart[highPos] = 'H';
        if (openPos < 34 && chart[openPos] == ' ') chart[openPos] = '[';
        if (closePos < 34 && chart[closePos] == ' ') chart[closePos] = ']';
        
        // Print the chart and values
        std::cout << chart << " ";
        std::cout << std::setw(5) << std::fixed << std::setprecision(1) << candle.getOpen() << " ";
        std::cout << std::setw(5) << candle.getHigh() << " ";
        std::cout << std::setw(5) << candle.getLow() << " ";
        std::cout << std::setw(5) << candle.getClose() << " ";
        std::cout << (candle.isPositive() ? " UP " : "DOWN") << std::endl;
    }
    
    std::cout << "\nLegend:" << std::endl;    std::cout << "- = Temperature range (wick)" << std::endl;
    std::cout << "# = Rising candle body (Close > Open)" << std::endl;
    std::cout << "O = Falling candle body (Close < Open)" << std::endl;
    std::cout << "L = Low point, H = High point" << std::endl;
    std::cout << "[ = Open, ] = Close" << std::endl;
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
    
    // Debug: Show what we actually read
    std::cout << "Read: '" << line << "'" << std::endl;
    
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception& e)
    {
        // Invalid input, return 0
        std::cout << "Invalid input, returning 0" << std::endl;
    }
    
    return userOption;
}

void WeatherMain::processUserOption(int userOption)
{
    std::cout << "Processing option: " << userOption << std::endl;
    
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
        default:
            std::cout << "Invalid choice: " << userOption << ". Please try again." << std::endl;
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


