#pragma once
#include <string>

class Candlestick
{
public:
    Candlestick(std::string date,
                double open,
                double high,
                double low,
                double close);

    // Getters
    std::string getDate() const;
    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;
    
    // Utility functions
    bool isPositive() const; // Close > Open (green candlestick)
    double getRange() const; // High - Low
    double getBody() const;  // |Close - Open|
    
    // String representation
    std::string toString() const;

private:
    std::string date;
    double open;
    double high;
    double low;
    double close;
};
