// #include <string>
// #include <iostream>
// #include <vector>
// #include <fstream>

// std::vector<std::string> tokenise(std::string csvLine, char separator)
// {
//     std::vector<std::string> tokens;

//     signed int start, end;
//     std::string token;

//     start = csvLine.find_first_not_of(separator, 0);
   
//     do{

//         end = csvLine.find_first_of(separator, start);

//         if(start == csvLine.length() || start == end) break;

//         if (end >= 0) token = csvLine.substr(start, end - start);
//         else token = csvLine.substr(start, csvLine.length() - start);

//         tokens.push_back(token);
//         start = end + 1;

//     }while (end != std::string::npos);
//     return tokens;
// }


// int main()
// {
//     // std::vector<std::string> tokens;
//     // std::string s = "2020/03/17 17:01:24.884492,ETH/BTC,bid,0.02187308,7.44564869";

//     // tokens = tokenise(s, ',');  

//     // for (std::string& t : tokens)
//     // {
//     //     std::cout << t << std::endl;
//     // }



//     std::ifstream csvFile{"orderBook.csv"};
//     std::string line;
//     std::vector<std::string> tokens;

//     if (csvFile.is_open()) 
//     {
//         std::cout << "File open " << std::endl; // Message for successful open

//         while(std::getline(csvFile, line))
//         {
//             std::cout << "Read line: " << line << std::endl;
//             tokens = tokenise(line, ','); // Tokenises the line
//             if (tokens.size() != 5) // Bad
//             {
//                 std::cout << "Invalid line: " << std::endl; // Message for invalid line
//                 continue;
//             }

//             // We have 5 tokens

//             try { 
//                 double price = std::stod(tokens[3]); // Converts the 4th token to double
//                 double amount = std::stod(tokens[4]); // Converts the 5th token to double
//                 std::cout << "Price: " << price << ":" << amount << std::endl; // Prints the price and amount

//             } catch (const std::invalid_argument& ia) {
//                 std::cout << "Bad float! " << tokens[3] << std::endl; // Message for bad float 3>>
//                 std::cout << "Bad float! " << tokens[4] << std::endl; // Message for bad float 4>>
//                 break;
//             } 


//             for (std::string& t : tokens)
//             {
//                 std::cout << t << std::endl; // Prints the tokens
//             }
//         }

//         csvFile.close(); // Closes the file

//     }

//     else
//     {
//         std::cout << "Could not open file" << std::endl; // Message for failed open
//     }


//     return 0;
// }