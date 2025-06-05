#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "Wallet.h"

int main()
{

      MerkelMain mainApp;
      mainApp.init();
   
      // Uncomment the following lines to test the Wallet functionality
      /* 
      Wallet wallet;
      wallet.insertCurrency("BTC", 50.0);
      wallet.insertCurrency("USDT", 40.0);
      std::cout << wallet.toString() << std::endl;
   
      wallet.removeCurrency("USDT", 10.0);
      std::cout << wallet.toString() << std::endl; 
      */
   
      return 0;

}