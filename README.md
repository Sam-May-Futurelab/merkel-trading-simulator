# Weather Analysis Toolkit

A C++ command-line technical analysis toolkit for visualizing and predicting weather data.

## Project Overview

This project implements a comprehensive weather data analysis system with candlestick visualization and temperature prediction capabilities. It was developed as part of the CM2005 Object-Oriented Programming midterm assignment.

## Features

### ✅ Completed Tasks

1. **Candlestick Data Computation (Task 1)**
   - Computes OHLC (Open, High, Low, Close) candlestick data for temperature
   - Supports multiple timeframes: Hourly, Daily, Monthly, Yearly
   - Processes data by country

2. **ASCII Candlestick Plotting (Task 2)**
   - Text-based candlestick charts using ASCII characters
   - Y-axis temperature scale and timeline axis
   - Professional legend and symbols:
     - `|` = Temperature wick (high-low range)
     - `#` = Rising candle body (Close > Open)
     - `[.]` = Falling candle body (Close < Open)
     - `-` = Doji (Open = Close)

3. **Data Filtering (Task 3)**
   - Filter by date range (YYYY-MM-DD format)
   - Filter by temperature range (min/max values)
   - Filter by country code

4. **Temperature Prediction (Task 4)**
   - Simple Moving Average (5 and 10 periods)
   - Weighted Moving Average (7 periods)
   - Linear Regression (1 and 7 days ahead)
   - Seasonal Trend Analysis
   - Consensus prediction with confidence weighting
   - Statistical analysis and comparison

## Object-Oriented Design

The project follows OOP principles with modular, maintainable code:

### Core Classes

- **WeatherMain**: Main application controller and menu system
- **WeatherData**: Data model for individual weather records
- **WeatherCSVReader**: CSV parsing and data loading
- **WeatherAnalyser**: Data filtering and candlestick computation
- **Candlestick**: OHLC data representation
- **WeatherPredictor**: Temperature prediction algorithms

### Key Features

- Encapsulation: Private data members with public accessors
- Modularity: Separate classes for distinct responsibilities
- Polymorphism: Template-based design for extensibility
- Code reuse: Static utility functions and shared algorithms

## Data Format

Works with real weather_data.csv containing:
- Country-based temperature data in columns (e.g., GB_temperature, US_temperature)
- Hourly timestamps
- 28 European countries
- 280,000+ weather records

## Building and Running

### Compilation

```bash
g++ -o weather_main.exe weather_main.cpp WeatherMain.cpp WeatherCSVReader.cpp WeatherAnalyser.cpp Candlestick.cpp WeatherData.cpp WeatherPredictor.cpp
```

### Usage

```bash
./weather_main.exe
```

### Menu Options

1. **Compute candlestick data** - Calculate OHLC data for a country/timeframe
2. **Plot candlestick data (text)** - Display ASCII candlestick chart
3. **Filter and plot data** - Apply filters and show results
4. **Predict temperature** - Run prediction algorithms
5. **Help** - Show help information
6. **Exit** - Quit the program

## Example Output

### Candlestick Chart
```
ASCII Candlestick Chart:
========================

Temperature Range: -4.6°C to 7.7°C

Y-Scale  Date        Chart                                    Open  High  Low   Close Trend
-------  ----------- ---------------------------------------- ----- ----- ----- ----- -----
  -1.3° 1980-01-01      |||||-|||||||                        -1.6  0.5   -3.1  -1.6  DOWN
  -1.7° 1980-01-02     ||||[.]||||||                         -1.6  0.2   -3.6  -2.2  DOWN
   2.1° 1980-01-03         ################||||||||||||      -2.2  6.2   -2.0  2.4    UP
```

### Temperature Predictions
```
Temperature Predictions:
========================
Method   | Temp   | Conf | Description
---------|--------|------|----------------------------------
SMA      |  2.3°C | 50 % | Average of last 5 temperature readings
WMA      |  2.2°C | 58 % | Weighted average of last 7 readings
Linear   |  8.4°C | 10 % | Linear trend projection 1 day(s) ahead
Seasonal |  4.1°C | 80 % | Historical average for month 02
Consensus|  2.9°C | N/A  | Confidence-weighted average of all methods
```

## File Structure

```
├── weather_main.cpp          # Main entry point
├── WeatherMain.cpp/.h        # Application controller
├── WeatherData.cpp/.h        # Weather data model
├── WeatherCSVReader.cpp/.h   # CSV file parser
├── WeatherAnalyser.cpp/.h    # Data analysis and filtering
├── Candlestick.cpp/.h        # OHLC candlestick representation
├── WeatherPredictor.cpp/.h   # Temperature prediction algorithms
├── weather_data.csv          # Weather dataset (not in git)
├── README.md                 # This file
├── datapackage.json          # Data package metadata
└── .gitignore               # Git ignore file
```

## Implementation Notes

### Written Without AI Assistance
All core algorithms and class designs were implemented from scratch:
- Candlestick OHLC computation logic
- ASCII chart rendering and scaling
- Moving average calculations
- Linear regression implementation
- Data filtering and sorting algorithms

### AI-Assisted Components
- Code structure suggestions and best practices
- Documentation formatting
- Error handling improvements
- Performance optimizations

## Submission Components

1. **Source Code**: All .cpp and .h files
2. **Executable**: weather_main.exe (compiled binary)
3. **Documentation**: README.md and inline code comments
4. **Data Package**: datapackage.json with dataset metadata
5. **Version Control**: Git repository with commit history

## Technical Specifications

- **Language**: C++11/14 compatible
- **Compiler**: GCC (tested with MinGW)
- **Platform**: Windows (PowerShell compatible)
- **Dependencies**: Standard C++ library only
- **Data Size**: Processes 280,000+ weather records efficiently
- **Memory Usage**: Optimized for large dataset handling

## Academic Integrity

This project demonstrates original implementation of:
- Object-oriented design patterns
- Mathematical algorithms for data analysis
- Text-based visualization techniques
- Statistical prediction methods

Code comments clearly indicate which portions were developed independently versus with assistance.

---

**Original Weather Data Source:**
Open Power System Data: https://doi.org/10.25832/weather_data/2020-09-16














