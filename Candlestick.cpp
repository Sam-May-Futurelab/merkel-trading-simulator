#include "Candlestick.h"
#include <sstream>
#include <iomanip>
#include <cmath>

// Constructor using member initializer list (efficient initialization)
Candlestick::Candlestick(std::string date, double open, double high, double low, double close)
    : date(date), open(open), high(high), low(low), close(close) {}

// Simple getter implementations - inline would be more efficient but kept separate for clarity
std::string Candlestick::getDate() const { return date; }
double Candlestick::getOpen() const { return open; }
double Candlestick::getHigh() const { return high; }
double Candlestick::getLow() const { return low; }
double Candlestick::getClose() const { return close; }

// Utility methods for technical analysis
bool Candlestick::isPositive() const { return close > open; }  // Green/bullish candle
double Candlestick::getRange() const { return high - low; }    // Price volatility
double Candlestick::getBody() const { return std::abs(close - open); }  // Body size

// Formatted string output for display/debugging
std::string Candlestick::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << date << " O:" << open << " H:" << high << " L:" << low << " C:" << close;
    return oss.str();
}
