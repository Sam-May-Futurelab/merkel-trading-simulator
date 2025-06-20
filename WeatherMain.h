#pragma once
#include "WeatherData.h"
#include "Candlestick.h"
#include "WeatherAnalyser.h"
#include <vector>
#include <string>

class WeatherMain
{
public:
    WeatherMain();
    
    //Main initialization function
    void init();

private:
    //Menu functions
    void printMenu();
    void printHelp();
    void computeCandlestickData();
    void plotCandlestickData();
    void filterAndPlotData();
    void predictTemperature();
    
    //User interaction
    int getUserOption();
    void processUserOption(int userOption);
      //Utility functions
    TimeFrame getTimeFrameFromUser();
    std::string getCountryFromUser();
    
    //Data members
    std::vector<WeatherData> weatherData;
    std::vector<Candlestick> currentCandlesticks;
    WeatherAnalyser analyser;
};
