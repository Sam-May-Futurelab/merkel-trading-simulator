#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <limits>
#include "OrderBookEntry.h"
#include "CSVReader.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input; 
    currentTime = orderBook.getEarliestTime(); // Get the earliest time from the order book

    wallet.insertCurrency("BTC", 10.);

    while (true)
    {
        printMenu();
        input = getUserOption();
        if (input == 0) { // If getUserOption returned 0 (for "exit")
            break;        // Break out of the while loop
        }
        processUserOption(input);
    }
}


    void MerkelMain::printMenu()
{
    std::cout << "Current time is: " << currentTime << std::endl; // Moved here
    std::cout << "1: Print help\n2: Print exchange stats\n3: Make an Ask\n4: Make a bid\n5: Print wallet\n6: Continue\nType 'exit' to quit the program\n";
    std::cout << "========= \nType in 1-6 or 'exit': ";
}

void MerkelMain::printHelp()
{
    std::cout << "Help - Your aim is to make money. Analyse the market and make bids and asks." << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                    p, currentTime); // Changed "currentTime" to currentTime

        std::cout << "Asks seen: " << entries.size() << std::endl;
        if (!entries.empty()) // Check if entries is not empty before accessing price data
        {
            std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
            std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;   
            std::cout << "Avg ask: " << OrderBook::getAveragePrice(entries) << std::endl; // Added average price                                                       
        }
        else
        {
            std::cout << "No asks found for this time frame." << std::endl;
        }
                                                         
    }

/*     std::cout << "OrderBook contains : " << orders.size() << " entries" << std::endl;
    unsigned int bids = 0;
    unsigned int asks = 0;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == OrderBookType::ask)
        
        {
            asks++;
        }
        else if (e.orderType == OrderBookType::bid)
        {
            bids++;
        }
    }
                std::cout << "OrderBook asks: " << asks << " bids: " << bids << std::endl; */

}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product, price, amount eg ETH/BTC,200,0.5." << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Invalid input format. Please enter in the format: product,price,amount." << std::endl;
    }
    else 
    {
        try 
        {
            OrderBookEntry newOrder = CSVReader::stringsToOBE(tokens[1], 
                                                              tokens[2], 
                                                              currentTime, 
                                                              tokens[0], 
                                                              "ask");

            std::cout << "Created ask order: " << tokens[0] << " price: " << tokens[1] << " amount: " << tokens[2] << std::endl;

            if (wallet.canFulfillOrder(newOrder)) // Check if the wallet can fulfill the order
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(newOrder); // Insert the new order into the order book
            }
            else
            {
                std::cout << "Not enough funds in wallet to fulfill this order." << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: Invalid price or amount format." << std::endl;
        }
    }

    std::cout << "You entered: " << input << std::endl;
}

void MerkelMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product, price, amount eg ETH/BTC,200,0.5." << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Invalid input format. Please enter in the format: product,price,amount." << std::endl;
    }
    else 
    {
        try 
        {
            OrderBookEntry newOrder = CSVReader::stringsToOBE(tokens[1], 
                                                              tokens[2], 
                                                              currentTime, 
                                                              tokens[0], 
                                                              "bid");

            std::cout << "Created bid order: " << tokens[0] << " price: " << tokens[1] << " amount: " << tokens[2] << std::endl;

            if (wallet.canFulfillOrder(newOrder)) // Check if the wallet can fulfill the order
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(newOrder); // Insert the new order into the order book
            }
            else
            {
                std::cout << "Not enough funds in wallet to fulfill this order." << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: Invalid price or amount format." << std::endl;
        }
    }

    std::cout << "You entered: " << input << std::endl;}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl; 
}

int MerkelMain::getUserOption()
{
    std::string input;
    std::cin >> input;

    if (input == "exit")
    {
        std::cout << "Exiting program. Goodbye!" << std::endl;
        return 0;
    }

    int userOption;
    try
    {
        userOption = std::stoi(input);
    }
    catch (...)
    {
        std::cout << "Invalid input. Please enter a number between 1-6 or 'exit'." << std::endl;
        return -1;
    }

    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    switch (userOption)
    {
    case 1: printHelp(); break;
    case 2: printMarketStats(); break;
    case 3: enterAsk(); break;
    case 4: enterBid(); break;
    case 5: printWallet(); break;
    case 6: 
        {
            std::cout << "Going to next time frame..." << std::endl;

            std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime); // Matches asks to bids for the current time
            std::cout << "Sales: " << sales.size() << std::endl;
            for (OrderBookEntry& sale : sales)
            {
                std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            }

            currentTime = orderBook.getNextTime(currentTime); // Update current time to the next time frame
            break;
        }

    default: 
        std::cout << "Invalid choice. Please select a number between 1-6." << std::endl;
        break; // Added break for default case as good practice
    }
}
