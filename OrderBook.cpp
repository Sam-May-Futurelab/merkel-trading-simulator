#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>


/** construct, reading a csv data file */
       OrderBook::OrderBook(std::string filename)
       {
            orders = CSVReader::readCSV (filename); // Reads the CSV file and populate the order book

       }


    /** return vector of all known products in the dataset */
        std::vector<std::string> OrderBook::getKnownProducts()
        {
            std::vector<std::string> products;

            std::map<std::string, bool> prodMap; 

            for (OrderBookEntry& e : orders)
            {
                prodMap[e.product] = true; 
            }

            // now flatten the map to a vector of strings
            for (auto const& e : prodMap)
            {
                products.push_back(e.first);
            }

            return products; 
        }


    /** return vector of Orders according to the sent filters */
        std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                                std::string product,
                                                std::string timestamp)
        {
            std::vector<OrderBookEntry> orders_sub;

            for (OrderBookEntry& e : orders)
            {
                if (e.orderType == type &&
                    e.product == product &&
                    e.timestamp == timestamp )

                {
                    orders_sub.push_back(e); // Add the entry to the sub-vector if it matches the filters
                }

            }

            return orders_sub;
        }


            double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
            {
                double max = orders[0].price;
                for (OrderBookEntry & e : orders)
                {
                    if (e.price > max)max = e.price; // Update max if the current price is higher
            }
            return max;
        }

                    double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
            {
                double min = orders[0].price;
                for (OrderBookEntry & e : orders)
                {
                    if (e.price < min)min = e.price; // Update max if the current price is higher
            }
            return min;
        }

            std::string OrderBook::getEarliestTime()
            {
                return orders[0].timestamp;
            }

            std::string OrderBook::getNextTime(std::string timestamp)
            {
                std::string next_timestamp = "";
                for (OrderBookEntry& e : orders)
                {
                    if (e.timestamp > timestamp)
                    {
                        next_timestamp = e.timestamp;
                        break;
                    }
                }
                if (next_timestamp == "")
                {
                    next_timestamp = orders[0].timestamp; // If no next time found, return the first timestamp
                }   
                return next_timestamp;
            }

            double OrderBook::getAveragePrice(std::vector<OrderBookEntry>& orders)
            {
                if (orders.empty())
                {
                    return 0.0; // Or throw an exception, depending on desired behavior for empty list
                }
                double sum = 0.0;
                for (OrderBookEntry& e : orders)
                {
                    sum += e.price;
                }
                return sum / orders.size();
            }


            void OrderBook::insertOrder(OrderBookEntry& order)
            {
                orders.push_back(order); // Add the new order to the order book
                std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
            }

            std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp )
            {
                // Separate bids and asks
                std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                            product, 
                                                            timestamp);

                std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                            product, 
                                                            timestamp);

                std::vector<OrderBookEntry> sales;
                
                std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

                std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

                for (OrderBookEntry& ask : asks)
                {
                    for (OrderBookEntry& bid : bids)
                    {
                        if (bid.price >= ask.price) // Match found
                        {
                            // Create a new sale entry
                            OrderBookEntry sale = {
                                ask.price, 
                                0, 
                                timestamp, 
                                product, 
                                OrderBookType::sale}; 



                            if (bid.amount == ask.amount)
                            {
                                sale.amount = ask.amount;
                                sales.push_back(sale);
                                bid.amount = 0;
                                break;
                            }                            if (bid.amount > ask.amount) 
                            {
                                sale.amount = ask.amount; 
                                sales.push_back(sale);
                                bid.amount = bid.amount - ask.amount; // Reduce the bid amount
                                break;
                            }

                            if (bid.amount < ask.amount) 
                            {
                                sale.amount = bid.amount;
                                sales.push_back(sale);
                                ask.amount = ask.amount - bid.amount; // Reduce the ask amount
                                bid.amount = 0;
                                continue;  
                            }                        }
                    }
                }
                
                return sales;
            }