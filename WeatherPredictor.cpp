#include "WeatherPredictor.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>

WeatherPredictor::Prediction WeatherPredictor::simpleMovingAverage(const std::vector<WeatherData>& data, int periods)
{
    if (data.size() < static_cast<size_t>(periods)) {
        return Prediction(0.0, 0.0, "SMA", "Insufficient data for Simple Moving Average");
    }
    
    std::vector<double> temperatures = extractTemperatures(data);
    
    // Calculate average of last 'periods' values
    double sum = 0.0;
    for (int i = temperatures.size() - periods; i < static_cast<int>(temperatures.size()); ++i) {
        sum += temperatures[i];
    }
    
    double prediction = sum / periods;
    double confidence = std::min(0.9, static_cast<double>(periods) / 10.0); // Higher confidence with more periods
    
    std::ostringstream desc;
    desc << "Average of last " << periods << " temperature readings";
    
    return Prediction(prediction, confidence, "SMA", desc.str());
}

WeatherPredictor::Prediction WeatherPredictor::weightedMovingAverage(const std::vector<WeatherData>& data, int periods)
{
    if (data.size() < static_cast<size_t>(periods)) {
        return Prediction(0.0, 0.0, "WMA", "Insufficient data for Weighted Moving Average");
    }
    
    std::vector<double> temperatures = extractTemperatures(data);
    
    // Calculate weighted average (more recent values have higher weight)
    double weightedSum = 0.0;
    double totalWeight = 0.0;
    
    for (int i = 0; i < periods; ++i) {
        int dataIndex = temperatures.size() - periods + i;
        double weight = i + 1; // Linear weighting: 1, 2, 3, ...
        weightedSum += temperatures[dataIndex] * weight;
        totalWeight += weight;
    }
    
    double prediction = weightedSum / totalWeight;
    double confidence = std::min(0.85, static_cast<double>(periods) / 12.0);
    
    std::ostringstream desc;
    desc << "Weighted average of last " << periods << " readings (recent values weighted higher)";
    
    return Prediction(prediction, confidence, "WMA", desc.str());
}

WeatherPredictor::Prediction WeatherPredictor::linearRegression(const std::vector<WeatherData>& data, int daysAhead)
{
    if (data.size() < 3) {
        return Prediction(0.0, 0.0, "Linear", "Insufficient data for Linear Regression");
    }
    
    std::vector<double> temperatures = extractTemperatures(data);
    std::vector<double> days = extractDayNumbers(data);
    
    int n = temperatures.size();
    
    // Calculate means
    double meanX = std::accumulate(days.begin(), days.end(), 0.0) / n;
    double meanY = std::accumulate(temperatures.begin(), temperatures.end(), 0.0) / n;
    
    // Calculate slope (b) and intercept (a) for y = a + bx
    double numerator = 0.0;
    double denominator = 0.0;
    
    for (int i = 0; i < n; ++i) {
        double dx = days[i] - meanX;
        double dy = temperatures[i] - meanY;
        numerator += dx * dy;
        denominator += dx * dx;
    }
    
    if (denominator == 0) {
        return Prediction(meanY, 0.3, "Linear", "No trend detected - returning mean temperature");
    }
    
    double slope = numerator / denominator;
    double intercept = meanY - slope * meanX;
    
    // Predict for next day(s)
    double nextX = days.back() + daysAhead;
    double prediction = intercept + slope * nextX;
    
    // Calculate R-squared for confidence
    double totalSumSquares = 0.0;
    double residualSumSquares = 0.0;
    
    for (int i = 0; i < n; ++i) {
        double predicted = intercept + slope * days[i];
        double actualDiff = temperatures[i] - meanY;
        double residual = temperatures[i] - predicted;
        
        totalSumSquares += actualDiff * actualDiff;
        residualSumSquares += residual * residual;
    }
    
    double rSquared = 1.0 - (residualSumSquares / totalSumSquares);
    double confidence = std::max(0.1, std::min(0.95, rSquared));
    
    std::ostringstream desc;
    desc << "Linear trend projection " << daysAhead << " day(s) ahead (R² = " 
         << std::fixed << std::setprecision(2) << rSquared << ")";
    
    return Prediction(prediction, confidence, "Linear", desc.str());
}

WeatherPredictor::Prediction WeatherPredictor::seasonalTrend(const std::vector<WeatherData>& data, const std::string& targetMonth)
{
    if (data.empty()) {
        return Prediction(0.0, 0.0, "Seasonal", "No data available for seasonal analysis");
    }
    
    // Extract temperatures for the target month from historical data
    std::vector<double> monthlyTemps;
    
    for (const auto& record : data) {
        std::string timestamp = record.getTimestamp();
        if (timestamp.length() >= 7) {
            std::string month = timestamp.substr(5, 2); // Extract MM from YYYY-MM-DD
            if (month == targetMonth) {
                monthlyTemps.push_back(record.getTemperature());
            }
        }
    }
    
    if (monthlyTemps.empty()) {
        return Prediction(0.0, 0.0, "Seasonal", "No historical data for month " + targetMonth);
    }
    
    double mean = calculateMean(monthlyTemps);
    double stdDev = calculateStandardDeviation(monthlyTemps);
    double confidence = std::min(0.8, static_cast<double>(monthlyTemps.size()) / 30.0);
    
    std::ostringstream desc;
    desc << "Historical average for month " << targetMonth 
         << " (±" << std::fixed << std::setprecision(1) << stdDev << "°C, " 
         << monthlyTemps.size() << " samples)";
    
    return Prediction(mean, confidence, "Seasonal", desc.str());
}

std::vector<WeatherPredictor::Prediction> WeatherPredictor::getAllPredictions(const std::vector<WeatherData>& data)
{
    std::vector<Prediction> predictions;
    
    // Add different prediction methods
    predictions.push_back(simpleMovingAverage(data, 5));
    predictions.push_back(simpleMovingAverage(data, 10));
    predictions.push_back(weightedMovingAverage(data, 7));
    predictions.push_back(linearRegression(data, 1));
    predictions.push_back(linearRegression(data, 7));
    
    // Add seasonal prediction for current month
    if (!data.empty()) {
        std::string timestamp = data.back().getTimestamp();
        if (timestamp.length() >= 7) {
            std::string currentMonth = timestamp.substr(5, 2);
            predictions.push_back(seasonalTrend(data, currentMonth));
        }
    }
    
    return predictions;
}

double WeatherPredictor::calculateMean(const std::vector<double>& values)
{
    if (values.empty()) return 0.0;
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double WeatherPredictor::calculateStandardDeviation(const std::vector<double>& values)
{
    if (values.size() <= 1) return 0.0;
    
    double mean = calculateMean(values);
    double sumSquaredDiffs = 0.0;
    
    for (double value : values) {
        double diff = value - mean;
        sumSquaredDiffs += diff * diff;
    }
    
    return std::sqrt(sumSquaredDiffs / (values.size() - 1));
}

void WeatherPredictor::printPrediction(const Prediction& pred)
{
    std::cout << std::setw(8) << pred.method << " | ";
    std::cout << std::setw(6) << std::fixed << std::setprecision(1) << pred.value << "°C | ";
    std::cout << std::setw(4) << std::fixed << std::setprecision(0) << (pred.confidence * 100) << "% | ";
    std::cout << pred.description << std::endl;
}

void WeatherPredictor::printAllPredictions(const std::vector<Prediction>& predictions)
{
    if (predictions.empty()) {
        std::cout << "No predictions available." << std::endl;
        return;
    }
    
    std::cout << "\nTemperature Predictions:" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Method   | Temp   | Conf | Description" << std::endl;
    std::cout << "---------|--------|------|----------------------------------" << std::endl;
    
    for (const auto& pred : predictions) {
        if (pred.confidence > 0.0) { // Only show valid predictions
            printPrediction(pred);
        }
    }
    
    // Calculate consensus prediction (weighted by confidence)
    double weightedSum = 0.0;
    double totalWeight = 0.0;
    int validPredictions = 0;
    
    for (const auto& pred : predictions) {
        if (pred.confidence > 0.0) {
            weightedSum += pred.value * pred.confidence;
            totalWeight += pred.confidence;
            validPredictions++;
        }
    }
    
    if (validPredictions > 0 && totalWeight > 0) {
        double consensus = weightedSum / totalWeight;
        std::cout << "---------|--------|------|----------------------------------" << std::endl;
        std::cout << "Consensus| " << std::setw(6) << std::fixed << std::setprecision(1) 
                  << consensus << "°C |  N/A | Confidence-weighted average of all methods" << std::endl;
    }
}

std::vector<double> WeatherPredictor::extractTemperatures(const std::vector<WeatherData>& data)
{
    std::vector<double> temperatures;
    temperatures.reserve(data.size());
    
    for (const auto& record : data) {
        temperatures.push_back(record.getTemperature());
    }
    
    return temperatures;
}

std::vector<double> WeatherPredictor::extractDayNumbers(const std::vector<WeatherData>& data)
{
    std::vector<double> days;
    days.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        days.push_back(static_cast<double>(i)); // Simple sequential numbering
    }
    
    return days;
}
