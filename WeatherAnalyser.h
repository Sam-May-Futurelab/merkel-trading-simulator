#pragma once
#include "WeatherData.h"
#include "Candlestick.h"
#include <vector>
#include <string>
#include <map>

enum class TimeFrame {
    HOURLY,
    DAILY,
    MONTHLY,
    YEARLY
};

class WeatherAnalyser
{
public:
    WeatherAnalyser();
    
    // Task 1: Compute candlestick data
    std::vector<Candlestick> computeCandlestickData(const std::vector<WeatherData>& weatherData,
                                                   const std::string& country,
                                                   TimeFrame timeFrame);
    
    // Filter data by various criteria
    std::vector<WeatherData> filterByCountry(const std::vector<WeatherData>& data, 
                                            const std::string& country);
    
    std::vector<WeatherData> filterByDateRange(const std::vector<WeatherData>& data,
                                              const std::string& startDate,
                                              const std::string& endDate);
                                              
    std::vector<WeatherData> filterByTemperatureRange(const std::vector<WeatherData>& data,
                                                     double minTemp,
                                                     double maxTemp);
    
    // Statistical functions for candlestick computation
    double calculateMean(const std::vector<double>& temperatures);
    double findMax(const std::vector<double>& temperatures);
    double findMin(const std::vector<double>& temperatures);
    
    // Group data by time frame
    std::map<std::string, std::vector<WeatherData>> groupByTimeFrame(const std::vector<WeatherData>& data,
                                                                    TimeFrame timeFrame);

private:
    // Helper functions
    std::string getTimeFrameKey(const WeatherData& wd, TimeFrame timeFrame);
    std::string timeFrameToString(TimeFrame tf);
};
