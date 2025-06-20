#pragma once
#include <string>

/**
 * Represents a candlestick data point for weather analysis
 * Adapts traditional OHLC (Open, High, Low, Close) financial concept to temperature data
 * Open: Previous period's average temperature
 * High: Maximum temperature in current period  
 * Low: Minimum temperature in current period
 * Close: Average temperature in current period
 */
class Candlestick
{
public:
    // Constructor with all OHLC values
    Candlestick(std::string date, double open, double high, double low, double close);

    // Data accessors
    std::string getDate() const;
    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;
    
    // Technical analysis utilities
    bool isPositive() const; // Temperature trend: Close > Open (warming)
    double getRange() const; // Temperature volatility: High - Low
    double getBody() const;  // Temperature change magnitude: |Close - Open|
    
    // Output formatting
    std::string toString() const;

private:
    std::string date;  // Time period identifier
    double open;       // Opening temperature (previous close)
    double high;       // Maximum temperature in period
    double low;        // Minimum temperature in period
    double close;      // Closing temperature (period average)
};
