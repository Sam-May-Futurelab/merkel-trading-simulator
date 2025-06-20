#include "WeatherAnalyser.h"
#include <algorithm>
#include <numeric>
#include <iostream>

WeatherAnalyser::WeatherAnalyser()
{
}

std::vector<Candlestick> WeatherAnalyser::computeCandlestickData(const std::vector<WeatherData>& weatherData,
                                                               const std::string& country,
                                                               TimeFrame timeFrame)
{
    std::vector<Candlestick> candlesticks;
    
    // First filter by country
    std::vector<WeatherData> countryData = filterByCountry(weatherData, country);
    
    // Group data by time frame
    std::map<std::string, std::vector<WeatherData>> groupedData = groupByTimeFrame(countryData, timeFrame);
    
    // Convert grouped data to candlesticks
    std::string previousPeriodKey = "";
    double previousClose = 0.0;
    
    for (const auto& pair : groupedData)
    {
        const std::string& periodKey = pair.first;
        const std::vector<WeatherData>& periodData = pair.second;
        
        if (periodData.empty()) continue;
        
        // Extract temperatures for this period
        std::vector<double> temperatures;
        for (const auto& wd : periodData)
        {
            temperatures.push_back(wd.getTemperature());
        }
        
        // Calculate candlestick values
        double high = findMax(temperatures);
        double low = findMin(temperatures);
        double close = calculateMean(temperatures);
        
        // Open is the close of the previous period (or current close for first period)
        double open = (previousPeriodKey.empty()) ? close : previousClose;
        
        // Create candlestick
        Candlestick candlestick(periodKey, open, high, low, close);
        candlesticks.push_back(candlestick);
        
        // Update for next iteration
        previousPeriodKey = periodKey;
        previousClose = close;
    }
    
    return candlesticks;
}

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

double WeatherAnalyser::calculateMean(const std::vector<double>& temperatures)
{
    if (temperatures.empty()) return 0.0;
    
    double sum = std::accumulate(temperatures.begin(), temperatures.end(), 0.0);
    return sum / temperatures.size();
}

double WeatherAnalyser::findMax(const std::vector<double>& temperatures)
{
    if (temperatures.empty()) return 0.0;
    
    return *std::max_element(temperatures.begin(), temperatures.end());
}

double WeatherAnalyser::findMin(const std::vector<double>& temperatures)
{
    if (temperatures.empty()) return 0.0;
    
    return *std::min_element(temperatures.begin(), temperatures.end());
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
            return wd.getTimestamp().substr(0, 13); // YYYY-MM-DD HH
        case TimeFrame::DAILY:
            return wd.getTimestamp().substr(0, 10); // YYYY-MM-DD
        case TimeFrame::MONTHLY:
            return wd.getYear() + "-" + wd.getMonth(); // YYYY-MM
        case TimeFrame::YEARLY:
            return wd.getYear(); // YYYY
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
