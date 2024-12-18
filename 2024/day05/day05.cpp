#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <unordered_map>
#include <vector>

#include <cstdlib>

/*  Part 1
        exampleRules.txt, exampleOrder.txt      143
        pageOrder.txt, pageRules.txt            5964

    Part 2

*/

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
    int rightOrderCount = 0;

    int rowCount = 0;

    for(auto pageNumbers : order){
        size_t inOrderCount = 0;
        size_t numUpdates = pageNumbers.size();
        bool inOrder = true;
        for(size_t i = 0; i < numUpdates && inOrder; i++){
            // can this page, pageNumbers[i] come before the others listed after it?
            for(size_t j = i+1; j < numUpdates && inOrder; j++){
                
                /*std::cout << "can page number " << pageNumbers[i] << " come before page number "
                    << pageNumbers[j] << "?\n";
                */
                
                bool foundRule = false;
                for(auto itr = rules.begin(); itr != rules.end() && !foundRule; itr++ ){
                    if(itr->first == pageNumbers[i]){
                        //std::cout << "found key: " << pageNumbers[i] << ", value is " << itr->second << "\n";
                        if(itr->second == pageNumbers[j]){
                            //std::cout << "foundRule, correct order\n";
                            foundRule = true;
                        }
                    }
                }
                if(!foundRule){
                    std::cout << "row number: "<< rowCount << ", page " << pageNumbers[i] << " is out of order\n";
                    inOrder = false;
                }
            }
            if(inOrder){
                inOrderCount++;
            }
        }

        if(inOrderCount == numUpdates){
            rightOrderCount++;
            int middlePageIndex = numUpdates / 2;
                middleTotal += pageNumbers[middlePageIndex];
        }

        rowCount++;
    }

    std::cout << "rightOrderCount: " << rightOrderCount << "\n";
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

    int middleTotal;
    middleTotal = part01(pageRules, pageOrder);

    std::cout << "part 1 middle total: " << middleTotal << "\n";

    return 0;
}