#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
    public:
        Wallet();
        /** Insert currency to the wallet */
        void insertCurrency(std::string type, double amount);

        /** Remove currency from the wallet */
        bool removeCurrency(std::string type, double amount);

        /** Check if the wallet contains a specific currency */
        bool containsCurrency(std::string type, double amount);

        /** Get the amount of a specific currency */
        std::string toString();

        /** Check if the wallet can fulfill an order */
        bool canFulfillOrder(OrderBookEntry order);

        /** Process a sale, updating the wallet accordingly */
        void processSale(OrderBookEntry& sale);


    private:
        std::map<std::string, double> currencies; // Map to store currency type and amount

};