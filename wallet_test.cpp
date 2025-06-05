#include "Wallet.h"
#include <iostream>

int main()
{
    std::cout << "=== Wallet Function Testing ===" << std::endl;
    Wallet myWallet{};
    
    std::cout << "\n1. Testing insertCurrency function:" << std::endl;
    std::cout << "Initial wallet state:" << std::endl;
    std::cout << myWallet.toString() << std::endl;
    
    std::cout << "Inserting 100 BTC..." << std::endl;
    myWallet.insertCurrency("BTC", 100);
    std::cout << "Wallet contents: " << myWallet.toString() << std::endl;
    
    std::cout << "Inserting 50 ETH..." << std::endl;
    myWallet.insertCurrency("ETH", 50);
    std::cout << "Wallet contents: " << myWallet.toString() << std::endl;
    
    std::cout << "Adding 25 more BTC (testing adding to existing currency)..." << std::endl;
    myWallet.insertCurrency("BTC", 25);
    std::cout << "Wallet contents: " << myWallet.toString() << std::endl;
    
    std::cout << "\n2. Testing containsCurrency function:" << std::endl;
    std::cout << "Does wallet contain 125 BTC? " << (myWallet.containsCurrency("BTC", 125) ? "Yes" : "No") << std::endl;
    std::cout << "Does wallet contain 130 BTC? " << (myWallet.containsCurrency("BTC", 130) ? "Yes" : "No") << std::endl;
    std::cout << "Does wallet contain 50 ETH? " << (myWallet.containsCurrency("ETH", 50) ? "Yes" : "No") << std::endl;
    std::cout << "Does wallet contain 100 USDT? " << (myWallet.containsCurrency("USDT", 100) ? "Yes" : "No") << std::endl;
    
    std::cout << "\n3. Testing removeCurrency function:" << std::endl;
    std::cout << "Current wallet state:" << std::endl;
    std::cout << myWallet.toString() << std::endl;
    
    std::cout << "Removing 25 BTC..." << std::endl;
    bool result1 = myWallet.removeCurrency("BTC", 25);
    std::cout << "Removal successful? " << (result1 ? "Yes" : "No") << std::endl;
    std::cout << "Wallet contents: " << myWallet.toString() << std::endl;
    
    std::cout << "Trying to remove 200 BTC (more than available)..." << std::endl;
    bool result2 = myWallet.removeCurrency("BTC", 200);
    std::cout << "Removal successful? " << (result2 ? "Yes" : "No") << std::endl;
    std::cout << "Wallet contents: " << myWallet.toString() << std::endl;
    
    std::cout << "Trying to remove 10 USDT (currency not in wallet)..." << std::endl;
    bool result3 = myWallet.removeCurrency("USDT", 10);
    std::cout << "Removal successful? " << (result3 ? "Yes" : "No") << std::endl;
    std::cout << "Wallet contents: " << myWallet.toString() << std::endl;
    
    std::cout << "\n4. Testing exception handling:" << std::endl;
    try {
        std::cout << "Trying to insert negative amount (-10 BTC)..." << std::endl;
        myWallet.insertCurrency("BTC", -10);
    } catch (const std::exception& e) {
        std::cout << "Exception caught for insertCurrency with negative amount!" << std::endl;
    }
    
    try {
        std::cout << "Trying to remove negative amount (-5 ETH)..." << std::endl;
        myWallet.removeCurrency("ETH", -5);
    } catch (const std::exception& e) {
        std::cout << "Exception caught for removeCurrency with negative amount!" << std::endl;
    }
    
    std::cout << "\nFinal wallet state:" << std::endl;
    std::cout << myWallet.toString() << std::endl;
    
    std::cout << "=== Testing Complete ===" << std::endl;
    return 0;
}
