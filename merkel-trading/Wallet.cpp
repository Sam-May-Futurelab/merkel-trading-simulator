#include "Wallet.h"
#include <iostream>
#include "CSVReader.h"

Wallet::Wallet() 
{

}

    void Wallet::insertCurrency(std::string type, double amount)
    {
        double balance;
        if (amount <0)
        {
            throw std::exception{};
        }

        if (currencies.count(type) == 0)
        {
            balance = 0;
        }
        else
        {
            balance = currencies[type];
        }
        balance += amount;
        currencies[type] = balance;
    }    bool Wallet::removeCurrency(std::string type, double amount)
    {
        if (amount < 0)
        {
            throw std::exception{}; // Throw exception if amount < 0
        }

        if (currencies.count(type) == 0)
        {
            std::cout << "No currency for " << type << " in wallet." << std::endl;
            return false; // Currency type does not exist
        }
        else // Currency is there, is there enough?
        {
            if (containsCurrency(type, amount))
            {
                std::cout << "Removing " << type << " : " << amount << std::endl;
                currencies[type] -= amount;
                return true;
            }
            else
            {
                return false; // Not enough currency to remove
            }
        }
    }

    bool Wallet::containsCurrency(std::string type, double amount)
    {
        if (currencies.count(type) == 0) return false;
        else
            return currencies[type] >= amount;
    }

std::string Wallet::toString()
{
    std::string s;
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
}


bool Wallet::canFulfillOrder(OrderBookEntry order)
{
        std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
        // Ask
        if (order.orderType == OrderBookType::ask)
        {
            double amount = order.amount;
            std::string currency = currs[0]; // The currency we are selling
            std::cout << "Wallet::canFulfillOrder: " << currency << " : " << amount << std::endl;
            return containsCurrency(currency, amount);
        }
        // Bid
        if (order.orderType == OrderBookType::bid)
        {
            double amount = order.amount * order.price; // Amount in the currency we are buying
            std::string currency = currs[1]; // The currency we are buying
            std::cout << "Wallet::canFulfillOrder: " << currency << " : " << amount << std::endl;
            return containsCurrency(currency, amount);
        }

    return false;
}

void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    // Ask
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0]; // The currency we are selling
        double incomingAmount = sale.amount * sale.price; // Amount in the currency we are buying
        std::string incomingCurrency = currs[1]; // The currency we are buying
        
        currencies[incomingCurrency] += incomingAmount; // Add the incoming currency to the wallet
        currencies[outgoingCurrency] -= outgoingAmount; // Remove the outgoing currency from the wallet
    }
    // Bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0]; // The currency we are buying
        double outgoingAmount = sale.amount * sale.price; // Amount in the currency we are spending
        std::string outgoingCurrency = currs[1]; // The currency we are spending

        currencies[incomingCurrency] += incomingAmount; // Add the incoming currency to the wallet
        currencies[outgoingCurrency] -= outgoingAmount; // Remove the outgoing currency from the wallet
    }
}