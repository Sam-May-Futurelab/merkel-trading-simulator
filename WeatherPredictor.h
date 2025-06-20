#pragma once

#include <vector>
#include <string>
#include "WeatherData.h"

class WeatherPredictor
{
public:
    // Prediction result structure
    struct Prediction
    {
        double value;           // Predicted temperature
        double confidence;      // Confidence level (0-1)
        std::string method;     // Method used for prediction
        std::string description; // Human-readable description
        
        Prediction(double val, double conf, const std::string& meth, const std::string& desc)
            : value(val), confidence(conf), method(meth), description(desc) {}
    };

    // Moving average prediction (simple and weighted)
    static Prediction simpleMovingAverage(const std::vector<WeatherData>& data, int periods = 5);
    static Prediction weightedMovingAverage(const std::vector<WeatherData>& data, int periods = 5);
    
    // Linear regression prediction
    static Prediction linearRegression(const std::vector<WeatherData>& data, int daysAhead = 1);
    
    // Seasonal trend prediction (based on historical same-month data)
    static Prediction seasonalTrend(const std::vector<WeatherData>& data, const std::string& targetMonth);
    
    // Combined prediction using multiple methods
    static std::vector<Prediction> getAllPredictions(const std::vector<WeatherData>& data);
    
    // Utility functions
    static double calculateMean(const std::vector<double>& values);
    static double calculateStandardDeviation(const std::vector<double>& values);
    static void printPrediction(const Prediction& pred);
    static void printAllPredictions(const std::vector<Prediction>& predictions);

private:
    // Helper for extracting temperatures from weather data
    static std::vector<double> extractTemperatures(const std::vector<WeatherData>& data);
    
    // Helper for time-based calculations
    static std::vector<double> extractDayNumbers(const std::vector<WeatherData>& data);
};
