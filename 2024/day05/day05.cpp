#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <unordered_map>
#include <vector>

#include <cstdlib>

void print_page_order(const std::vector<std::vector<int>>& order)
{
    for(auto row : order){
        for(auto col : row){
            std::cout << col << " ";
        }
        std::cout << "\n";
    }
}

void populate_page_order(std::string fileName, std::vector<std::vector<int>>& order)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(inFile, line))
    {
        std::vector<int> row;
        std::stringstream iss(line);
        int val1;
        char separator;
        while(iss >> val1){
            row.push_back(val1);
            iss >> separator;
        }

        order.push_back(row);
    }

    inFile.close();
}


void print_page_rules(const std::unordered_multimap<int,int>& rules)
{
    for(const auto& r : rules){
        std::cout << r.first << "|" << r.second << "\n";
    }
}

void populate_page_rules(std::string fileName, std::unordered_multimap<int,int>& rules)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(inFile, line))
    {
        std::stringstream iss(line);
        int val1, val2;
        char separator;
        iss >> val1 >> separator >> val2;
        if(iss){
            rules.insert(std::make_pair(val1, val2));
        }
    }

    inFile.close();
}


int part01(const std::unordered_multimap<int,int>& rules, const std::vector<std::vector<int>>& order)
{
    int middleTotal = 0;

    for(auto pageNumbers : order){
        int inOrderCount = 0;
        size_t numUpdates = pageNumbers.size();
        for(size_t i = 0; i < numUpdates; i++){
            for(size_t j = i+1; j < numUpdates; j++){
                // can this page come before the others listed after it?
                std::cout << "can page number " << pageNumbers[i] << " come before page number "
                    << pageNumbers[j] << "?\n";
                
                bool inOrder = false;
                for(auto itr = rules.begin(); itr != rules.end() && !inOrder; itr++ ){
                    if(itr->first == pageNumbers[i]){
                        std::cout << "found key: " << pageNumbers[i] << ", value is " << itr->second << "\n";
                        if(itr->second == pageNumbers[j]){
                            std::cout << "order is correct\n";
                            inOrder = true;
                            inOrderCount++;
                        }
                    }
                }

                std::cout << "inOrderCount: " << inOrderCount
                    << ", pageNumbers.size() " << pageNumbers.size() 
                    << ", i: " << i << "\n";
                if(inOrderCount == (pageNumbers.size() - i))
                {
                    std::cout << "find the middle value\n";
                }

                inOrderCount = 0;
            }
        }
    }

    return middleTotal;
}

int main(int argc, char* argv[])
{
    std::string rulesFile, orderFile;

    // multimap required due to duplicate key values
    std::unordered_multimap<int,int> pageRules;

    std::vector<std::vector<int>> pageOrder;

    if(argc < 3){
        std::cerr << "Usage: a.out rulesFile orderFile\n";
        return 1;
    }

    rulesFile = argv[1];
    orderFile = argv[2];

    populate_page_rules(rulesFile, pageRules);
    //print_page_rules(pageRules);

    populate_page_order(orderFile, pageOrder);
    //print_page_order(pageOrder);

    int rightOrderCount;
    rightOrderCount = part01(pageRules, pageOrder);

    return 0;
}