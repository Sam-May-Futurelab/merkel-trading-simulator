#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 
#include <stdexcept> 
#include <cstddef>   

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;

    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            if (line.empty()) { // Skip empty lines
                continue;
            }
            try {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ',')); // Tokenises the line
                entries.push_back(obe); // Adds the OrderBookEntry to the vector
            } catch (const std::exception& e) {
                // Log the error or the problematic line itself if desired
                std::cerr << "Warning: Could not parse line: " << line << std::endl;
            }
        }//end of while
    }
    std::cout << "CSVReader::readCSV read " << entries.size() << "entries" << std::endl; // Message for successful read
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    std::string token;
    std::string::size_type start, end;

    // Find the beginning of the first token
    start = csvLine.find_first_not_of(separator, 0);

    // Loop while there are tokens to find
    while (start != std::string::npos)
    {
        // Find the end of the current token
        end = csvLine.find_first_of(separator, start);

        if (end == std::string::npos) // Last token in the line
        {
            token = csvLine.substr(start); // Get the rest of the string
            tokens.push_back(token);
            break; // No more tokens
        }
        else // Token found before a separator
        {
            token = csvLine.substr(start, end - start);
            tokens.push_back(token);
            // Find the beginning of the next token (skip over separators)
            start = csvLine.find_first_not_of(separator, end);
        }
    }
    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // Bad
        {
            std::cout << "Invalid line (received " << tokens.size() << " tokens): "; 
            for (size_t i = 0; i < tokens.size(); ++i) {
                std::cout << "[" << tokens[i] << "]";
                if (i < tokens.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl; 
            throw std::runtime_error("Invalid token count in CSV line"); // Throw a more specific exception
        }

        try { 
            price = std::stod(tokens[3]); // Converts the 4th token to double
            amount = std::stod(tokens[4]); // Converts the 5th token to double

        } catch (const std::exception& e) {
            std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl; // Message for bad float 3>>
            std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl; // Message for bad float 4>>
            throw;
        } 
            
    OrderBookEntry obe
    {
        price,
        amount,
        tokens[0], // timestamp
        tokens[1], // product
        OrderBookEntry::stringToOrderBookType(tokens[2]) // orderType
    };
            
    return obe;
}

OrderBookEntry CSVReader::stringsToOBE( std::string priceString, 
                                            std::string amountString, 
                                            std::string timestamp,
                                            std::string product, 
                                            std::string orderTypeString)
{
    double price, amount;
        try { 
        price = std::stod(priceString); 
        amount = std::stod(amountString); 

        } catch (const std::exception& e) {
            std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl; 
            std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl; 
            throw;
        }
    OrderBookEntry obe{price,
                    amount,
                    timestamp,
                    product,
                    OrderBookEntry::stringToOrderBookType(orderTypeString)};
    return obe;
    };
