#include "Candlestick.h"
#include <sstream>
#include <iomanip>
#include <cmath>

Candlestick::Candlestick(std::string date,
                        double open,
                        double high,
                        double low,
                        double close)
    : date(date), open(open), high(high), low(low), close(close)
{
}

std::string Candlestick::getDate() const
{
    return date;
}

double Candlestick::getOpen() const
{
    return open;
}

double Candlestick::getHigh() const
{
    return high;
}

double Candlestick::getLow() const
{
    return low;
}

double Candlestick::getClose() const
{
    return close;
}

bool Candlestick::isPositive() const
{
    return close > open;
}

double Candlestick::getRange() const
{
    return high - low;
}

double Candlestick::getBody() const
{
    return std::abs(close - open);
}

std::string Candlestick::toString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << date << " Open: " << open 
        << " High: " << high 
        << " Low: " << low 
        << " Close: " << close;
    return oss.str();
}
