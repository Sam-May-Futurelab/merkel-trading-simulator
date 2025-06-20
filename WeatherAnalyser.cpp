#include "WeatherAnalyser.h"
#include <algorithm>
#include <numeric>
#include <iostream>

WeatherAnalyser::WeatherAnalyser()
{
}

// WRITTEN WITHOUT ASSISTANCE - Core candlestick computation algorithm
std::vector<Candlestick> WeatherAnalyser::computeCandlestickData(const std::vector<WeatherData>& weatherData,
                                                               const std::string& country,
                                                               TimeFrame timeFrame)
{
    std::vector<Candlestick> candlesticks;
    
    // Filter data for specified country and group by time periods
    auto countryData = filterByCountry(weatherData, country);
    auto groupedData = groupByTimeFrame(countryData, timeFrame);
    
    double previousClose = 0.0;
    bool isFirstPeriod = true;
    
    // Process each time period to create candlestick data
    for (const auto& [periodKey, periodData] : groupedData) {
        if (periodData.empty()) continue;
        
        // Extract temperature values for statistical calculations
        std::vector<double> temps;
        for (const auto& wd : periodData) {
            temps.push_back(wd.getTemperature());
        }
        
        // Calculate OHLC values: Open, High, Low, Close
        double high = findMax(temps);               // Maximum temperature in period
        double low = findMin(temps);                // Minimum temperature in period  
        double close = calculateMean(temps);        // Average temperature (close)
        double open = isFirstPeriod ? close : previousClose;  // Previous period's close
        
        // Create and store candlestick
        candlesticks.emplace_back(periodKey, open, high, low, close);
        
        // Update state for next iteration
        previousClose = close;
        isFirstPeriod = false;
    }
    
    return candlesticks;
}

// WRITTEN WITHOUT ASSISTANCE - Data filtering functions for weather analysis
std::vector<WeatherData> WeatherAnalyser::filterByCountry(const std::vector<WeatherData>& data, 
                                                        const std::string& country)
{
    std::vector<WeatherData> filtered;
    
    for (const auto& wd : data)
    {
        if (wd.getCountry() == country)
        {
            filtered.push_back(wd);
        }
    }
    
    return filtered;
}

std::vector<WeatherData> WeatherAnalyser::filterByDateRange(const std::vector<WeatherData>& data,
                                                          const std::string& startDate,
                                                          const std::string& endDate)
{
    std::vector<WeatherData> filtered;
    
    for (const auto& wd : data)
    {
        std::string timestamp = wd.getTimestamp();
        if (timestamp >= startDate && timestamp <= endDate)
        {
            filtered.push_back(wd);
        }
    }
    
    return filtered;
}

std::vector<WeatherData> WeatherAnalyser::filterByTemperatureRange(const std::vector<WeatherData>& data,
                                                                 double minTemp,
                                                                 double maxTemp)
{
    std::vector<WeatherData> filtered;
    
    for (const auto& wd : data)
    {
        double temp = wd.getTemperature();
        if (temp >= minTemp && temp <= maxTemp)
        {
            filtered.push_back(wd);
        }
    }
    
    return filtered;
}

// WRITTEN WITHOUT ASSISTANCE - Statistical helper functions for candlestick computation
double WeatherAnalyser::calculateMean(const std::vector<double>& temperatures) {
    return temperatures.empty() ? 0.0 : 
           std::accumulate(temperatures.begin(), temperatures.end(), 0.0) / temperatures.size();
}

double WeatherAnalyser::findMax(const std::vector<double>& temperatures) {
    return temperatures.empty() ? 0.0 : 
           *std::max_element(temperatures.begin(), temperatures.end());
}

double WeatherAnalyser::findMin(const std::vector<double>& temperatures) {
    return temperatures.empty() ? 0.0 : 
           *std::min_element(temperatures.begin(), temperatures.end());
}

std::map<std::string, std::vector<WeatherData>> WeatherAnalyser::groupByTimeFrame(const std::vector<WeatherData>& data,
                                                                                TimeFrame timeFrame)
{
    std::map<std::string, std::vector<WeatherData>> grouped;
    
    for (const auto& wd : data)
    {
        std::string key = getTimeFrameKey(wd, timeFrame);
        grouped[key].push_back(wd);
    }
    
    return grouped;
}

std::string WeatherAnalyser::getTimeFrameKey(const WeatherData& wd, TimeFrame timeFrame)
{
    switch (timeFrame)
    {
        case TimeFrame::HOURLY:
            return wd.getTimestamp().substr(0, 13); 
        case TimeFrame::DAILY:
            return wd.getTimestamp().substr(0, 10); 
        case TimeFrame::MONTHLY:
            return wd.getYear() + "-" + wd.getMonth(); 
        case TimeFrame::YEARLY:
            return wd.getYear();
        default:
            return wd.getTimestamp().substr(0, 10);
    }
}

std::string WeatherAnalyser::timeFrameToString(TimeFrame tf)
{
    switch (tf)
    {
        case TimeFrame::HOURLY: return "Hourly";
        case TimeFrame::DAILY: return "Daily";
        case TimeFrame::MONTHLY: return "Monthly";
        case TimeFrame::YEARLY: return "Yearly";
        default: return "Unknown";
    }
}
